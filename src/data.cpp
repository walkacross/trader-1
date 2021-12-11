
#include <cstdlib>
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

void download(std::string ticker) {
    std::string cmd = "./python/download.py " + ticker;
    std::system(cmd.c_str());
}

std::vector<std::vector<double>> read_pair(std::string path) {
    std::vector<std::vector<double>> pair_data;

    std::ifstream f(path);
    if(f.is_open()) {
        std::string line, val;
        while(std::getline(f, line)) {
            std::vector<double> pair;
            for(unsigned int i = 0; i < line.length(); i++) {
                if(line[i] != ' ') val += line[i];
                else {
                    pair.push_back(std::stod(val));
                    val = "";
                }
            }
            pair_data.push_back(pair);
            pair.clear();
        }
        f.close();
    }

    return pair_data;
}

