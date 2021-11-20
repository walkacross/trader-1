
#ifndef __LINEAR_HPP_
#define __LINEAR_HPP_

#include <cstdlib>
#include <vector>

class LinearRegression
{
private:
    double slope;
    double bias;
    double r;
public:
    LinearRegression() {}
    std::vector<double> model();
    double fit(std::vector<double> &x, std::vector<double> &y);
    double predict(double x);
};

#endif
