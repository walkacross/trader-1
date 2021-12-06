
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
/*    for(unsigned int i = 0; i < macro_indicators.size(); i++) {
        std::string cmd;
        cmd = "./python/download.py " + macro_indicators[i];
        std::system(cmd.c_str());
    }*/

    std::vector<std::string> tickers = read_ticker_list("./data/tickers.csv");

    for(unsigned int i = 0; i < tickers.size(); i++) {
        if(data_exists(tickers[i])){}
    }

    return 0;
}
