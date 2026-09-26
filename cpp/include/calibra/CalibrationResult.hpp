#include "calibra/CalibrationProblem.hpp"

class CalibrationResult
{
    public:
        CalibrationResult(
            CalibrationProblem& calibrationProblem
        )
            : calibrationProblem_(calibrationProblem)
        {
        }
        
        double identifiability() const;
        double uncertainty() const;
        double stability() const;
        double globality() const;

        bool model_adequacy() const;
        bool model_is_adequate() const;
        
    private:
        CalibrationProblem& calibrationProblem_;
};