
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include "../lib/encoder.hpp"

void Encoder::add_layer(std::vector<unsigned int> conv_shape, unsigned int stride, std::vector<unsigned int> pool_shape) {
    layer.push_back(ConvPool2D(conv_shape, stride, pool_shape));
}

std::vector<std::vector<double>> Encoder::encode(std::vector<std::vector<double>> &data) {
    std::vector<std::vector<double>> *x;
    std::vector<std::vector<double>> convolved, pooled;

    x = &data;

    for(unsigned int l = 0; l < layer.size(); l++) {
        // convolution
        convolved.clear();
        std::vector<std::vector<double>> *kernel = layer[l].kernel_mat();
        for(unsigned int r = 0; r <= (*x).size() - layer[l].conv_shape()[0]; r += layer[l].conv_stride()) {
            std::vector<double> conv_row;
            for(unsigned int c = 0; c <= (*x)[r].size() - layer[l].conv_shape()[1]; c += layer[l].conv_stride()) {
                double dot = 0.00;
                for(unsigned int i = r; i < r + layer[l].conv_shape()[0]; i++) {
                    for(unsigned int j = c; j < c + layer[l].conv_shape()[1]; j++) {
                        dot += (*x)[i][j] * (*kernel)[i-r][j-c];
                    }
                }
                conv_row.push_back(dot > 0.00 ? dot : 0.00); // ReLU
            }
            convolved.push_back(conv_row);
            conv_row.clear();
        }

        // max pooling
        pooled.clear();
        for(unsigned int r = 0; r <= convolved.size() - layer[l].pool_shape()[0]; r += layer[l].pool_shape()[0]) {
            std::vector<double> pool_row;
            for(unsigned int c = 0; c <= convolved[r].size() - layer[l].pool_shape()[1]; c += layer[l].pool_shape()[1]) {
                double max = 0.00;
                for(unsigned int i = r; i < r + layer[l].pool_shape()[0]; i++) {
                    for(unsigned int j = c; j < c + layer[l].pool_shape()[1]; j++) {
                        if(convolved[i][j] > max) max = convolved[i][j];
                    }
                }
                pool_row.push_back(max);
            }
            pooled.push_back(pool_row);
            pool_row.clear();
        }

        x = &pooled;
    }

    data.clear(); // forget initial data

    return pooled;
}


void Encoder::save() {
    std::ofstream f1(path + "/layers");
    if(f1.is_open()) {
        for(unsigned int l = 0; l < layer.size(); l++) {
            // save parameters of each layer into each line
            f1 << layer[l].conv_shape()[0] << " ";
            f1 << layer[l].conv_shape()[1] << " ";
            f1 << layer[l].conv_stride() << " ";
            f1 << layer[l].pool_shape()[0] << " ";
            f1 << layer[l].pool_shape()[1] << " ";
            if(l != layer.size() - 1) f1 << "\n";
            // save kernel of each layer
            std::vector<std::vector<double>> *kernel = layer[l].kernel_mat();
            std::ofstream f2(path + "/kernels/kernel" + std::to_string(l));
            if(f2.is_open()) {
                // save each row of the kernel into each line
                for(unsigned int i = 0; i < (*kernel).size(); i++) {
                    for(unsigned int j = 0; j < (*kernel)[i].size(); j++) {
                        f2 << (*kernel)[i][j] << " ";
                    }
                    if(i != (*kernel).size() - 1) f2 << "\n";
                }
                f2.close();
            }
        }
        layer.clear();
        f1.close();
    }
}

void Encoder::load() {
    // load encoder parameters
    std::string line, val;
    std::ifstream f1, f2;
    f1.open(path + "/layers");
    if(f1.good()) {
        layer.clear();
        while(getline(f1, line)) {
            // read parameter values of each layer
            unsigned int stride = 0;
            std::vector<unsigned int> conv_shape, pool_shape;
            for(unsigned int i = 0; i < line.length(); i++) {
                if(line[i] != ' ') val += line[i];
                else {
                    if(conv_shape.size() < 2) conv_shape.push_back(std::stod(val));
                    else if(conv_shape.size() == 2 && stride == 0) stride = std::stoi(val);
                    else pool_shape.push_back(std::stod(val));
                    val = "";
                }
            }
            // read kernel of each layer
            std::vector<std::vector<double>> kernel;
            f2.open(path + "/kernels/kernel" + std::to_string(layer.size()));
            if(f2.good()) {
                while(getline(f2, line)) {
                    std::vector<double> row;
                    for(unsigned int i = 0; i < line.length(); i++) {
                        if(line[i] != ' ') val += line[i];
                        else {
                            row.push_back(std::stod(val));
                            val = "";
                        }
                    }
                    kernel.push_back(row);
                    row.clear();
                }
                f2.close();
            }
            // construct layer
            layer.push_back(ConvPool2D(conv_shape, stride, pool_shape));
            layer[layer.size() - 1].init_kernel(kernel);
        }
        f1.close();
    }
    // display encoder parameters
    
}

