#pragma once
#include "IModel.hpp"
#include "ParameterSpace.hpp"
#include "IObjectiveFunction.hpp"
#include <vector>
#include <stdexcept>

enum class OptimisationDirection
{
    minimise, maximise
};

class CalibrationProblem
{

    public:
        CalibrationProblem(
            IModel& model,
            ParameterSpace& paramSpace,
            std::vector<double>& initParameters,
            IObjectiveFunction& objectiveFunction,
            const std::vector<std::vector<double>>& calibrationData,
            const std::vector<double>& labels,
            OptimisationDirection direction = OptimisationDirection::minimise
        ) 
            : model_(model)
            , paramSpace_(paramSpace)
            , initParameters_(initParameters)
            , objectiveFunction_(objectiveFunction)
            , calibrationData_(calibrationData)
            , labels_(labels)
            , direction_(direction)
        {
            /*
            Initialise a CalibrationProblem only if:
            1. Parameter vector is contained by an admissible parameter space.
            2. Dimensions of parameter vector and model parameters must match.
            3. Dimensions of model input must match number of features.
            4. Number of observations must match number of labels. 
            */
            
            // 1.
            if( !paramSpace.contains(initParameters) )
            {
                throw std::invalid_argument(
                    "Dimensions of initialised parameters\n" 
                    "do not match that of chosen parameter space.\n"
                    "Initialised parameter vector has to be contained in a\n"
                    "chosen parameter space."
                );
            }

            // 2.
            if( model.parameter_dimension() != initParameters.size() )
            {
                throw std::invalid_argument(
                    "Initialised parameter vector dimensions\n"
                    "must match model parameter dimensions."
                );
            }

            // 3.
            for( const std::vector<double>& x : calibrationData )
            {
                if( x.size() != model.input_dimension() )
                {
                    throw std::invalid_argument(
                        "Model input dimension must match number of inputs/features."
                    );
                }
            }

            // 4.
            if( calibrationData.size() != labels.size() )
            {
                throw std::invalid_argument(
                    "The number of observations must match the number of labels."
                );
            }
        }

        double evaluate_objective(const std::vector<double>& params) const
        {
            /*
            Check if passed parameters are a member of the initialised parameter space.
            If so, proceed to evaluating the objective function.
            */
            if( !paramSpace_.contains(params) )
            {
                throw std::invalid_argument(
                    "Initialised parameter space does not contain passed parameters."
                );
            }
            
            auto predictions = model_.evaluate(calibrationData_, params);

            double loss = objectiveFunction_.evaluate(predictions, labels_);

            return loss;
        }

    private:
        IModel& model_;
        ParameterSpace& paramSpace_;
        std::vector<double> initParameters_;
        IObjectiveFunction& objectiveFunction_;
        std::vector<std::vector<double>> calibrationData_;
        std::vector<double> labels_;
        OptimisationDirection direction_;
};