#include "calibra/ParameterSpace.hpp"
#include <vector>
#include <iostream>

int main()
{
    std::vector<double> lowerBounds = {1.0, 2.0, 3.0};
    std::vector<double> upperBounds = {2.0, 3.0, 4.0};

    std::vector<double> Params = {2.0, 2.1, std::nan};

    ParameterSpace space {lowerBounds, upperBounds};

    bool test = space.contains(Params);

    std::cout << test << std::endl;

    return 0;
}