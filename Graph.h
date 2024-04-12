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
    Graph(int vertices) ;// 构造函数，用于在main函数中初始化一个图ok
    ~Graph() {};// 析构函数，用于释放内存ok
    void AddEdge(Point& start, Point& end, double distance = -1);//ok
    void DeleteEdge(Point& start, Point& end);//ok
    void AddPoint(const Point& a);//ok
    void DeletePoint(const Point& a);//ok
    Point* SearchById(int id);// 根据ID搜索地点。可以用一个快速的搜索办法,返回指向该对象的指针。ok

    std::pair<int, std::vector<Point>> Navigation(const Point& start, const Point& end);// 导航：最短路径算法。pair的第一个是最短距离，第二个是路径上所有的点的对象（按走过的顺序）
    std::vector<Point> SearchByRange(const Point& center, int range);// 按搜索所有以center为中心，走路距离在range范围内的景点（不排序）（使用place判断是否是景点）
    void SortByDistance(std::vector<Point>& reference);// 排序算法：景点按距离排序。因为传入的是引用，所以不需要额外返回

    int GetNum() const { return num; }// 获取点的数量
    std::vector<Point> GetInformationByDeepth();// 获取所有点的信息,深度优先遍历
    std::vector<Point> GetInformationByWidth();// 获取所有点的信息,广度优先遍历
};
Graph::Graph(int vertices) : num(vertices), array(new AdjList[vertices]) {

    // 初始化邻接链表
    for (int i = 0; i < vertices; ++i) {
        array[i].point = new Point(); // 分配内存
        array[i].head = nullptr;

    }

}

void Graph::AddEdge(Point& start, Point& end, double distance ) {
    if (start.id >= 0 && end.id >= 0) {
        Edge* new_edge = new Edge;
        if (new_edge == NULL) {
            return;
        }
        else {
            new_edge->point = &end;
            new_edge->distance = distance;
            new_edge->next = array[start.id].head;
            array[start.id].head = new_edge;
        }
    }
    else {
        return;
    }
}
void Graph::DeleteEdge(Point& start, Point& end) {
    if (start.id >= 0 && end.id >= 0) {
        Edge* current = array[start.id].head;
        Edge* prev = nullptr;

        while (current != nullptr) {
            if (current->point->id == end.id) {
                if (prev == nullptr) {
                    array[start.id].head = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }
}
void Graph::AddPoint(const Point& a) {
    if (a.id > 0) {
        // 创建新的邻接表节点
        AdjList new_adj_list;
        new_adj_list.point = new Point(a);
        new_adj_list.head = nullptr;

        // 将新的邻接表节点添加到数组中
        array[num] = new_adj_list;
        num++;

    }
}
void Graph::DeletePoint(const Point& a) {
    if (a.id > 0) {
        int index = -1;

        // 查找要删除的点的索引
        for (int i = 0; i < num; i++) {
            if (array[i].point->id == a.id) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            // 删除邻接表中的边
            Edge* current = array[index].head;
            while (current != nullptr) {
                Edge* next = current->next;
                delete current;
                current = next;
            }

            // 删除邻接表节点
            delete array[index].point;

            // 将后面的节点向前移动一位
            for (int i = index; i < num - 1; i++) {
                array[i] = array[i + 1];
            }

            num--;
        }
    }
}
Point* Graph::SearchById(int id) {
    if (id >= 0) {
        for (int i = 0; i < num - 1; i++) {
            if (array[i].point->id == id) {
                return array[i].point;
            }
            if (i == num - 2) { // 添加这一行，判断是否到达数组末尾
                break;
            }
        }
    }
    return nullptr; // 确保在所有情况下都有返回值
}

