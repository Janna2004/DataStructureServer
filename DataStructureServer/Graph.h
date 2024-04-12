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
    Graph();// ���캯����������main�����г�ʼ��һ��ͼ
    ~Graph();// ���������������ͷ��ڴ�
    void AddEdge(Point& start, Point& end, double distance = -1);
    void DeleteEdge(Point& start, Point& end);
    void AddPoint(const Point& a);
    void DeletePoint(const Point& a);
    Point* SearchById(int id);// ����ID�����ص㡣������һ�����ٵ������취,����ָ��ö����ָ�롣

    std::pair<int, std::vector<Point>> Navigation(const Point& start, const Point& end);// ���������·���㷨��pair�ĵ�һ������̾��룬�ڶ�����·�������еĵ�Ķ��󣨰��߹���˳��
    std::vector<Point> SearchByRange(const Point& center, int range);// ������������centerΪ���ģ���·������range��Χ�ڵľ��㣨�����򣩣�ʹ��place�ж��Ƿ��Ǿ��㣩
    void SortByDistance(std::vector<Point>& reference);// �����㷨�����㰴����������Ϊ����������ã����Բ���Ҫ���ⷵ��

    int GetNum() const { return num; }// ��ȡ�������
    std::vector<Point> GetInformationByDeepth();// ��ȡ���е����Ϣ,������ȱ���
    std::vector<Point> GetInformationByWidth();// ��ȡ���е����Ϣ,������ȱ���
};

