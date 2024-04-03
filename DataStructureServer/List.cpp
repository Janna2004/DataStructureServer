#include "List.h"

// 在构造函数的初始化列表中初始化成员变量
List::List() : num(0), places() {} 


//添加地点
void List::AddPlace(const Place& place) {
	places.insert(place);
	num++;
}

//删除地点
void List::DeletePlace(int id) {
	places.remove(id);
	num--;
}

//搜索地点byID

Place* List::SearchById(int id) {
	return places.SearchById(id);
}

//搜索地点byName
Place* List::SearchByName(const std::string& name) {
	return places.SearchByName(name);
	
}

//获取所有place信息
LinkedList List::GetInformation(){
	return places.GetInformation();
}



