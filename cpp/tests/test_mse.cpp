#include "calibra/MeanSquaredError.hpp"
#include <vector>
#include <iostream>

int main()
{
    std::vector<double> X = {1.2, 3.2, 5.5}; 
    std::vector<double> labels = {1.2, 3.2, 5.5};

    MeanSquaredError mse;

    double loss = mse.evaluate(X, labels);

    std::cout << loss << std::endl;

    return 0;
};