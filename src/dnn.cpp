
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>

#include "../lib/dnn.hpp"

//--------------------------------------------------

double relu(double x) {
    return x > 0.00 ? x : 0.00;
}

double relu_prime(double x) {
    return x > 0.00 ? 1.00 : 0.00;
}

double mse(std::vector<double> &y, std::vector<double> &yhat) {
    double cost = 0.00;
    for(unsigned int i = 0; i < y.size(); i++) {
        cost += pow(y[i] - yhat[i], 2);
    }
    cost /= y.size();
    return cost;
}

//--------------------------------------------------

double Node::summation() {
    return sum;   
}

double Node::activation() {
    return act;
}

double Node::error_summation() {
    return error;
}

std::vector<double> * Node::weight_vector() {
    return &weights;
}

void Node::add_error_summation(double val) {
    error += val;
}

void Node::set_summation(double val) {
    sum = val;
}

void Node::compute_activation() {
    act = relu(sum);
}

void Node::init() {
    sum = 0.00;
    act = 0.00;
    error = 0.00;
}

//--------------------------------------------------

unsigned int Layer::in_features() {
    return shape[0];
}

unsigned int Layer::out_features() {
    return shape[1];
}

std::vector<Node> * Layer::get_nodes() {
    return &nodes;
}

//--------------------------------------------------

std::vector<double> DeepNet::predict(std::vector<double> &x) {
    std::vector<double> yhat;
    // fully connected forward propagation
    for(unsigned int l = 0; l < layers.size(); l++) {
        std::vector<Node> *nodes;
        nodes = layers[l].get_nodes();
        for(unsigned int n = 0; n < (*nodes).size(); n++) {
            double dot = 0.00;
            std::vector<double> *weights;
            weights = (*nodes)[n].weight_vector();
            (*nodes)[n].init();
            for(unsigned int i = 0; i < (*weights).size(); i++) {
                if(l == 0) {
                    dot += x[i] * (*weights)[i];
                }
                else {
                    dot += (*layers[l-1].get_nodes())[i].activation() * (*weights)[i];
                }
            }
            (*nodes)[n].set_summation(dot);
            (*nodes)[n].compute_activation();
            if(l == layers.size() - 1) yhat.push_back((*nodes)[n].activation());
        }
    }
    return yhat;
}

void DeepNet::fit(std::vector<double> &x, std::vector<double> &y, double alpha) {
    std::vector<double> yhat = predict(x);
    // stochastic gradient descent
    for(int l = layers.size() - 1; l >= 0; l--) {
        std::vector<Node> *nodes;
        nodes = layers[l].get_nodes();
        for(unsigned int n = 0; n < (*nodes).size(); n++) {
            std::vector<double> *weights;
            weights = (*nodes)[n].weight_vector();
            // compute gradient
            double delta, gradient = 0.00;
            if(l == layers.size() - 1) {
                delta = (-2.00 / y.size()) * (y[n] - yhat[n]) * relu_prime((*nodes)[n].summation());
            }
            else {
                delta = (*nodes)[n].error_summation() * relu_prime((*nodes)[n].summation());
            }
            for(unsigned int i = 0; i < (*weights).size(); i++) {
                if(l != 0) {
                    gradient = delta * (*layers[l-1].get_nodes())[i].activation();
                    (*layers[l-1].get_nodes())[i].add_error_summation(delta * (*weights)[i]);
                }
                else {
                    gradient = delta * x[i];
                }
                (*weights)[i] -= alpha * gradient;
            }
        }
    }
    yhat.clear();
}

void DeepNet::save() {
    std::string checkpoint = "./models/" + name + "/checkpoint";
    std::ofstream f(checkpoint);
    if(f.is_open()) {
        for(unsigned int l = 0; l < layers.size(); l++) {
            f << layers[l].in_features() << " " << layers[l].out_features() << " \n";
            std::vector<Node> *nodes;
            nodes = layers[l].get_nodes();
            for(unsigned int n = 0; n < (*nodes).size(); n++) {
                std::vector<double> *weights;
                weights = (*nodes)[n].weight_vector();
                for(unsigned int i = 0; i < (*weights).size(); i++) f << (*weights)[i] << " ";
                f << "\n";
            }
            f << "/ ";
            if(l != layers.size() - 1) f << "\n";
        }
        f.close();
    }
}

bool DeepNet::load() {
    bool loaded = false;
    std::string checkpoint = "./models/" + name + "/checkpoint";
    std::ifstream f(checkpoint);
    if(f.is_open()) {
        std::string line, val;
        std::vector<unsigned int> shape;
        bool have_shape = false;
        unsigned int n, k = 0;
        while(std::getline(f, line)) {
            for(unsigned int i = 0; i < line.length(); i++) {
                if(line[i] != ' ') val += line[i];
                else {
                    if(val.compare("/") == 0) {
                        n = 0; k = 0;
                        shape.clear();
                        have_shape = false;
                    }
                    else {
                        if(!have_shape) {
                            shape.push_back(std::stoi(val));
                            if(shape.size() == 2) {
                                layers.push_back(Layer(shape[0], shape[1]));
                                have_shape = true;
                            } else {}
                        }
                        else {
                            std::vector<Node> *nodes; nodes = layers[layers.size() - 1].get_nodes();
                            std::vector<double> *weights; weights = (*nodes)[n].weight_vector();
                            (*weights)[k] = std::stod(val);
                            k++;

                            if(k == (*weights).size()) { n++; k = 0; }
                        }
                    }
                    val = "";
                }
            }
        }
        loaded = true;
        f.close();
    }
    return loaded;
}

