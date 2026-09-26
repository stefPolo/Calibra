#include "calibra/IOptimiser.hpp"
#include "calibra/CalibrationProblem.hpp"
#include <stdexcept>

enum class Algorithm
{
    COBYLA,
    BOBYQA,
    NelderMead
    // ...
};

enum class StoppingCriterion
{
    RelativeFuncTolerance,
    AbsoluteFuncTolerance,
    RelativeParamTolerance,
    AbsoluteParamTolerance
};

class NLoptOptimiser : public IOptimiser
{
    public:
        NLoptOptimiser(
            double tolerance_value,
            int max_evaluations, 
            Algorithm algorithm = Algorithm::BOBYQA,
            StoppingCriterion stopping_criterion = StoppingCriterion::RelativeFuncTolerance 
        )
            : tolerance_value_(tolerance_value)
            , max_evaluations_(max_evaluations)
            , algorithm_(algorithm)
            , stopping_criterion_(stopping_criterion)
        {
            if (max_evaluations < 0)
            {
                throw std::invalid_argument(
                    "Maximum number of evaluations has to be > 0."
                );
            }
            if (tolerance_value < 0)
            {
                throw std::invalid_argument(
                    "Tolerance value has to be > 0."
                );
            }
        }

        OptimisationResult optimise(
            const CalibrationProblem& problem
        ) const override;
    
    private:
        Algorithm algorithm_;
        StoppingCriterion stopping_criterion_;
        double tolerance_value_;
        int max_evaluations_;
};