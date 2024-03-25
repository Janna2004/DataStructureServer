#include"HTTP.h"
#include <mutex>

std::mutex mutex; // ���ڱ����������ݵĻ�����

void handleRequest(const httplib::Request& req, httplib::Response& res) {
    if (req.method == "POST") {
        if (req.path == "/creatRoom") {
            std::lock_guard<std::mutex> lock(mutex);
        }
        else if (req.path == "/searchNear") {
            // ����������
            // ...
        }
        else if (req.path == "/navigate") {
            // ����
            // ...
        }
    }
    else if (req.method == "GET") {
        if (req.path == "/getRooms") {
            // �����ѯ����Ŀ¼��GET����
        }
        else if (req.path == "/allPlaces") {
            // ��ȡ���о���λ��
            // ...
        }
        else if (req.path == "/info") {
            // ��ȡ�����������
            // ...
        }
        else if (req.path == "/searchOne") {
            // �������Ҿ���
            // ...
        }
    }
}