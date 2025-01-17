#include <vector>
#include <stdexcept>
#include <numeric>

class UtilityCalculator {
public:
    static float CalculateUtility(const std::vector<float>& factors, const std::vector<float>& weights) {
        if (factors.size() != weights.size()) {
            throw std::invalid_argument("Factors and weights vectors must be of the same length.");
        }

        float weightedSum = 0.0f;
        float weightTotal = 0.0f;

        for (size_t i = 0; i < factors.size(); ++i) {
            weightedSum += factors[i] * weights[i];
            weightTotal += weights[i];
        }

        if (weightTotal == 0.0f) {
            return 0.0f; // Avoid division by zero, return minimum utility.
        }

        return weightedSum / weightTotal; // Return normalized utility score.
    }
};
