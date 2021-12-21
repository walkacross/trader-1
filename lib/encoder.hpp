#ifndef __ENCODER_HPP_
#define __ENCODER_HPP_

#include <vector>

class Encoder
{
private:
    std::vector<std::vector<double>> kernel;

public:
    Encoder() {}
    void init_kernel();

    std::vector<double> conv2d(std::vector<std::vector<double>> &dat);
    std::vector<double> pool2d(std::vector<std::vector<double>> &dat);

    void save();
    void load();
    void terminate();
};

#endif
