#include"HTTP.h"
#include<stdio.h>


int main(void){
    //调用初始化函数,建立线性表

    //调用初始化函数,建图

    // http服务启动（已完成）
	Server svr;
	svr.Post(R"(/(searchNear|navigate))", [](const Request& req, Response& res) {
		std::thread(handleRequest, req, std::ref(res)).detach();
		});

	svr.Get(R"(/(allPlaces|info|searchOne))", [](const Request& req, Response& res) {
		std::thread(handleRequest, req, std::ref(res)).detach();
		});

	std::cout << "Server is listening on port 8080..." << std::endl;
	svr.listen("localhost", 8080); //本地测试：访问http://127.0.0.1:8080/
	return 0;
}