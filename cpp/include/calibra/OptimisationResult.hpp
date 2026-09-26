#include <vector>
#include <string>
#include <cstddef>

class OptimisationResult
{
    public:
        OptimisationResult(
            std::string optimiser_name,
            std::size_t objective_evaluations,
            std::string termination_status,
            double optimisation_duration,
            std::vector<double> initial_params,
            std::vector<double> final_params,
            double initial_loss,
            double final_loss
        )
            : optimiser_name_(optimiser_name)
            , objective_evaluations_(objective_evaluations)
            , termination_status_(termination_status)
            , optimisation_duration_(optimisation_duration)
            , initial_params_(initial_params)
            , final_params_(final_params)  
            , initial_loss_(initial_loss)
            , final_loss_(final_loss)
        {

        }
        
        const std::string& optimiser_name() const;    
        std::size_t objective_evaluations() const;
        const std::string& termination_status() const;
        double optimisation_duration_seconds() const;
        
        const std::vector<double>& initial_params() const;
        const std::vector<double>& final_params() const;

        double initial_loss() const;
        double final_loss() const;

    private:
        std::string optimiser_name_;
        std::size_t objective_evaluations_;
        std::string termination_status_;
        double optimisation_duration_;
        std::vector<double> initial_params_;
        std::vector<double> final_params_;
        double initial_loss_;
        double final_loss_;
};