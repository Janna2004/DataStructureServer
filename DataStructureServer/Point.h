#pragma once
class Point {
public:
    int id;
    int placeId = -1; //�����place���������Ӧ��place�е�id��������ǣ�Ĭ��Ϊ-1
    double x, y;

    Point(int id, double x, double y,int placeId)//��������������
        : id(id), x(x), y(y), placeId(placeId) {}

    Point(int id, double x, double y)//��ͨ������������
        : id(id), x(x), y(y) {}
};
