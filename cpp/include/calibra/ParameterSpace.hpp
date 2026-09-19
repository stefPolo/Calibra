#include <vector>
#include <cmath>
#include <cstddef>
#include <stdexcept>

class ParameterSpace 
{
    public:
        ParameterSpace(
            std::vector<double>& lower_bounds,
            std::vector<double>& upper_bounds
        );
        
        /*
        Just tracks the dimension of the parameter space.
        */
        std::size_t dimension() const;

        /*
        Method which checks whether the chosen parameter vector is a subset of the parameter space.
        */
        bool contains(
            const std::vector<double>& parameters
        ) const;

        const std::vector<double>& lower_bounds() const;
        const std::vector<double>& upper_bounds() const;
    
    private:
        std::vector<double>& lower_bounds_;
        std::vector<double>& upper_bounds_;
};