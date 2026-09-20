#include "IModel.hpp"
#include "ParameterSpace.hpp"
#include <vector>

class CalibrationProblem {

    public:
        CalibrationProblem(
            IModel& model,
            ParameterSpace& paramSpace,
            std::vector<double> initParameters
        );
    
    private:
        IModel& model_;
        ParameterSpace& paramSpace_;
        std::vector<double> initParameters_;

        std::vector<std::vector<double>> calibrationData_;
        std::vector<std::vector<double>> observedData_;
        
        enum optimisionDirection_ {
            Minimize_,
            Maximize
        };    

};