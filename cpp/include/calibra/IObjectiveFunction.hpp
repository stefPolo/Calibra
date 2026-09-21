#pragma once
#include <vector>

class IObjectiveFunction {

    public:
        virtual double evaluate(

            const std::vector<double>& predictions,
            const std::vector<double>& labels 
            
        ) const = 0; 
        
        virtual ~IObjectiveFunction() = default;
};