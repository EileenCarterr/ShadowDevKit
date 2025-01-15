#ifndef FUZZY_SHAPES_H
#define FUZZY_SHAPES_H

#include <stdexcept>
#include <algorithm>
#include "Centroid.h"

// Base struct for all fuzzy shapes
struct FuzzyShape {
    virtual float GetMembership(float x) const = 0;
	virtual Centroid GetCentroid(float y) const = 0;
    virtual ~FuzzyShape() = default;
};

struct TrapezoidShape : public FuzzyShape {
    float a, b, c, d;

    // Constructor with parameter validation
    TrapezoidShape(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {
        if (!(a <= b && b <= c && c <= d)) {
            throw std::invalid_argument("Invalid parameters for TrapezoidShape: a <= b <= c <= d is required.");
        }
    }

    inline float GetMembership(float x) const override {
        if (x <= a || x >= d) return 0;
        else if (x >= b && x <= c) return 1;
        else if (x > a && x < b) return (x - a) / (b - a); // Rising edge
        else return (d - x) / (d - c); // Falling edge
    }
	
    inline Centroid GetCentroid(float y) const override {
		
        if (y == 0) return Centroid(0, 0);

        std::vector<Centroid> centroids;

        // Left slope centroid
        float leftSlope = 1 / (b - a);
        float leftMax = y / leftSlope + a;
        float leftCenter = (a + leftMax) / 2;
        float leftWeight = (leftMax - a) * y / 2;
        centroids.emplace_back(leftCenter, leftWeight);

        // Right slope centroid
        float rightSlope = 1 / (d - c);
        float rightMin = d - y / rightSlope;
        float rightCenter = (rightMin + d) / 2;
        float rightWeight = (d - rightMin) * y / 2;
        centroids.emplace_back(rightCenter, rightWeight);

        // Center rectangle centroid
        float centerCenter = (leftMax + rightMin) / 2;
        float centerWeight = (rightMin - leftMax) * y;
        centroids.emplace_back(centerCenter, centerWeight);

        // Combine all centroids
        return Centroid::Combine(centroids);
    }
};

struct TriangleShape : public FuzzyShape {
    float a, b, c;

    // Constructor with parameter validation
    TriangleShape(float a, float b, float c) : a(a), b(b), c(c) {
        if (!(a <= b && b <= c)) {
            throw std::invalid_argument("Invalid parameters for TriangleShape: a <= b <= c is required.");
        }
    }

    inline float GetMembership(float x) const override {
        if (x <= a || x >= c) return 0;
        else if (x == b) return 1;
        else if (x > a && x < b) return (x - a) / (b - a); // Rising edge
        else return (c - x) / (c - b); // Falling edge
    }
	
    inline Centroid GetCentroid(float y) const override {
        if (y == 0) return Centroid(0, 0);

        std::vector<Centroid> centroids;

        // Left slope
        float leftSlope = 1 / (b - a);
        float leftMax = y / leftSlope + a;
        float leftCenter = (a + leftMax) / 2;
        float leftWeight = (leftMax - a) * y / 2;
        centroids.emplace_back(leftCenter, leftWeight);

        // Right slope
        float rightSlope = 1 / (c - b);
        float rightMin = c - y / rightSlope;
        float rightCenter = (rightMin + c) / 2;
        float rightWeight = (c - rightMin) * y / 2;
        centroids.emplace_back(rightCenter, rightWeight);

        // Center region (intersection of leftMax and rightMin)
        float centerCenter = (leftMax + rightMin) / 2;
        float centerWeight = (rightMin - leftMax) * y;
        centroids.emplace_back(centerCenter, centerWeight);

        // Combine all centroids
        return Centroid::Combine(centroids);
    }
};

#endif
