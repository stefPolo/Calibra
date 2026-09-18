#include "Calibra/LinearModel.hpp"
#include <vector>
#include <numeric>
#include <stdexcept>

std::size_t LinearModel::input_dimension() const
{
    return input_dimension_;
}

std::size_t LinearModel::parameter_dimension() const
{
    return input_dimension_ + 1;
}

double LinearModel::evaluate(
    const std::vector<double>& X, 
    const std::vector<double>& params
) const
{
    if (X.size() != input_dimension_)
    {
        throw std::invalid_argument( 
            "Input dimension do not match specified dimension" 
        );
    }
    
    if (params.size() != parameter_dimension())
    {
        throw std::invalid_argument( 
            "Parameter dimension do not match specified dimension" 
        );
    }

    return params[0] 
        + std::inner_product(
            X.begin(), 
            X.end(), 
            params.begin() + 1, 
            0.0);
}                                  