
#ifndef __DATA_HPP_
#define __DATA_HPP_

#include <vector>
#include <string>

bool data_exists(std::string ticker);

bool download(std::string ticker);

std::vector<std::string> read_lines(std::string path);

std::vector<std::vector<double>> load_data(std::string path);

void sort_correlating_pairs(std::vector<std::string> &tickers, std::vector<double> &correlation);

double sigmoid_inverse(double x);

#endif
