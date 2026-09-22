#pragma once
#include <vector>
#include <cstddef>
#include <stdexcept>

class ParameterSpace 
{
    public:
        ParameterSpace(
            std::vector<double>& lower_bounds,
            std::vector<double>& upper_bounds
        );
        
        std::size_t dimension() const;

        bool contains(
            const std::vector<double>& parameters
        ) const;

        const std::vector<double>& lower_bounds() const;
        const std::vector<double>& upper_bounds() const;
    
    private:
        std::vector<double>& lower_bounds_;
        std::vector<double>& upper_bounds_;
};