#include "calibra/LinearModel.hpp"
#include <vector>
#include <iostream>

int main()
{
    std::vector<double> X = {4.0, 5.2};
    std::vector<double> params = {2.0, 2.3, 3.3};

    LinearModel model{2};

    std::vector<double> res = model.evaluate(X, params);
    int size = res.size();

    if(size == 1)
    {
        if(res[0] == model.evaluate(X, params)[0]) 
        {
            std::cout << res[0] << std::endl;
        }
    }

    return 0;
}