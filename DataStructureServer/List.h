#pragma once
#include "Place.h"
#include <vector>
#include <algorithm>
#include <iostream>

class List {
private:
    int num;// 景点数量
    std::vector<Place> places;//线性表存储，这里是一个示例，需要手搓一个数据结构
public:
    List() {};// 构造函数，用于在main函数中初始化一个线性表
    void AddPlace(const Place& place);// 添加一个新的地点
    void DeletePlace(int id);// 删除一个地点
    Place* SearchById(int id);// 根据ID搜索地点，可以用一个快速的搜索办法,返回指向该对象的指针。
    Place* SearchByName(const std::string& name);// 根据名称搜索地点,返回指向该对象的指针。

    int getNum() { return num; }// 获取景点数量
    std::vector<Place> GetInformation();// 获取所有地点的信息
};