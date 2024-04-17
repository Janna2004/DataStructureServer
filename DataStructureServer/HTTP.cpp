#include"HTTP.h"


std::mutex mutex; // 用于保护共享数据的互斥量
std::string UTF8_To_string(const std::string& str){
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1];//加1用于截断字符串 
    memset(pwBuf, 0, nwLen * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);

    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr = pBuf;

    delete[]pBuf;
    delete[]pwBuf;

    pBuf = NULL;
    pwBuf = NULL;

    return retStr;
}
std::string string_To_UTF8(const std::string& str){
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

    wchar_t* pwBuf = new wchar_t[nwLen + 1];//加1用于截断字符串 
    ZeroMemory(pwBuf, nwLen * 2 + 2);

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr(pBuf);

    delete[]pwBuf;
    delete[]pBuf;

    pwBuf = NULL;
    pBuf = NULL;

    return retStr;
}


void handleRequest(const httplib::Request& req, std::promise<json> prom) {
    if (req.method == "POST") {
        try {
            if (req.path == "/searchNearByPoint") {
                std::lock_guard<std::mutex> lock(mutex);
                // 解析请求主体中的JSON数据
                json requestData = json::parse(req.body);
                int centerId = requestData["centerId"];
                double range = requestData["range"];
                Point *startPoint = graph.SearchById(centerId);
                if (startPoint == nullptr) {
                    // 如果未找到ID对应的点，返回错误信息
                    nlohmann::json response;
                    response["code"] = 404;
                    response["msg"] = "Start point not found";
                    prom.set_value(response);
                    return;
                }
                std::vector<Point>result = graph.SearchByRangePoint(*startPoint,range);
                graph.SortByDistance(result);
                nlohmann::json data = nlohmann::json::array();
                for (const auto& pt : result) {
                    nlohmann::json p;
                    p["id"] = pt.id;
                    p["x"] = pt.x;
                    p["y"] = pt.y;
                    p["name"] = string_To_UTF8(list.SearchById(pt.id)->name);
                    data.push_back(p);
                }
                nlohmann::json response;
                response["code"] = 200;
                response["data"] = data;
                response["msg"] = "success";
                printf("response: %s\n", response.dump().c_str());
                prom.set_value(response);
            }
            else if (req.path == "/searchNearDirect") {
                std::lock_guard<std::mutex> lock(mutex);
                // 解析请求主体中的JSON数据
                json requestData = json::parse(req.body);
                double x = requestData["x"];
                double y = requestData["y"];
                double range = requestData["range"];
                // 具体实现

                nlohmann::json response;
                response["code"] = 200;
                // response["data"] = data;
                response["msg"] = "success";
                printf("response: %s\n", response.dump().c_str());
                prom.set_value(response);
            }
            else if (req.path == "/navigate") {
                std::lock_guard<std::mutex> lock(mutex);
                // 解析请求主体中的JSON数据
                json requestData = json::parse(req.body);

                // 提取 startId 和 endId
                int startId = requestData["from"]["id"];
                int endId = requestData["to"]["id"];

                Point* startPoint = graph.SearchById(startId);
                Point* endPoint = graph.SearchById(endId);
                if (startPoint == nullptr || endPoint == nullptr) {
                    // 如果未找到ID对应的点，返回错误信息
                    nlohmann::json response;
                    response["code"] = 404;
                    response["msg"] = "Start point not found";
                    prom.set_value(response);
                    return;
                }
                auto result = graph.Navigation(*startPoint, *endPoint);
                for (const auto& pt : result.second) {
                    printf("id: %d\n", pt.id);
                }
                nlohmann::json pathArray = nlohmann::json::array();
                for (const auto& pt : result.second) {
                    nlohmann::json p;
                    p["x"] = pt.x;
                    p["y"] = pt.y;
                    pathArray.push_back(p);
                }
                nlohmann::json data;
                data["from"] = { {"id", startPoint->id}, {"x", startPoint->x}, {"y", startPoint->y}};
                data["to"] = { {"id", endPoint->id}, {"x", endPoint->x}, {"y", endPoint->y} };
                data["path"] = pathArray;
                data["distance"] = result.first;

                nlohmann::json response;
                response["code"] = 200;
                response["data"] = data;
                response["msg"] = "success";
                printf("response: %s\n", response.dump().c_str());
                prom.set_value(response);
            }
        }
        catch (const std::exception& e) {
            json response;
            response["code"] = 500;
            response["msg"] = e.what();
            prom.set_value(response); // 发生异常时也设置promise的值
        }
    }
    else if (req.method == "GET") {
        try {
            if (req.path == "/allPlaces") {
                std::lock_guard<std::mutex> lock(mutex);
                Node* current = list.GetHeadNode();
                json data = json::array();
                while (current != nullptr) {
                    json place;
                    place["id"] = current->place.id;
                    place["name"] = string_To_UTF8(current->place.name);
                    place["x"] = current->place.x;
                    place["y"] = current->place.y;
                    data.push_back(place);
                    current = current->next;
                }
                json response;
                response["code"] = 200;
                response["data"] = data;
                response["msg"] = "success";
                prom.set_value(response);
            }
            else if (req.path == "/allPoints") {
                std::lock_guard<std::mutex> lock(mutex);
                std::vector<Point>result = graph.GetInformationByWidth();
                nlohmann::json data = nlohmann::json::array();
                for (const auto& pt : result) {
                    nlohmann::json p;
                    p["id"] = pt.id;
                    p["x"] = pt.x;
                    p["y"] = pt.y;
                    p["placeId"] = pt.placeId;
                    data.push_back(p);
                }
                nlohmann::json response;
                response["code"] = 200;
                response["data"] = data;
                response["msg"] = "success";
                printf("response: %s\n", response.dump().c_str());
                prom.set_value(response);
            }
            else if (req.path == "/TSP") {
                std::lock_guard<std::mutex> lock(mutex);
                // 获取请求中的ID参数
                int id = std::stoi(req.get_param_value("id"));
                printf("id: %d\n", id); 
                Point* startPoint = graph.SearchById(id);
                if (startPoint == nullptr) {
                    // 如果未找到ID对应的点，返回错误信息
                    nlohmann::json response;
                    response["code"] = 404;
                    response["msg"] = "Start point not found";
                    prom.set_value(response);
                    return;
                }
                auto result = graph.TSP(*startPoint);
                for (const auto& pt : result.second) {
					printf("id: %d\n", pt.id);
				}
                nlohmann::json pathArray = nlohmann::json::array();
                for (const auto& pt : result.second) {
                    nlohmann::json p;
                    p["x"] = pt.x;
                    p["y"] = pt.y;
                    p["id"] = pt.id;
                    pathArray.push_back(p);
                }
                nlohmann::json data;
                data["point"] = { {"id", startPoint->id}, {"x", startPoint->x}, {"y", startPoint->y},{"name", string_To_UTF8(list.SearchById(startPoint->id)->name)} };
                data["path"] = pathArray;
                data["distance"] = result.first;

                nlohmann::json response;
                response["code"] = 200;
                response["data"] = data;
                response["msg"] = "success";
                printf("response: %s\n", response.dump().c_str());
                prom.set_value(response);
            }
            else if (req.path == "/info") {
                std::lock_guard<std::mutex> lock(mutex);
                // 获取请求中的ID参数
                int id = std::stoi(req.get_param_value("placeId"));
                printf("id: %d\n", id);
                Place* Place = list.SearchById(id);
                if (Place == nullptr) {
                    // 如果未找到ID对应的点，返回错误信息
                    nlohmann::json response;
                    response["code"] = 404;
                    response["msg"] = "point not found";
                    prom.set_value(response);
                    return;
                }
                nlohmann::json data;
                data["id"] = Place->id;
                data["name"] = string_To_UTF8(Place->name);
                data["info"] = string_To_UTF8(Place->description);
                data["x"] = Place->x;
                data["y"] = Place->y;   


                nlohmann::json response;
                response["code"] = 200;
                response["data"] = data;
                response["msg"] = "success";
                printf("response: %s\n", response.dump().c_str());
                prom.set_value(response);
            }
            else if (req.path == "/searchOne") {
                // 获取请求中的name参数
                std::string name = UTF8_To_string(req.get_param_value("placeName"));
                printf("name: %s\n", name.c_str());
                Place* Place = list.SearchByName(name);
                if (Place == nullptr) {
					// 如果未找到Nmae对应的点，返回错误信息
					nlohmann::json response;
					response["code"] = 404;
					response["msg"] = "point not found";
					prom.set_value(response);
					return;
				}
                nlohmann::json data;
				data["id"] = Place->id;
				data["name"] = string_To_UTF8(Place->name);
				data["info"] = string_To_UTF8(Place->description);
				data["x"] = Place->x;
				data["y"] = Place->y;

				nlohmann::json response;
				response["code"] = 200;
				response["data"] = data;
				response["msg"] = "success";
				printf("response: %s\n", response.dump().c_str());
				prom.set_value(response);
            }
        }
        catch (const std::exception& e) {
            printf("error: %s\n", e.what());
            json response;
            response["code"] = 500;
            response["msg"] = e.what();
            prom.set_value(response); // 发生异常时也设置promise的值
        }
    }
}