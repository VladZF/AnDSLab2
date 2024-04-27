#ifndef TREEALGORITHM_H
#define TREEALGORITHM_H
#include "Structs.h"
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

namespace TreeAlgorithm {
    std::vector<int> xValues;
    std::vector<int> yValues;

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

    Node* buildTree(std::vector<int>& array, int begin, int end) {
        if (begin >= end)
            return new Node(array[begin], nullptr, nullptr, begin, end);
        int mid = (begin + end) / 2;
        Node* left = buildTree(array, begin, mid);
        Node* right = buildTree(array, mid + 1, end);
        return new Node(left->value + right->value, left, right, left->leftIndex, right->rightIndex);
    }

    Node* add(Node* root, int begin, int end, int value) {
        if (begin <= root->leftIndex && root->rightIndex <= end)
            return new Node(root->value + value, root->left, root->right, root->leftIndex, root->rightIndex);
        if (root->rightIndex < begin || end < root->leftIndex)
            return root;
        auto node = new Node(root->value, root->left, root->right, root->leftIndex, root->rightIndex);
        node->left = add(node->left, begin, end, value);
        node->right = add(node->right, begin, end, value);
        return node;
    }

    std::vector<Node*> getFullTree(std::vector<Rectangle>& rectangles) {
        if (rectangles.size() == 0)
            return {};
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

        std::vector<Event> events(2 * rectangles.size());
        int ptr = 0;
        for (auto rectangle : rectangles) {
            events[ptr++] = Event(xIndex[rectangle.start.x], yIndex[rectangle.start.y], yIndex[rectangle.end.y] - 1, 1);
            events[ptr++] = Event(xIndex[rectangle.end.x], yIndex[rectangle.start.y], yIndex[rectangle.end.y] - 1, -1);
        }
        std::sort(events.begin(), events.end(), [](Event& a, Event& b) { return a.x < b.x; });
        std::vector<int> values(yValues.size());
        auto roots = std::vector<Node*>(2 * rectangles.size() + 1);
        auto root = buildTree(values, 0, yValues.size() - 1);
        ptr = 0;
        int lastX = events[0].x;
        for (auto event : events) {
            if (event.x != lastX) {
                roots[ptr++] = root;
                lastX = event.x;
            }
            root = add(root, event.startY, event.endY, event.state);
        }
        return roots;
    }

    int answerForPoint(Node* root, int index) {
        if (!root)
            return 0;
        int mid = (root->leftIndex + root->rightIndex) / 2;
        int value;
        if (index <= mid)
            value = answerForPoint(root->left, index);
        else
            value = answerForPoint(root->right, index);
        return value + root->value;
    }

    std::vector<int> treeAnswer(std::vector<Node*>& roots, std::vector<Point>& points) {
        std::vector<int> answer(points.size());
        int ptr = 0;
        for (auto point : points) {
            int xPos = binarySearch(xValues, point.x);
            int yPos = binarySearch(yValues, point.y);
            if (xPos == -1 || yPos == -1) {
                ptr++;
                continue;
            }
            answer[ptr++] = answerForPoint(roots[xPos], yPos);
        }
        return answer;
    }
}

#endif //TREEALGORITHM_H
