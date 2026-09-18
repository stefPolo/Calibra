#include <cstddef>
#include <vector>

class IModel {
    public:

        virtual double evaluate(

            const std::vector<double>& x,
            const std::vector<double>& parameters

        ) const = 0;

        virtual std::size_t input_dimension() const = 0;
        virtual std::size_t parameter_dimension() const = 0;

        virtual ~IModel() = default;
};