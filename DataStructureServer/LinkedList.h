#pragma once
#include"Place.h"

struct Node {
    Place place;
    Node* next;

    // ���캯��
    Node(Place place) :place(place), next(nullptr){}
    Node(){}
};

class LinkedList {
private:
    Node* head; // ����ͷָ��

public:
    // ���캯��
    LinkedList();

    // �����������ͷ������е��ڴ�
    ~LinkedList();

    // ������β����ӽڵ�
    void insert(Place place);

    // ɾ��ָ��ֵ�ĵ�һ���ڵ�
    void remove(int id);

    //����id��ѯplace
    Place* SearchById(int id);

    //����name��ѯplace
    Place* SearchByName(const std::string& name);

    //��ȡ����place��Ϣ
    LinkedList GetInformation();

    
};

