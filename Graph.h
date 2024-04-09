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
    Edge* head; // ָ���һ��Edge��ָ��
};
class Graph {
private:
    int num; // �������
    AdjList* array; // �ڽӱ����飬ͼ�Ĵ洢��
public:
    Graph(int vertices) ;// ���캯����������main�����г�ʼ��һ��ͼok
    ~Graph() {};// ���������������ͷ��ڴ�ok
    void AddEdge(Point& start, Point& end, double distance = -1);//ok
    void DeleteEdge(Point& start, Point& end);//ok
    void AddPoint(const Point& a);//ok
    void DeletePoint(const Point& a);//ok
    Point* SearchById(int id);// ����ID�����ص㡣������һ�����ٵ������취,����ָ��ö����ָ�롣ok

    std::pair<int, std::vector<Point>> Navigation(const Point& start, const Point& end);// ���������·���㷨��pair�ĵ�һ������̾��룬�ڶ�����·�������еĵ�Ķ��󣨰��߹���˳��
    std::vector<Point> SearchByRange(const Point& center, int range);// ������������centerΪ���ģ���·������range��Χ�ڵľ��㣨�����򣩣�ʹ��place�ж��Ƿ��Ǿ��㣩
    void SortByDistance(std::vector<Point>& reference);// �����㷨�����㰴����������Ϊ����������ã����Բ���Ҫ���ⷵ��

    int GetNum() const { return num; }// ��ȡ�������
    std::vector<Point> GetInformationByDeepth();// ��ȡ���е����Ϣ,������ȱ���
    std::vector<Point> GetInformationByWidth();// ��ȡ���е����Ϣ,������ȱ���
};
Graph::Graph(int vertices) : num(vertices), array(new AdjList[vertices]) {

    // ��ʼ���ڽ�����
    for (int i = 0; i < vertices; ++i) {
        array[i].point = new Point(); // �����ڴ�
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
        // �����µ��ڽӱ�ڵ�
        AdjList new_adj_list;
        new_adj_list.point = new Point(a);
        new_adj_list.head = nullptr;

        // ���µ��ڽӱ�ڵ���ӵ�������
        array[num] = new_adj_list;
        num++;

    }
}
void Graph::DeletePoint(const Point& a) {
    if (a.id > 0) {
        int index = -1;

        // ����Ҫɾ���ĵ������
        for (int i = 0; i < num; i++) {
            if (array[i].point->id == a.id) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            // ɾ���ڽӱ��еı�
            Edge* current = array[index].head;
            while (current != nullptr) {
                Edge* next = current->next;
                delete current;
                current = next;
            }

            // ɾ���ڽӱ�ڵ�
            delete array[index].point;

            // ������Ľڵ���ǰ�ƶ�һλ
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
            if (i == num - 2) { // �����һ�У��ж��Ƿ񵽴�����ĩβ
                break;
            }
        }
    }
    return nullptr; // ȷ������������¶��з���ֵ
}

