#include "Graph.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <utility> // 对于std::pair
#include <limits>  // 对于std::numeric_limits
#include <algorithm> // 用于std::sort
#include <cmath>     // 用于std::sqrt和std::pow


Graph::Graph() : num(0), array(nullptr) {
    // 由于初始时图中没有点，num设置为0，array设置为nullptr
}

Graph::~Graph() {
    if (array != nullptr) {
        for (int i = 0; i < num; ++i) {
            Edge* current = array[i].head;
            while (current != nullptr) {
                Edge* temp = current;
                current = current->next;
                delete temp; // 删除当前边
            }
        }
        delete[] array; // 删除整个邻接表数组
    }
}


void Graph::AddEdge(Point& start, Point& end, double distance) {
    if (start.id < 0 || start.id >= num || end.id < 0 || end.id >= num) {
        std::cerr << "起点或终点不存在于图中。 " << std::endl;
        return;
    }

    Edge* newEdge = new Edge{ &end, distance, array[start.id].head };
    array[start.id].head = newEdge;
}

void Graph::DeleteEdge(Point& start, Point& end) {
    if (start.id < 0 || start.id >= num || end.id < 0 || end.id >= num) {
        std::cerr << "起点或终点不存在于图中。 " << std::endl;
        return;
    }

    Edge** edgePtr = &array[start.id].head;
    while (*edgePtr != nullptr) {
        Edge* currentEdge = *edgePtr;
        if (currentEdge->point->id == end.id) {
            *edgePtr = currentEdge->next;
            delete currentEdge;
            return;
        }
        edgePtr = &currentEdge->next;
    }

    std::cerr << "没有找到从起点到终点的边。 " << std::endl;
}

void Graph::AddPoint(const Point& a) {
    if (a.id >= num) {
        int newSize = std::max(a.id + 1, 2 * num);
        AdjList* newArray = new AdjList[newSize];
        for (int i = 0; i < num; ++i) {
            newArray[i] = array[i];
        }
        for (int i = num; i < newSize; ++i) {
            newArray[i].point = nullptr;
            newArray[i].head = nullptr;
        }
        delete[] array;
        array = newArray;
        num = newSize;
    }

    if (array[a.id].point == nullptr) {
        array[a.id].point = new Point(a);
        array[a.id].head = nullptr;
    }
    else {
        std::cerr << "点 " << a.id << " 已经存在于图中。 " << std::endl;
    }
}

void Graph::DeletePoint(const Point& a) {
    if (a.id < 0 || a.id >= num || array[a.id].point == nullptr) {
        std::cerr << "点不存在于图中。 " << std::endl;
        return;
    }

    // 删除所有出边
    Edge* current = array[a.id].head;
    while (current != nullptr) {
        Edge* temp = current;
        current = current->next;
        delete temp;
    }
    array[a.id].head = nullptr;

    // 删除所有入边
    for (int i = 0; i < num; ++i) {
        Edge** edgePtr = &array[i].head;
        while (*edgePtr != nullptr) {
            Edge* edge = *edgePtr;
            if (edge->point->id == a.id) {
                *edgePtr = edge->next;
                delete edge;
            }
            else {
                edgePtr = &edge->next;
            }
        }
    }

    delete array[a.id].point;
    array[a.id].point = nullptr;
}


Point* Graph::SearchById(int id) {
    if (id < 0 || id >= num) {
        std::cerr << "提供的ID超出图的范围。 " << std::endl;
        return nullptr;
    }

    return array[id].point;
}


