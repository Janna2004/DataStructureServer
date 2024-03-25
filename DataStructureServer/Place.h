#pragma once
#include "Point.h"
#include <string>

class Place {
public:
    int id;
    double x, y;
    std::string name;
    std::string description;

    // ʹ�û���Ĺ��캯����ʼ�������Ա��������µĳ�Ա������ʼ��
    Place(int id, std::string name, double x, double y, std::string description)
        : id(id), name(name), x(x), y(y), description(description) {};
};