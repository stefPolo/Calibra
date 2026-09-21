#include "calibra/CalibrationProblem.hpp"
#include "calibra/IModel.hpp"
#include "calibra/MeanSquaredError.hpp"
#include "calibra/LinearModel.hpp"
#include "calibra/ParameterSpace.hpp"
#include <vector>
#include <iostream>

int main()
{
    std::vector<std::vector<double>> X = {
        {2.6, 2.7, 7.2},
        {1.6, 7.7, 3.2},
        {5.6, 5.7, 0.2}
    };

    std::vector<double> labels = {3.2, 2.2, 8.2};

    std::vector<double> lower_bounds = {-6.4, -3.2, -1.2, -5.2};
    std::vector<double> upper_bounds = {12.4, 13.2, 15.2, 77.2};

    ParameterSpace ParamSpace { lower_bounds, upper_bounds };
    std::vector<double> init_params = {1.0, 2.3, 0.2, 0.5};

    LinearModel model { 3 };
    MeanSquaredError mse;

    CalibrationProblem CalibProb {
        model,
        ParamSpace,
        init_params,
        mse,
        X,
        labels, 
        OptimisationDirection::minimise
    };

    double calib_res = CalibProb.evaluate_objective(init_params);

    std::cout << calib_res << std::endl;

    return 0;
}