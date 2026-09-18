#include "calibra/LinearModel.hpp"
#include <vector>
#include <iostream>

int main()
{
    std::vector<double> X = {4.0, 5.2};
    std::vector<double> params = {2.0, 2.3, 3.3};

    LinearModel model{2};

    std::cout << model.evaluate(X, params) << std::endl;
    return 0;
}