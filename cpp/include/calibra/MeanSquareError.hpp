#pragma once
#include "IObjectiveFunction.hpp"
#include <vector>

class MeanSquaredError : public IObjectiveFunction 
{
    public:
        double evaluate(
            const std::vector<double>& predictions,
            const std::vector<double>& labels
        ) const override;
};