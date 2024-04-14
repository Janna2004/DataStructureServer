#include "List.h"

//构造函数
List::List() : num(0), head(new Node(Place())), tail(head) {}

//析构函数
List::~List() {
	Node* current = head;
	while (current != nullptr) {
		Node* temp = current;
		current = current->next;
		delete temp;
	}
}

//添加地点
void List::AddPlace(const Place& place) {
	num++;
	Node* newNode = new Node(place);
	tail->next = newNode;
	tail = newNode;
}

//删除地点byID
void List::DeletePlace(int id) {
	Node* current = head;
	while (current->next != nullptr) {
		if (current->next->place.id == id) {
			Node* temp = current->next;
			current->next = temp->next;
			delete temp;
			if (temp == tail) { // 如果删除的是尾节点，更新尾指针
				tail = current;
			}
			num--;
			return;
		}
		current = current->next;
	}
}

//搜索地点byID
Place* List::SearchById(int id) {
	Node* current = head->next;  // 跳过头节点
	while (current != nullptr) {
		if (current->place.id == id) {
			return &current->place;
		}
		current = current->next;
	}
	return nullptr;  // 没有找到
}

//搜索地点byName
Place* List::SearchByName(const std::string& name) {
	Node* current = head->next;  // 跳过头节点
	while (current != nullptr) {
		if (current->place.name == name) {
			return &current->place;
		}
		current = current->next;
	}
	return nullptr;  // 没有找到
}

//获取所有place信息
Node* List::GetHeadNode() {
	return head->next; // 直接返回 head->next，无论它是不是 nullptr
}

