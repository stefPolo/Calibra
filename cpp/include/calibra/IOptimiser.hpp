#pragma once
#include "calibra/CalibrationProblem.hpp"
#include "calibra/OptimisationResult.hpp"

class IOptimiser
{
    public:
        virtual OptimisationResult optimise(
            const CalibrationProblem& problem
        ) const = 0;

        virtual ~IOptimiser() = default;
};