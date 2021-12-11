
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "../lib/data.hpp"

int main(int argc, char *args[])
{
    // download macroeconomic indicators
    std::vector<std::string> macro_indicators = {
        "spy",
        "gld",
        "slv",
        
    };

    // download macroindicators

    std::vector<std::string> tickers = read_ticker_list("./data/tickers.csv");

    for(unsigned int i = 0; i < tickers.size(); i++) {
        download(tickers[i]);
        std::vector<std::string> correlating_tickers;
        for(unsigned int j = 0; j < tickers.size(); j++) {
            if(i != j) {
                if(!data_exists(tickers[j])) { download(tickers[j]); }
                // sample pair data
                std::string cmd = "./python/pair.py " + tickers[j] + " " + tickers[i]; // x, y
                std::system(cmd.c_str());
                // compute pair correlation
                std::vector<std::vector<double>> pair = read_pair("./temp/pair");
                if(pair.size() != 1) {
                    std::vector<double> x, y;
                    for(unsigned int i = 0; i < pair.size(); i++ ) {
                        x.push_back(pair[i][0]);
                        y.push_back(Pair[i][1]);
                    }
                    LinearRegression linear;
                    double correlation = linear.fit(x, y);
                    if(correlation > 0.50) {}
                }
            }
        }

        

        // sample residual data (macroindices and top 5 correlating pair)

        // encode residual data

        // train neural network (reinforcement)

    }

    return 0;
}
