#ifndef MAPCOMPRESSION_H
#define MAPCOMPRESSION_H
#include <vector>
#include <set>
#include <unordered_map>
#include "Structs.h"

namespace MapCompression {
    std::vector<int> xValues;
    std::vector<int> yValues;
    using Map = std::vector<std::vector<int>>;

    int binarySearch(std::vector<int> &array, int target) {
        int left = 0, right = array.size() - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (array[mid] == target)
                return mid;
            if (target < array[mid])
                right = mid - 1;
            else
                left = mid + 1;
        }
        return right;
    }

    Map getMap(std::vector<Rectangle> &rectangles) {
        std::set<int> xSet, ySet;
        std::unordered_map<int, int> xIndex, yIndex;
        for (auto rectangle: rectangles) {
            xSet.insert(rectangle.start.x);
            xSet.insert(rectangle.end.x);
            ySet.insert(rectangle.start.y);
            ySet.insert(rectangle.end.y);
        }
        xValues = std::vector<int>(xSet.begin(), xSet.end());
        yValues = std::vector<int>(ySet.begin(), ySet.end());
        auto map = std::vector<std::vector<int>>(yValues.size(), std::vector<int>(xValues.size()));
        for (int i = 0; i < xValues.size(); ++i)
            xIndex[xValues[i]] = i;
        for (int i = 0; i < yValues.size(); ++i)
            yIndex[yValues[i]] = i;
        for (auto rectangle: rectangles) {
            int sx = xIndex[rectangle.start.x];
            int sy = yIndex[rectangle.start.y];
            int ex = xIndex[rectangle.end.x];
            int ey = yIndex[rectangle.end.y];
            for (int i = sy; i < ey; ++i)
                for (int j = sx; j < ex; ++j)
                    map[i][j]++;
        }
        return map;
    }

    std::vector<int> mapAnswer(Map &map, std::vector<Point> &points) {
        std::vector<int> answer;
        answer.reserve(points.size());
        for (auto point: points) {
            int yPos = binarySearch(yValues, point.y);
            int xPos = binarySearch(xValues, point.x);
            answer.push_back(map[yPos][xPos]);
        }
        return answer;
    }
}

#endif //MAPCOMPRESSION_H
