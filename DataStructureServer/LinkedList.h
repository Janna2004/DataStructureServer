#pragma once
#include"Place.h"

struct Node {
    Place place;
    Node* next;

    // 构造函数
    Node(Place place) :place(place), next(nullptr){}
    Node(){}
};

class LinkedList {
private:
    Node* head; // 链表头指针

public:
    // 构造函数
    LinkedList();

    // 析构函数，释放链表中的内存
    ~LinkedList();

    // 在链表尾部添加节点
    void insert(Place place);

    // 删除指定值的第一个节点
    void remove(int id);

    //根据id查询place
    Place* SearchById(int id);

    //根据name查询place
    Place* SearchByName(const std::string& name);

    //获取所有place信息
    LinkedList GetInformation();

    
};

