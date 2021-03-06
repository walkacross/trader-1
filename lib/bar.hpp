
#ifndef __PROGRESS_BAR_HPP_
#define __PROGRESS_BAR_HPP_

#include <iostream>
#include <string>

void progress_bar(unsigned int current, unsigned int total, std::string label) {
    std::string bar;
    for(unsigned int i = 0; i < 40; i++) {
        if(i < label.length()) bar += label[i];
        else bar += " ";
    }
    bar += "[";

    unsigned int bar_width = 50;
    unsigned int pos = (int)(bar_width * current / total);
    for(unsigned int i = 0; i < bar_width; i++) {
        if(i < pos) bar += "|";
        else bar += " ";
    }
    bar += "] (" + std::to_string((int)(100 * current / total)) + "%)\r";
    std::cout << bar;
    std::cout.flush();
    if(current == total - 1) std::cout << "\n";
}

#endif
