#if !defined(BEZIER_FUNCTIONS_H)
#define BEZIER_FUNCTIONS_H

#include "Vertex.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace MathUtils
{
    // Combinatorial Function
    //   (n)
    //   (i)
    unsigned combFunc(unsigned n, unsigned i)
    {
        if (i > n)
            return 0;
        if (i * 2 > n)
            i = n - i;
        if (i == 0)
            return 1;

        int retval = n;
        for (int k = 2; k <= i; ++k)
        {
            retval *= (n - k + 1);
            retval /= k;
        }
        return retval;
    }

    // generate Bernstein polynomials?
    // TODO -> Finish this
    inline float calculateBernsteinPolynomial(float t, int upperIndex, int lowerIndex)
    {
        if (lowerIndex < 0 || lowerIndex > upperIndex)
            return 0.0;

        float retval = 0.0;
        retval = combFunc(upperIndex, lowerIndex) * std::pow(t, lowerIndex) * std::pow((1 - t), (upperIndex - lowerIndex));

        return retval;
    }

    inline std::size_t getTransitionPointsCount(std::size_t controlPointsCount)
    {
        if (controlPointsCount == 0)
            return 0;
        return controlPointsCount + getTransitionPointsCount(--controlPointsCount);
    }

    inline std::vector<glm::vec2> getBezierTransitionPoints(const std::vector<Vertex> &controlPoints, const float t)
    {
        if (controlPoints.empty())
            return std::vector<glm::vec2>();
        std::vector<glm::vec2> glmVec(controlPoints.begin(), controlPoints.end());

        std::vector<glm::vec2> retval;
        retval.reserve(getTransitionPointsCount(glmVec.size()));
        retval.insert(retval.end(), glmVec.begin(), glmVec.end());

        std::vector<glm::vec2> previousLevel = glmVec;
        std::vector<glm::vec2> nextLevel;

        while (previousLevel.size() > 1)
        {
            nextLevel.clear();
            for (std::size_t i = 1; i < previousLevel.size(); ++i)
            {
                glm::vec2 current = (1 - t) * previousLevel[i - 1] + t * previousLevel[i];
                nextLevel.push_back(current);
                retval.push_back(current);
            }
            previousLevel = nextLevel;
        }
        return retval;
    }

    glm::vec2 evaluateBezier(const std::vector<glm::vec2> &controlPoints, float t)
    {
        std::vector<glm::vec2> points = controlPoints;
        while (points.size() > 1)
        {
            std::vector<glm::vec2> nextPoints;
            for (std::size_t i = 1; i < points.size(); ++i)
            {
                glm::vec2 p = (1 - t) * points[i - 1] + t * points[i];
                nextPoints.push_back(p);
            }
            points = nextPoints;
        }
        return points[0]; // Return the final point, which is on the curve at t
    }

}

#endif // BEZIER_FUNCTIONS_H