std::pair<int, std::vector<Point>> Graph::Navigation(const Point& start, const Point& end) {
    std::vector<int> dist(num, std::numeric_limits<int>::max());  // 存储起点到每个点的最短距离
    std::vector<int> prev(num, -1);  // 存储到达每个点的最短路径上该点的前一个点
    std::vector<bool> visited(num, false);  // 标记每个点是否已经找到了最短路径

    // 使用优先队列优化选择过程
    auto cmp = [&dist](int left, int right) { return dist[left] > dist[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    dist[start.id] = 0;  // 起点到自己的最短距离为0
    pq.push(start.id);

    while (!pq.empty()) {
        int u = pq.top();  // 选择一个未处理的距离最小的顶点
        pq.pop();

        if (visited[u]) continue;  // 如果这个顶点已经处理过，则跳过
        visited[u] = true;

        for (Edge* e = array[u].head; e != nullptr; e = e->next) {
            int v = e->point->id;
            int alt = dist[u] + static_cast<int>(e->distance);  // 计算新的距离
            if (alt < dist[v]) {  // 如果找到了更短的路径，则更新
                dist[v] = alt;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    // 重建路径
    std::vector<Point> path;
    for (int at = end.id; at != -1; at = prev[at]) {
        path.push_back(*array[at].point);
    }
    std::reverse(path.begin(), path.end());  // 翻转路径，因为我们是从终点向起点回溯的

    if (path.size() == 1 && path[0].id != start.id) {  // 如果只有一个点且该点不是起点，则没有路径
        return { 0, {} };
    }

    return { dist[end.id], path };
}

std::vector<Point> Graph::SearchByRangePoint(const Point& center, int range) {
    std::vector<Point> pointsWithinRange; // 存储在范围内的点
    std::vector<bool> visited(num, false); // 标记已访问的点
    std::queue<std::pair<int, int>> q; // 存储点的ID和从中心点到该点的距离

    visited[center.id] = true; // 标记中心点为已访问
    q.push(std::make_pair(center.id, 0)); // 从中心点开始，距离为0

    while (!q.empty()) {
        int currentId = q.front().first; // 当前点的ID
        int dist = q.front().second; // 从中心点到当前点的距离
        q.pop();

        // 如果当前点是景点且在指定范围内，添加到结果列表
        if (array[currentId].point->placeId != -1 && dist <= range) {
            pointsWithinRange.push_back(*array[currentId].point);
        }

        // 遍历当前点的所有邻接点
        for (Edge* e = array[currentId].head; e != nullptr; e = e->next) {
            int nextId = e->point->id;
            int nextDist = dist + static_cast<int>(e->distance); // 累加距离

            if (!visited[nextId] && nextDist <= range) {
                visited[nextId] = true; // 标记为已访问
                q.push(std::make_pair(nextId, nextDist)); // 将邻接点及其距离入队
            }
        }
    }
    return pointsWithinRange;
}

std::vector<Point> Graph::SearchByRangeAny(double x,double y, int range) {
    std::vector<Point> pointsWithinRange; // 存储在范围内的点


    return pointsWithinRange;
}

void Graph::SortByDistance(std::vector<Point>& points) {
    Point referencePoint(0, 0, 0); // 假设参考点是(0,0)
    std::sort(points.begin(), points.end(), [&referencePoint](const Point& a, const Point& b) {
        return std::hypot(a.x - referencePoint.x, a.y - referencePoint.y) <
            std::hypot(b.x - referencePoint.x, b.y - referencePoint.y);
        });
}

std::pair<int, std::vector<Point>> Graph::TSP(const Point& start){
    // 首先，创建一个只包含景点的列表
    std::vector<int> places;
    int placeNum = 0;
    for (int i = 0; i < this->GetNum(); i++) {
        Point* p = this->SearchById(i);
        if (p && p->placeId != -1) {
            places.push_back(i);
            placeNum++;
        }
    }
    // 初始化动态规划表
    const int inf = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dp(1 << placeNum, std::vector<int>(placeNum, inf));
    std::vector<std::vector<int>> parent(1 << placeNum, std::vector<int>(placeNum, -1));
    int startIdx = std::find(places.begin(), places.end(), start.id) - places.begin();

    // 初始状态
    dp[1 << startIdx][startIdx] = 0;

    // 填充表
    int distance = 0;
    for (int mask = 0; mask < (1 << placeNum); mask++) {
        for (int i = 0; i < placeNum; i++) {
            if (mask & (1 << i)) {
                for (int j = 0; j < placeNum; j++) {
                    if (!(mask & (1 << j))) {
                        int nextMask = mask | (1 << j);
                        distance = Navigation(*SearchById(places[i]), *SearchById(places[j])).first;
                        int cost = dp[mask][i] + distance;
                        if (cost < dp[nextMask][j]) {
                            dp[nextMask][j] = cost;
                            parent[nextMask][j] = i;
                        }
                    }
                }
            }
        }
    }

    // 寻找最小的回路
    int finalMask = (1 << placeNum) - 1;
    int minCost = inf, lastIndex = -1;
    for (int i = 0; i < placeNum; i++) {
        distance = Navigation(*SearchById(places[i]), start).first;
        int cost = dp[finalMask][i] + distance;
        if (cost < minCost) {
            minCost = cost;
            lastIndex = i;
        }
    }

    // 构建路径
    std::vector<Point> path;
    for (int mask = finalMask, index = lastIndex; index != -1; ) {
        path.push_back(*SearchById(places[index]));
        int pIndex = parent[mask][index];
        mask &= ~(1 << index);
        index = pIndex;
    }
    std::reverse(path.begin(), path.end());

    std::cout << "Path reconstruction:" << std::endl;
    for (const auto& p : path) {
        std::cout << "ID: " << p.id << " Coords: (" << p.x << ", " << p.y << ")" << std::endl;
    }

    return { minCost, path };
}

//void Graph::SortByDistance(std::vector<Point>& points) {
//    // 计算所有点的平均坐标，用作参考点
//    double sumX = 0, sumY = 0;
//    int count = 0;
//    for (const auto& pt : points) {
//        sumX += pt.x;
//        sumY += pt.y;
//        count++;
//    }
//    Point referencePoint = Point(-1, sumX / count, sumY / count); // 创建几何中心参考点
//
//    // 定义比较函数，用于比较两个点与参考点的距离
//    auto compareByDistance = [&referencePoint](const Point& a, const Point& b) -> bool {
//        double distA = std::sqrt(std::pow(a.x - referencePoint.x, 2) + std::pow(a.y - referencePoint.y, 2));
//        double distB = std::sqrt(std::pow(b.x - referencePoint.x, 2) + std::pow(b.y - referencePoint.y, 2));
//        return distA < distB; // 按距离升序排序
//    };
//
//    // 使用std::sort和比较函数对points进行排序
//    std::sort(points.begin(), points.end(), compareByDistance);
//}

std::vector<Point> Graph::GetInformationByDeepth() {
    std::vector<Point> result; // 用于存储遍历结果
    std::vector<bool> visited(num, false); // 记录每个点是否已访问
    std::stack<int> stack; // 使用栈来模拟递归的DFS过程

    // 遍历所有点以确保能访问到所有连通分量
    for (int start = 0; start < num; ++start) {
        if (!visited[start] && array[start].point != nullptr) {
            stack.push(start); // 将未访问的起始点压入栈

            while (!stack.empty()) {
                int current = stack.top(); // 访问栈顶元素
                stack.pop();

                if (!visited[current]) {
                    visited[current] = true; // 标记当前点为已访问
                    result.push_back(*array[current].point); // 收集当前点的信息

                    // 将所有未访问的邻接点压入栈中
                    Edge* e = array[current].head;
                    while (e != nullptr) {
                        if (!visited[e->point->id]) {
                            stack.push(e->point->id);
                        }
                        e = e->next;
                    }
                }
            }
        }
    }

    return result; // 返回包含所有点信息的向量
}

std::vector<Point> Graph::GetInformationByWidth() {
    std::vector<Point> result; // 用于存储遍历结果
    std::vector<bool> visited(num, false); // 记录每个点是否已访问
    std::queue<int> queue; // 使用队列进行广度优先遍历

    // 遍历所有点以确保能访问到所有连通分量
    for (int start = 0; start < num; ++start) {
        if (!visited[start] && array[start].point != nullptr) {
            queue.push(start); // 将未访问的起始点加入队列

            while (!queue.empty()) {
                int current = queue.front(); // 访问队列的前端元素
                queue.pop();

                if (!visited[current]) {
                    visited[current] = true; // 标记当前点为已访问
                    result.push_back(*array[current].point); // 收集当前点的信息

                    // 将所有未访问的邻接点加入队列
                    for (Edge* e = array[current].head; e != nullptr; e = e->next) {
                        if (!visited[e->point->id]) {
                            queue.push(e->point->id);
                        }
                    }
                }
            }
        }
    }

    return result; // 返回包含所有点信息的向量
}
