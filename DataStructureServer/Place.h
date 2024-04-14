#pragma once
#include "Point.h"
#include <string>

class Place {
public:
    int id;
    double x, y;
    std::string name;
    std::string description;

    // 使用基类的构造函数初始化基类成员，仅添加新的成员变量初始化
    Place(int id, std::string name, double x, double y, std::string description)
        : id(id), name(name), x(x), y(y), description(description) {};
    Place(){}
};