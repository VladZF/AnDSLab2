#include <iostream>
#include <fstream>
#include <chrono>
#include "Naive.h"
#include "MapCompression.h"
#include "TreeAlgorithm.h"

const int launchNumber = 100;
const int pointsCount = 1000;
using steadyClock = std::chrono::steady_clock;
std::vector<Point> points;
std::vector<Rectangle> rectangles;

int modExp(int base, int exp, int mod) {
    unsigned long long answer = 1;
    while (exp > 0) {
        if (exp & 1)
            answer = (answer * base) % mod;
        base = (base * base) & mod;
        exp >>= 1;
    }
    return static_cast<int>(answer);
}

void getPoints() {
    points.reserve(pointsCount);
    for (int i = 0; i < pointsCount; ++i) {
        int x = modExp(9613 * i, 31, pointsCount * 20);
        int y = modExp(5227 * i, 31, pointsCount * 20);
        points.emplace_back(x, y);
    }
}

void getRectangles(int n) {
    rectangles.reserve(n);
for (int i = 0; i < n; ++i) {
        rectangles.emplace_back(Point(10 * i, 10 * i), Point(2 * n - i, 2 * n - i));
    }
}

long long naiveTest() {

    long long time = 0;
    for (int i = 0; i < launchNumber; ++i) {
        auto start = steadyClock::now();
        auto answer = Naive::naiveAlgo(points, rectangles);
        auto finish = steadyClock::now();
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    }
    return time / launchNumber;
}

long long getMapTest() {
    long long time = 0;
    for (int i = 0; i < launchNumber; ++i) {
        auto start = steadyClock::now();
        auto map = MapCompression::getMap(rectangles);
        auto finish = steadyClock::now();
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    }
    return time / launchNumber;
}

long long mapAnswerTest() {
    long long time = 0;
    auto map = MapCompression::getMap(rectangles);
    for (int i = 0; i < launchNumber; ++i) {
        auto start = steadyClock::now();
        auto answer = MapCompression::mapAnswer(map, points);
        auto finish = steadyClock::now();
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    }
    return time / launchNumber;
}

long long getFullTreeTest() {
    long long time = 0;
    for (int i = 0; i < launchNumber; ++i) {
        auto start = steadyClock::now();
        auto roots = TreeAlgorithm::getFullTree(rectangles);
        auto finish = steadyClock::now();
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    }
    return time / launchNumber;
}

long long treeAnswerTest() {
    long long time = 0;
    auto roots = TreeAlgorithm::getFullTree(rectangles);
    for (int i = 0; i < launchNumber; ++i) {
        auto start = steadyClock::now();
        auto answer = TreeAlgorithm::treeAnswer(roots, points);
        auto finish = steadyClock::now();
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    }
    return time / launchNumber;
}

int main() {
    std::ofstream outLog("../outLog.txt");
    getPoints();
    for (int i = 1; i <= 1024; i *= 2) {
        getRectangles(i);
        auto naiveTime = naiveTest();
        auto getMapTime = getMapTest();
        auto mapAnswerTime = mapAnswerTest();
        auto getFullTreeTime = getFullTreeTest();
        auto treeAnswerTime = treeAnswerTest();
        outLog
        << i << " "
        << naiveTime << " "
        << getMapTime << " "
        << mapAnswerTime << " "
        << getFullTreeTime << " "
        << treeAnswerTime << "\n";
        std::cout << i << " ";
        rectangles.clear();
    }
    outLog.close();
}
