#include "LinkedList.h"

// 构造函数
LinkedList::LinkedList() : head(new Node(Place())) {} // 创建一个空的头节点


// 析构函数，释放链表中的内存
LinkedList::~LinkedList() {
	Node* current = head;
	while (current != nullptr) {
		Node* temp = current;
		current = current->next;
		delete temp;
	}
}

// 在链表尾部添加节点
void LinkedList::insert(Place place) {
	Node* newNode = new Node(place);
	if (head->next == nullptr) {
		head->next = newNode;
	}
	else {
		Node* current = head;
		while (current->next != nullptr) {
			current = current->next;
		}
		current->next = newNode;
		delete current;
	}

}

// 删除节点
void LinkedList::remove(int id) {
	if (head->next == nullptr) {
		return;
	}
	else {
		Node* current = head;
		while (current->next != nullptr) {
			if (current->next->place.id == id) {
				Node* temp = current->next;
				current->next = current->next->next;
				delete temp;
				delete current;
				return;
			}
			current = current->next;
		}
	}

}

Place* LinkedList::SearchById(int id) {
	if (head->next != nullptr) {
		Node* current = head;
		while (current->next != nullptr) {
			if (current->next->place.id == id) {
				return &current->next->place;
			}
			current = current->next;
		}
	}

	
}

Place* LinkedList::SearchByName(const std::string& name) {
	if (head->next != nullptr) {
		Node* current = head;
		while (current->next != nullptr) {
			if (current->next->place.name == name) {
				return &current->next->place;
			}
			current = current->next;
		}
	}

	
}

LinkedList LinkedList::GetInformation() {
	if (head->next != nullptr) {
		Node* current = head;
		LinkedList* temp = new LinkedList;
		while (current->next != nullptr) {
			temp->insert(current->next->place);
			current = current->next;
		}
		return *temp;
	}
	
}
