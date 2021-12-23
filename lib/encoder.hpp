
#ifndef __ENCODER_HPP_
#define __ENCODER_HPP_

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <tuple>
#include <ctime>

class ConvPool2D
{
private:
    unsigned int conv_rows;
    unsigned int conv_cols;
    unsigned int stride;
    std::vector<std::vector<double>> kernel;

    unsigned int pool_rows;
    unsigned int pool_cols;
    bool padding;

public:
    ConvPool2D() {}
    ConvPool2D(std::vector<unsigned int> conv_shape, unsigned int _stride, std::vector<unsigned int> pool_shape, bool pad) {
        // initialize parameters
        conv_rows = conv_shape[0];
        conv_cols = conv_shape[1];
        stride = _stride;
        pool_rows = pool_shape[0];
        pool_cols = pool_shape[1];
        padding = pad;
        // initialize convolution kernel
        srand(time(NULL));
        for(unsigned int i = 0; i < conv_rows; i++) {
            std::vector<double> row;
            for(unsigned int j = 0; j < conv_cols; j++) {
                double init = -0.5 + (double)rand() / RAND_MAX * (0.5 - (-0.5)); // kernel values: -0.5 <= x <= 0.5
                row.push_back(init);
            }
            kernel.push_back(row);
            row.clear();
        }
    }
    // load layer from exisiting source
    ConvPool2D(std::vector<std::vector<double>> &kernel_source, unsigned int _stride, std::vector<unsigned int> pool_shape, bool pad) {
        // initialize parameters
        conv_rows = kernel_source.size();
        conv_cols = kernel_source[0].size();
        stride = _stride;
        pool_rows = pool_shape[0];
        pool_cols = pool_shape[1];
        padding = pad;
        // initialize kernel
        kernel = kernel_source;
        kernel_source.clear();
    }

    std::vector<std::vector<double>> *kernel_mat() { return &kernel; }
    std::vector<unsigned int> conv_shape() { return std::vector<unsigned int>({conv_rows, conv_cols}); }
    std::vector<unsigned int> pool_shape() { return std::vector<unsigned int>({pool_rows, pool_cols}); }
    unsigned int conv_stride() { return stride; }
    bool pad() { return padding; }
};

class Encoder
{
private:
    std::string path;
    std::vector<ConvPool2D> layer;
public:
    Encoder(std::string model_name): path("./models/" + model_name + "/encoder") {}
    void add_layer(std::vector<unsigned int> conv_shape, unsigned int stride, std::vector<unsigned int> pool_shape, bool pad);
    std::vector<std::vector<double>> encode(std::vector<std::vector<double>> &data);
//    void save();
//    void load();
};

#endif
