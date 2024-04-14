#pragma once
#include "Place.h"
#include <algorithm>
#include <iostream>

struct Node {
	Place place;
	Node* next;

	// 构造函数
	Node(Place place) :place(place), next(nullptr) {}
	Node() {}
};

class List {
private:
	int num;// 景点数量
	Node* head; // 链表头指针
	Node* tail;
public:
	List();
	~List();
	void AddPlace(const Place& place);// 添加一个新的地点
	void DeletePlace(int id);// 删除一个地点
	Place* SearchById(int id);// 根据ID搜索地点,返回指向该对象的指针。
	Place* SearchByName(const std::string& name);// 根据名称搜索地点,返回指向该对象的指针。

	int getNum() { return num; }// 获取景点数量
	Node* GetHeadNode();// 获取所有地点的信息
};