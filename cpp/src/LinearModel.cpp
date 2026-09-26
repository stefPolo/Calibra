#include "calibra/LinearModel.hpp"
#include <vector>
#include <stdexcept>

std::size_t LinearModel::input_dimension() const
{
    return input_dimension_;
}

std::size_t LinearModel::parameter_dimension() const
{
    return input_dimension_ + 1;
}

std::size_t LinearModel::output_dimension() const
{
    return 1;
}

std::vector<double> LinearModel::evaluate(
    const std::vector<std::vector<double>>& X, 
    const std::vector<double>& params
) const
{
    if (X.empty())
    {
        throw std::invalid_argument(
            "Input data must not be empty."
        );
    }
    
    for (std::size_t i = 0; i < X.size(); ++i)
    {
        if (X[i].size() != input_dimension_)
        {
            throw std::invalid_argument( 
                "Input dimension do not match specified dimension" 
            );
        }
    }

    if (params.size() != parameter_dimension())
    {
        throw std::invalid_argument( 
            "Parameter dimension do not match specified dimension" 
        );
    }

    std::size_t n = X.size();
    std::vector<double> predictions(n);

    for (std::size_t i = 0; i < n; ++i)
    {
        double prediction = params[0];

        for (std::size_t j = 0; j < input_dimension_; ++j)
        {
            prediction += X[i][j] * params[j+1];           
        }

        predictions[i] = prediction;
    }
    
    return predictions;
}                                 