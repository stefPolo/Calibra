#include "IModel.hpp"
#include <vector>

class CalibrationProblem {
    
    private:
        std::vector<double> initParameters;
        std::vector<std::vector<double>> admissableParameterSpace;

        std::vector<std::vector<double>> calibrationData;
        std::vector<double> observedData;
        
        bool calibrationDirection;
        std::vector<std::vector<double>> constraints;

    public:
        virtual double IModel();
        virtual double describeProblem() const = 0;

        virtual ~CalibrationProblem() = default;
};