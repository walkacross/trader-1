
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include "../lib/bar.hpp"
#include "../lib/data.hpp"
#include "../lib/linear.hpp"
#include "../lib/encoder.hpp"
#include "../lib/dnn.hpp"

void setup(std::string model) {
    std::vector<std::string> required = {
        "./models/" + model + "/",
        "./models/" + model + "/linear",
        "./models/" + model + "/encoder",
        "./models/" + model + "/encoder/kernels",
        "./models/" + model + "/dnn"
    };

    for(std::string dir: required) {
        std::filesystem::directory_entry entry { dir };
        if(!entry.exists()) {
            std::string cmd = "mkdir " + dir;
            std::system(cmd.c_str());
        }
    }
}

void train() {
    std::vector<std::string> tickers = read_lines("./data/tickers.csv");
    // initial training module
    for(unsigned int i = 8; i < 9; i++) { // tickers.size()
        if(download(tickers[i])) {
            setup(tickers[i]);
            std::cout << "\n=========================" << tickers[i] << "=========================\n\n";

            // find correlating pairs
            std::vector<double> correlation;
            std::vector<std::string> correlating_tickers;
            for(unsigned int j = 0; j < tickers.size(); j++) {
                std::string bar_label = "Computing pair correlation [" + tickers[i] + "-" + tickers[j] + "]";
                progress_bar(j, tickers.size(), bar_label);
                if((i != j) && download(tickers[j])) {
                    // sample pair data
                    std::string cmd = "./python/pair.py " + tickers[j] + " " + tickers[i]; // x, y
                    std::system(cmd.c_str());
                    // compute pair correlation
                    std::vector<std::vector<double>> pair = load_data("./temp/pair");
                    if(pair.size() != 1) {
                        std::vector<double> x, y;
                        for(unsigned int i = 0; i < pair.size(); i++ ) {
                            x.push_back(pair[i][0]);
                            y.push_back(pair[i][1]);
                        }

                        LinearRegression linear;
                        double r = linear.fit(x, y);
                        if(r > 0.50) {
                            correlating_tickers.push_back(tickers[j]);
                            correlation.push_back(r);
                            // plot
                            std::vector<double> param = linear.model(); // param[0]: slope, param[1]: bias
                            cmd = "./python/plot_pair.py " + tickers[j] + " " + tickers[i] 
                                + " " + std::to_string(param[0]) + " " + std::to_string(param[1]);
                            std::system(cmd.c_str());
                        }
                    }
                    else {
                        if((int)pair[0][0] == -999) break; // y ticker does not have enough data
                        else {}
                    }
                }
            }

            // build model
            if(correlating_tickers.size() > 10) {
                // sort correlating pairs (max ... min)
                sort_correlating_pairs(correlating_tickers, correlation);

                std::cout << "\nPairs correlated with " << tickers[i] << std::endl;
                for(unsigned int j = 0; j < correlating_tickers.size(); j++) {
                    if(j < 10) std::cout << "** ";
                    else std::cout << "   ";
                    std::cout << tickers[i] << "-" << correlating_tickers[j] << ": " << correlation[j] << "\n";
                }
                std::cout << "\n";

                // sample residual (top 10 correlating pairs)
                std::string cmd = "./python/residual.py ";
                for(unsigned int j = 0; j < 10; j++) {
                    cmd += correlating_tickers[j] + " ";
                }
                cmd += tickers[i];
                std::system(cmd.c_str());
                std::cout << "Residual data sampled in [./temp/residual] and [./temp/y_out]\n";

                // read residual data
                std::vector<std::vector<double>> map;
                std::vector<std::vector<std::vector<double>>> residual_map;
                for(std::vector<double> residual: load_data("./temp/residual")) {
                    map.push_back(residual);
                    if (map.size() == 10) {
                        residual_map.push_back(map);
                        map.clear();
                    }
                }
                // read buy-sell signal of y (sigmoid of 1-day return %)
                std::vector<double> y_signal;
                for(std::string val : read_lines("./temp/y_out")) {
                    y_signal.push_back(std::stod(val));
                }

                // encode residual maps into synthesized residual vector
                Encoder encoder(tickers[i]);
                encoder.add_layer({10, 3}, 1, {1, 2});
                encoder.load();

                std::vector<std::vector<double>> synthesized_residual;
                for(unsigned int k = 0; k < residual_map.size(); k++) {
                    progress_bar(k, residual_map.size(), "Encoding synthesized residual vector...");
                    synthesized_residual.push_back(encoder.encode(residual_map[k])[0]);
                }
                residual_map.clear();
                encoder.save();

                // train neural network (reinforcement)
                

            }
            else {
                std::cout << "Not enough correlating pairs found. Model rejected.\n";
            }
        }
    }
}

