
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include "../lib/bar.hpp"
#include "../lib/data.hpp"
#include "../lib/linear.hpp"

void setup(std::string model) {
    std::vector<std::string> required = {
        "./models/" + model + "/",
        "./models/" + model + "/linear",
        "./models/" + model + "/dnn"
    };

    for(unsigned int i = 0; i < required.size(); i++) {
        std::filesystem::directory_entry entry { required[i] };
        if(!entry.exists()) {
            std::string cmd = "mkdir " + required[i];
            std::system(cmd.c_str());
        }
    }
}

void train() {
    std::vector<std::string> tickers = read_lines("./data/tickers.csv");
    // initial training module
    for(unsigned int i = 0; i < 1; i++) { // tickers.size()
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
                std::cout << "Residual data sampled in [./temp/residual]\n";
                
                // read residual data
                std::vector<std::vector<double>> raw_residuals = load_data("./temp/residual");
                std::vector<double> y_return;
                for (std::string val: read_lines("./temp/y_out")) {
                    y_return.push_back(std::stod(val));
                }

                std::cout << y_return.size() << std::endl;
                std::cout << raw_residuals.size() << std::endl;

                // initial encoding of residual data (raw residual --> synthesized residual)

                // secondary encoding (synthesized residual --> feature map)

                // train neural network (reinforcement)
            }
            else {
                std::cout << "Not enough correlating pairs found. Model rejected.\n";
            }

        }
    }
}

