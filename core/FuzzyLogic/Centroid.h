#ifndef CENTROID_H
#define CENTROID_H

#include <vector>

struct Centroid {
    float value;
    float weight;

    Centroid(float v = 0, float w = 0) : value(v), weight(w) {}

    // Combine two centroids
    static Centroid Combine(const std::vector<Centroid>& centroids) {
        float totalWeight = 0;
        float weightedValueSum = 0;

        for (const auto& centroid : centroids) {
            totalWeight += centroid.weight;
            weightedValueSum += centroid.value * centroid.weight;
        }

        return totalWeight > 0 ? Centroid(weightedValueSum / totalWeight, totalWeight) : Centroid(0, 0);
    }
};

#endif