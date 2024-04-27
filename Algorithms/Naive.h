#ifndef NAIVE_H
#define NAIVE_H
#include <vector>
#include "Structs.h"

namespace Naive {
    inline std::vector<int> naiveAlgo(const std::vector<Point> &points, const std::vector<Rectangle> &rectangles) {
        std::vector<int> answer(points.size());
        for (auto rectangle: rectangles) {
            for (int i = 0; i < points.size(); ++i)
                if (rectangle.hasPoint(points[i]))
                    answer[i]++;
        }
        return answer;
    }
}


#endif //NAIVE_H
