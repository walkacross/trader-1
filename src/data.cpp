
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>

#include "../lib/data.hpp"

bool data_exists(std::string ticker) {
    std::ifstream f("./data/" + ticker + ".csv");
    return f.is_open();
}

bool download(std::string ticker) {
    if (!data_exists(ticker)) {
        std::string cmd = "./python/download.py " + ticker;
        std::system(cmd.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return data_exists(ticker);
}

std::vector<std::string> read_lines(std::string path) {
    std::vector<std::string> lines;

    std::ifstream f(path);
    if(f.is_open()) {
        std::string line;
        while(std::getline(f, line)) {
            if (line.compare("\n") != 0) {
                lines.push_back(line);
            }
        }
        f.close();
    }

    return lines;
}

std::vector<std::vector<double>> load_data(std::string path) {
    std::vector<std::vector<double>> data;

    std::ifstream f(path);
    if(f.is_open()) {
        std::string line, val;
        while(std::getline(f, line)) {
            if (line.compare("\n") != 0) {
                std::vector<double> x;
                for (unsigned int i = 0; i < line.length(); i++) {
                    if (line[i] != ' ') val += line[i];
                    else {
                        x.push_back(std::stod(val));
                        val = "";
                    }
                }
                data.push_back(x);
                x.clear();
            }
        }
        f.close();
    }

    return data;
}

void sort_correlating_pairs(std::vector<std::string> &tickers, std::vector<double> &correlation) {
    // selection sort (max ... min)
    for(unsigned int i = 0; i < tickers.size() - 1; i++) {
        std::string max_ticker = "";
        double max_correlation = 0.00;
        int k = 0;
        for(unsigned int j = i; j < tickers.size(); j++) {
            if(correlation[j] > max_correlation) {
                max_ticker = tickers[j];
                max_correlation = correlation[j];
                k = j;
            }
        }

        std::string temp_ticker = tickers[i];
        double temp_correlation = correlation[i];
        tickers[i] = max_ticker;
        tickers[k] = temp_ticker;
        correlation[i] = max_correlation;
        correlation[k] = temp_correlation;
    }
}






