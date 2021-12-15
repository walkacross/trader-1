
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

bool download(std::string ticker) {
    if(!data_exists(ticker)) {
        std::string cmd = "./python/download.py " + ticker;
        std::system(cmd.c_str());
    }
    return data_exists(ticker);
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

void sort_correlating_pairs(std::vector<std::string> &tickers, std::vector<double> &correlation) {
    for(unsigned int i = 0; i < tickers.size() - 1; i++) {
        std::string max_ticker = "";
        double max_correlation = 0.00;
        for(unsigned int j = i + 1; j < tickers.size(); j++) {
            if(correlation[j] > max_correlation) {
                max_ticker = tickers[j];
                max_correlation = correlation[j];
            }
        }
        std::string temp_ticker = tickers[i];
        double temp_correlation = correlation[i];
        tickers[i] = max_ticker;
        correlation[i] = max_correlation;
    }
}
