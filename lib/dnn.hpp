
#ifndef __DNN_HPP_
#define __DNN_HPP_

#include <cstdlib>
#include <vector>
#include <string>

double relu(double x);
double relu_prime(double x);

double mse(std::vector<double> &y, std::vector<double> &yhat);

class Node
{
private:
    double sum;
    double act;
    double error;
    std::vector<double> weights;
public:
    Node() {}
    Node(unsigned int in_features) {
        init();
        // initialize weight vector
        for(unsigned int i = 0; i < in_features; i++) {
            double val = -1.0 + (double)rand() / RAND_MAX * (1.0 - (-1.0));
            weights.push_back(val);
        }
    }

    double summation();
    double activation();
    double error_summation();
    std::vector<double> *weight_vector();

    void add_error_summation(double val);
    void set_summation(double val);
    void compute_activation();

    void init();
};

class Layer
{
private:
    std::vector<Node> nodes;
    std::vector<unsigned int> shape;
public:
    Layer(unsigned int in_features, unsigned out_features): shape({in_features, out_features}) {
        for(unsigned int n = 0; n < out_features; n++) {
            nodes.push_back(Node(in_features));
        }
    }

    unsigned int in_features();
    unsigned int out_features();
    std::vector<Node> *get_nodes();
};

class DeepNet
{
private:
    std::string name;
    std::vector<Layer> layers;
public:
    DeepNet(std::string model_name, std::vector<std::vector<double>> shape): name(model_name) {
        for(unsigned int l = 0; l < shape.size(); l++) {
            layers.push_back(Layer(shape[l][0], shape[l][1]));
        }
    }

    std::vector<double> predict(std::vector<double> &x);
    void fit(std::vector<double> &x, std::vector<double> &y, double alpha);
    void save();
    bool load();
};

#endif
