
#ifndef __DATA_HPP_
#define __DATA_HPP_

#include <vector>
#include <string>

std::vector<std::string> read_ticker_list(std::string path);

bool data_exists(std::string ticker);

void download(std::string ticker);

std::vector<std::vector<double>> read_pair(std::string path);

#endif
