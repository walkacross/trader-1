
#include <cstdlib>
#include <vector>
#include <cmath>

#include "../lib/linear.hpp"

std::vector<double> LinearRegression::model() {
    return std::vector<double>({slope, bias});
}

double LinearRegression::fit(std::vector<double> &x, std::vector<double> &y) {
    // compute x_mean and y_mean
    double x_mean = 0.00;
    double y_mean = 0.00;
    for(unsigned int i = 0; i < x.size(); i++) {
        x_mean += x[i];
        y_mean += y[i];
    }
    x_mean /= x.size();
    y_mean /= y.size();
    // compute linear slope and bias term
    double delta_x, delta_y = 0.00;
    for(unsigned int i = 0; i < x.size(); i++) {
        delta_y += x[i] * (y[i] - y_mean);
        delta_x += x[i] * (x[i] - x_mean);
    }
    slope = delta_y / delta_x;
    bias = y_mean - slope * x_mean;
    // compute mean squared error
    double mse = 0.00;
    for(unsigned int i = 0; i < x.size(); i++) {
        double yhat = slope * x[i] + bias;
        double residual_squared = pow(y[i] - yhat, 2);
        mse += residual_squared;
    }
    mse /= x.size();
    // compute covariance of x and y
    double cov = 0.00;
    for(unsigned int i = 0; i < x.size(); i++) {
        cov += (x[i] - x_mean) * (y[i] - y_mean);
    }
    cov /= x.size();
    // compute standard deviation of x and y
    double x_var, y_var = 0.00;
    for(unsigned int i = 0; i < x.size(); i++) {
        x_var += pow(x[i] - x_mean, 2);
        y_var += pow(y[i] - y_mean, 2);
    }
    double x_stdev = sqrt(x_var / x.size());
    double y_stdev = sqrt(y_var / y.size());
    // pearson correlation coefficient
    r = cov / (x_stdev * y_stdev);
    return r;
}

double LinearRegression::predict(double x) {
    return slope * x + bias;
}

