#pragma once
#include "Place.h"
#include <vector>
#include <algorithm>
#include <iostream>

class List {
private:
    int num;// ��������
    std::vector<Place> places;//���Ա�洢��������һ��ʾ������Ҫ�ִ�һ�����ݽṹ
public:
    List() {};// ���캯����������main�����г�ʼ��һ�����Ա�
    void AddPlace(const Place& place);// ���һ���µĵص�
    void DeletePlace(int id);// ɾ��һ���ص�
    Place* SearchById(int id);// ����ID�����ص㣬������һ�����ٵ������취,����ָ��ö����ָ�롣
    Place* SearchByName(const std::string& name);// �������������ص�,����ָ��ö����ָ�롣

    int getNum() { return num; }// ��ȡ��������
    std::vector<Place> GetInformation();// ��ȡ���еص����Ϣ
};