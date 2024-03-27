#include "List.h"

// �ڹ��캯���ĳ�ʼ���б��г�ʼ����Ա����
List::List() : num(0), places() {} 


//��ӵص�
void List::AddPlace(const Place& place) {
	places.insert(place);
	num++;
}

//ɾ���ص�
void List::DeletePlace(int id) {
	places.remove(id);
	num--;
}

//�����ص�byID

Place* List::SearchById(int id) {
	return places.SearchById(id);
}

//�����ص�byName
Place* List::SearchByName(const std::string& name) {
	return places.SearchByName(name);
	
}

//��ȡ����place��Ϣ
LinkedList List::GetInformation(){
	return places.GetInformation();
}



