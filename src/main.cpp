
#include <iostream>
#include <cstdlib>

#include "../lib/train.hpp"

int main(int argc, char *argv[])
{
    std::string mode = argv[1];

    if(mode.compare("trade") == 0) {}
    else if(mode.compare("train") == 0) {
        train();
    }
    else if(mode.compare("update") == 0) {
    }
    else {}

    return 0;
}
