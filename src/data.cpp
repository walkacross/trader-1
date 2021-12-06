
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "../lib/data.hpp"

std::vector<std::string> read_ticker_list(std::string path) {
    std::vector<std::string> tickers;

    std::ifstream f(path);
    if(f.is_open()) {
        std::string line;
        while(std::getline(f, line)) {
            tickers.push_back(line);
        }
        f.close();
    }

    return tickers;
}

bool data_exists(std::string ticker) {
    std::ifstream f("./data/" + ticker + ".csv");
    return f.is_open();
}
