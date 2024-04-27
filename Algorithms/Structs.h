#ifndef STRUCTS_H
#define STRUCTS_H

struct Point {
    int x, y;

    Point(int x, int y) : x(x), y(y) {}
};

struct Rectangle {
    Point start, end;

    Rectangle (Point s, Point e) : start(s), end(e) {}

    bool hasPoint(const Point& point) {
        return start.x <= point.x && point.x < end.x &&
            start.y <= point.y && point.y < end.y;
    }
};

struct Node {
    int value;
    Node* left;
    Node* right;
    int leftIndex;
    int rightIndex;

    Node(int val, Node* l, Node* r, int li, int ri) : value(val), left(l), right(r), leftIndex(li), rightIndex(ri) {}
};

struct Event {
    int x;
    int startY;
    int endY;
    int state;

    Event() = default;

    Event(int x, int sy, int ey, int s) : x(x), startY(sy), endY(ey), state(s) {}
};

#endif //STRUCTS_H
