#include "calibra/ParameterSpace.hpp"
#include <vector>
#include <cmath>
#include <stdexcept>

ParameterSpace::ParameterSpace(
    std::vector<double>& lower_bounds, 
    std::vector<double>& upper_bounds
)
    : lower_bounds_{lower_bounds},
      upper_bounds_{upper_bounds}
{
    // Vectors of lower and upper bounds have to match to form valid intervals for each dimension.
    if( lower_bounds.size() != upper_bounds.size() )
    {
        throw std::invalid_argument(
            "Dimensions of chosen lower and upper bounds must match."
        );
    }

    // At least one dimension.
    if( lower_bounds.empty() ) 
    {
        throw std::invalid_argument(
            "Admissible parameter space cannot be of dimension zero."
        );
    }           

    // Assure that the bounds are real numbers
    for( std::size_t i = 0; i < lower_bounds.size(); i++ )
    {
        if( (!std::isfinite(lower_bounds[i])) || (!std::isfinite(upper_bounds[i])) )
        {
            throw std::invalid_argument(
                "Invalid bounds."
            );
        }

        // Intervals must have non-negative length.
        if( lower_bounds[i] > upper_bounds[i] )
        {
            throw std::invalid_argument(
                "Lower bounds cannot be strictly greater than upper bounds."
            );
        }
    }
}

const std::vector<double>& ParameterSpace::lower_bounds() const
{
    return lower_bounds_;
}

const std::vector<double>& ParameterSpace::upper_bounds() const
{
    return upper_bounds_;
}

std::size_t ParameterSpace::dimension() const
{
    return lower_bounds_.size();
}

bool ParameterSpace::contains(
    const std::vector<double>& params
) const
{
    // Check if parameter vector and instantiated parameter space domensions match.
    if( params.size() != dimension() )
    {
        return false;
    }

    // Each parameter has to be a member of the parameter space and hence be finite.
    for( std::size_t i = 0; i < params.size(); ++i )
    {
        if (
            (params[i] < lower_bounds_[i]) ||
            (params[i] > upper_bounds_[i])           
        )
        {
            return false;
        }

        if (!std::isfinite(params[i]))
        {
            return false;
        }
    }

    return true;
}