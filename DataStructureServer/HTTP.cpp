#include"HTTP.h"
#include <mutex>

std::mutex mutex; // 用于保护共享数据的互斥量

void handleRequest(const httplib::Request& req, httplib::Response& res) {
    if (req.method == "POST") {
        if (req.path == "/creatRoom") {
            std::lock_guard<std::mutex> lock(mutex);
        }
        else if (req.path == "/searchNear") {
            // 按距离搜索
            // ...
        }
        else if (req.path == "/navigate") {
            // 导航
            // ...
        }
    }
    else if (req.method == "GET") {
        if (req.path == "/getRooms") {
            // 处理查询房间目录的GET请求
        }
        else if (req.path == "/allPlaces") {
            // 获取所有景点位置
            // ...
        }
        else if (req.path == "/info") {
            // 获取单个景点介绍
            // ...
        }
        else if (req.path == "/searchOne") {
            // 单个查找景点
            // ...
        }
    }
}