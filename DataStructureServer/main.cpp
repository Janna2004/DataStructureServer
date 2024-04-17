#include"HTTP.h"
#include<stdio.h>
#include <iostream>
#include <future>
#include <vector>

List list;
Graph graph;

int main(void) {
    //调用初始化函数,建图
    std::vector<Point> points = {
        {1, 116.417548, 39.948745, 1},  // 永康阁
        {2, 116.416958, 39.948741, 2},  // 延绥阁
        {3, 116.417236, 39.948739, 3},  // 观音洞
        {4, 116.417515, 39.948215, 4},  // 班禅楼
        {5, 116.417038, 39.948450, 5},  // 戒台楼
        {6, 116.416958, 39.948215, 6},  // 西配殿
        {7, 116.417232, 39.948063, 7},  // 永佑殿
        {8, 116.417232, 39.947906, 8},  // 大雄宝殿
        {9, 116.417232, 39.948215, -1}, // 拐点3
        {10, 116.417220, 39.948577, -1},// 拐点6
        {11, 116.417038, 39.948585, -1},// 拐点8
        {12, 116.417441, 39.948577, -1},// 拐点7
        {13, 116.417489, 39.947906, -1},// 拐点1
        {14, 116.417017, 39.947906, -1},// 拐点2
        {15, 116.417038, 39.948215, -1},// 拐点4
        {16, 116.417441, 39.948215, -1},// 拐点5
        {17, 116.416914, 39.947906, 17} // 开光室
    };
    // 将点添加到图中
    for (const auto& point : points) {
        graph.AddPoint(point);
    }
    //添加边 
    graph.AddEdge(points[7], points[13], 0.02); // 大雄宝殿-拐点2
    graph.AddEdge(points[7], points[12], 0.02); // 大雄宝殿-拐点1
    graph.AddEdge(points[13], points[16], 0.01); // 拐点2-开光室
    graph.AddEdge(points[8], points[14], 0.02); // 拐点3-拐点4
    graph.AddEdge(points[8], points[15], 0.02); // 拐点3-拐点5
    graph.AddEdge(points[14], points[5], 0.01); // 拐点4-西配殿
    graph.AddEdge(points[15], points[3], 0.01); // 拐点5-班禅楼
    graph.AddEdge(points[7], points[6], 0.02); // 大雄宝殿-永佑殿
    graph.AddEdge(points[6], points[8], 0.02); // 永佑殿-拐点3
    graph.AddEdge(points[14], points[4], 0.03); // 拐点4-戒台楼
    graph.AddEdge(points[4], points[10], 0.01); // 戒台楼-拐点8
    graph.AddEdge(points[8], points[9], 0.04); // 拐点3-拐点6
    graph.AddEdge(points[9], points[2], 0.02); // 拐点6-观音洞
    graph.AddEdge(points[9], points[11], 0.02); // 拐点6-拐点7
    graph.AddEdge(points[9], points[10], 0.02); // 拐点6-拐点8
    graph.AddEdge(points[10], points[1], 0.02); // 拐点8-延绥阁
    graph.AddEdge(points[1], points[2], 0.02); // 延绥阁-观音洞
    graph.AddEdge(points[2], points[0], 0.03); // 观音洞-永康阁
    graph.AddEdge(points[11], points[0], 0.02); // 拐点7-永康阁
    graph.AddEdge(points[15], points[11], 0.04); // 拐点5-拐点7
    graph.AddEdge(points[12], points[15], 0.03); // 拐点1-拐点5
    graph.AddEdge(points[13], points[14], 0.03); // 拐点2-拐点4

    // 创建 Place 对象并插入到链表中
    list.AddPlace(Place(1, "永康阁", 116.417548, 39.948745, "Yongkang Pavilion is a good place, good in many places"));
    list.AddPlace(Place(2, "延绥阁", 116.416958, 39.948741, "Yansui Pavilion is a good place, good in many places"));
    list.AddPlace(Place(3, "观音洞", 116.417236, 39.948739, "Guanyin Cave is a good place, good in many places"));
    list.AddPlace(Place(4, "班禅楼", 116.417515, 39.948215, "Panchen Tower is a good place, good in many places"));
    list.AddPlace(Place(5, "戒台楼", 116.417038, 39.948450, "Jietai Tower is a good place with many advantages."));
    list.AddPlace(Place(6, "西配殿", 116.416958, 39.948215, "The West Wing Hall is a good place with many advantages"));
    list.AddPlace(Place(7, "永佑殿", 116.417232, 39.948063, "Yongyou Hall is a good place, good in many places"));
    list.AddPlace(Place(8, "大雄宝殿", 116.417232, 39.947906, "The Main Hall is a good place with many good things"));
    list.AddPlace(Place(17, "开光室", 116.416914, 39.947906, "The consecration room is a good place, and it has many advantages"));

    // http服务启动
    /*
    Server svr;
    svr.Post(R"(/(searchNearByPoint|navigate))", [](const Request& req, Response& res) {
        std::promise<json> prom;
        auto fut = prom.get_future();
        std::thread([req, prom = std::move(prom)]() mutable {
            handleRequest(req, std::move(prom));
            }).detach();
            res.set_content(fut.get().dump(), "application/json");
        });

    svr.Get(R"(/(allPlaces|allPoints|info|searchOne|TSP))", [](const Request& req, Response& res) {
        std::promise<json> prom;
        auto fut = prom.get_future();
        std::thread([req, prom = std::move(prom)]() mutable {
            handleRequest(req, std::move(prom));
            }).detach();
            res.set_content(fut.get().dump(), "application/json");
        });

    std::cout << "Server is listening on port 8080..." << std::endl;
    svr.listen("localhost", 8080); //本地测试：访问http://127.0.0.1:8080/
    */
    int startId = 7;
    int endId = 8;
    Point* startPoint = graph.SearchById(startId);
    Point* endPoint = graph.SearchById(endId);
    auto result = graph.Navigation(*startPoint, *endPoint);
    return 0;
}