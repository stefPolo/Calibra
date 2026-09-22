#include "calibra/MeanSquaredError.hpp"
#include <vector>
#include <stdexcept>
#include <cmath>

double MeanSquaredError::evaluate(
    const std::vector<double>& predictions,
    const std::vector<double>& labels
) const
{
    std::size_t n = labels.size();
    double se = 0.0;

    // Assure non-empty vectors
    if( n == 0 || predictions.empty() )
    {
        throw std::invalid_argument(
            "Predictions and labels must be non-empty."
        );
    }

    // Assure equality of prediction and label vectors
    if( n != predictions.size() )
    {
        throw std::invalid_argument(
            "Predictions and labels must be of equal dimensions."
        );
    }

    // Predictions and labels must be real numbers. If they are, comoute the squared errors.
    for (std::size_t i = 0; i < n; ++i)
    {
        if (!std::isfinite(predictions[i]) || !std::isfinite(labels[i]))
        {
            throw std::invalid_argument(
                "Predictions and labels must be real numbers."
            );
        }

        se += (predictions[i] - labels[i]) * (predictions[i] - labels[i]);
    }

    // Return the MSE
    return se / static_cast<double>(n);
};