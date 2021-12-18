
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "../lib/bar.hpp"
#include "../lib/data.hpp"
#include "../lib/linear.hpp"

int main(int argc, char *args[])
{
    std::vector<std::string> tickers = read_ticker_list("./data/tickers.csv");

    for(unsigned int i = 0; i < 1; i++) { // tickers.size()
        if(download(tickers[i])) {
            std::vector<double> correlation;
            std::vector<std::string> correlating_tickers;

            std::cout << "\n=========================" << tickers[i] << "=========================\n\n";

            for(unsigned int j = 0; j < tickers.size(); j++) {
                std::string bar_label = "Computing pair correlation [" + tickers[i] + "-" + tickers[j] + "]";
                progress_bar(j, tickers.size(), bar_label);
                if((i != j) && download(tickers[j])) {
                    // sample pair data
                    std::string cmd = "./python/pair.py " + tickers[j] + " " + tickers[i]; // x, y
                    std::system(cmd.c_str());
                    // compute pair correlation
                    std::vector<std::vector<double>> pair = read_pair("./temp/pair");
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
                        }
                    }
                }
            }

            if(correlating_tickers.size() >= 10) {
                // sort correlating pairs (max ... min)
                sort_correlating_pairs(correlating_tickers, correlation);

                std::cout << "\n\nPairs correlated with " << tickers[i] << std::endl;
                for(unsigned int j = 0; j < correlating_tickers.size(); i++) {
                    std::cout << tickers[i] << "-" << correlating_tickers[j] << ": " << correlation[j] << std::endl;
                }
                std::cout << "\n";

                // sample residual

                // encode residual data

                // train neural network (reinforcement)
            }
            else {
                std::cout << "Not enough correlating pairs found. Model rejected.\n";
            }

        }
    }

    return 0;
}
