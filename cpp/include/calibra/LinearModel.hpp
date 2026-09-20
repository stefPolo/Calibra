#include "calibra/IModel.hpp"

class LinearModel : public IModel {

    public:
        explicit LinearModel(std::size_t input_dimension)
            : input_dimension_ {input_dimension}
        {           
        }    
        
        std::vector<double> evaluate(
            const std::vector<double>& x,
            const std::vector<double>& parameters
        ) const override;

        std::size_t input_dimension() const override;
        std::size_t parameter_dimension() const override;
        std::size_t output_dimension() const override;

        private:
            std::size_t input_dimension_ {};
};