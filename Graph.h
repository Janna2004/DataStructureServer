#pragma once
#include "Point.h"
#include <map>
#include <set>
#include <queue>
#include <limits>
#include <cmath>
#include <memory>

struct Edge {
    Point* point;
    double distance;
    Edge* next;
};
struct AdjList {
    Point* point;
    Edge* head; // 指向第一个Edge的指针
};
class Graph {
private:
    int num; // 点的数量
    AdjList* array; // 邻接表数组，图的存储。
public:
    Graph();// 构造函数，用于在main函数中初始化一个图
    ~Graph();// 析构函数，用于释放内存
    void AddEdge(Point& start, Point& end, double distance = -1);
    void DeleteEdge(Point& start, Point& end);
    void AddPoint(const Point& a);
    void DeletePoint(const Point& a);
    Point* SearchById(int id);// 根据ID搜索地点。可以用一个快速的搜索办法,返回指向该对象的指针。

    std::pair<int, std::vector<Point>> Navigation(const Point& start, const Point& end);// 导航：最短路径算法。pair的第一个是最短距离，第二个是路径上所有的点的对象（按走过的顺序）
    std::vector<Point> SearchByRange(const Point& center, int range);// 按搜索所有以center为中心，走路距离在range范围内的景点（不排序）（使用place判断是否是景点）
    void SortByDistance(std::vector<Point>& reference);// 排序算法：景点按距离排序。因为传入的是引用，所以不需要额外返回

    int GetNum() const { return num; }// 获取点的数量
    std::vector<Point> GetInformationByDeepth();// 获取所有点的信息,深度优先遍历
    std::vector<Point> GetInformationByWidth();// 获取所有点的信息,广度优先遍历
};

