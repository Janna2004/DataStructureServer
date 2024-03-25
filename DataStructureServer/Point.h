#pragma once
class Point {
public:
    int id;
    int placeId = -1; //如果是place，这里填对应的place中的id，如果不是，默认为-1
    double x, y;

    Point(int id, double x, double y,int placeId)//景点调用这个构造
        : id(id), x(x), y(y), placeId(placeId) {}

    Point(int id, double x, double y)//普通点调用这个构造
        : id(id), x(x), y(y) {}
};
