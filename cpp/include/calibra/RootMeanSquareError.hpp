#pragma once
#include "Calibra/MeanSquareError.hpp"
#include <vector>

class RootMeanSquaredError : public MeanSquaredError
{
    public:
        double evaluate(
            const std::vector<double>& predictions,
            const std::vector<double>& labels
        ) const override;
};