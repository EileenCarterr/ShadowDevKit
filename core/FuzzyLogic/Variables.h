#ifndef FUZZY_VAR_H
#define FUZZY_VAR_H

#include <iostream>
#include "Shapes.h"

struct Variable {
    TrapezoidShape low;
    TriangleShape medium;
    TrapezoidShape high;

    // Membership degrees for each fuzzy set
    float lowDegree = 0.0f;
    float mediumDegree = 0.0f;
    float highDegree = 0.0f;

    // Constructor
    Variable(const TrapezoidShape& lowShape, const TriangleShape& mediumShape, const TrapezoidShape& highShape)
        : low(lowShape), medium(mediumShape), high(highShape) {}

    // Evaluate the membership degrees based on input x
    void Evaluate(float x) {
        lowDegree    = low.GetMembership(x);
        mediumDegree = medium.GetMembership(x);
        highDegree   = high.GetMembership(x);
    }

    // Helper functions to check if the variable is primarily Low, Medium, or High
    bool IsLow() const {
        return lowDegree >= mediumDegree && lowDegree >= highDegree;
    }

    bool IsMedium() const {
        return mediumDegree >= lowDegree && mediumDegree >= highDegree;
    }

    bool IsHigh() const {
        return highDegree >= lowDegree && highDegree >= mediumDegree;
    }

	float GetCrispValue() const {

		Centroid lowCentroid = low.GetCentroid(lowDegree);
		Centroid mediumCentroid = medium.GetCentroid(mediumDegree);
		Centroid highCentroid = high.GetCentroid(highDegree);

		Centroid combinedCentroid = Centroid::Combine({lowCentroid, mediumCentroid, highCentroid});

		return combinedCentroid.value;
	}

    // Debugging utility
    void debugPrint() const {
        std::cout << "Low Degree: " << lowDegree
                  << ", Medium Degree: " << mediumDegree
                  << ", High Degree: "   << highDegree
                  << ", Crisp Value: "   << GetCrispValue() << std::endl;
    }
};

#endif


