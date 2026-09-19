#include <vector>
#include <cmath>
#include <cstddef>
#include <stdexcept>

class ParameterSpace 
{
    public:
        ParameterSpace(
            std::vector<double> lower_bounds,
            std::vector<double> upper_bounds
        )
            : lower_bounds_ {lower_bounds},
              upper_bounds_ {upper_bounds}  
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

            // Assure that the bounds are (-\infty, \infty)
            for( std::size_t i = 0; i < lower_bounds.size(); i++ )
            {
                if( (!std::isfinite(lower_bounds[i])) || (!std::isfinite(upper_bounds[i])) )
                {
                    throw std::invalid_argument(
                        "Invalid bounds."
                    );
                }

                // Intervals must have positive length.
                if( lower_bounds[i] > upper_bounds[i] )
                {
                    throw std::invalid_argument(
                        "Lower bounds cannot be strictly greater than upper bounds."
                    );
                }
            }
        }    

        /*
        Just tracks the dimension of the parameter space.
        */
        std::size_t dimension() const;

        /*
        Method which checks whether the chosen parameter vector is a subset of the parameter space.
        */
        bool contains(
            const std::vector<double>& parameters
        ) const
        {
            // Check if parameter vector and instantiated parameter space domensions match.
            if ( parameters.size() != dimension() )
            {
                throw std::invalid_argument(
                    "Dimension of input parameters must match parameter space dimension."
                );
            }

            // Each parameter has to be a member of the parameter space and hence, implicitly, finite.
            for ( std::size_t i = 0; i < lower_bounds_.size(); i++)
            {
                if( 
                ((parameters[i] < lower_bounds_[i]) || 
                (parameters[i] > upper_bounds_[i])) || 
                (!std::isfinite(parameters[i])) 
            )
                {
                        return false;
                }
            }

            return true;
        }

        const std::vector<double>& lower_bounds() const;
        const std::vector<double>& upper_bounds() const;
    
    private:
        std::vector<double> lower_bounds_;
        std::vector<double> upper_bounds_;
};