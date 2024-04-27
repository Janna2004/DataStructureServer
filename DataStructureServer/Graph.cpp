#include "Graph.h"

Graph::Graph() : num(0), array(nullptr) {
    // 由于初始时图中没有点，num设置为0，array设置为nullptr
}

Graph::~Graph() {
    if (array != nullptr) {
        std::unordered_set<Edge*> deletedEdges; // 用来跟踪已删除的边，避免重复删除

        for (int i = 0; i < num; ++i) {
            Edge* current = array[i].head;
            while (current != nullptr) {
                if (deletedEdges.find(current) == deletedEdges.end()) { // 如果当前边尚未删除
                    deletedEdges.insert(current); // 标记为已删除
                    Edge* temp = current;
                    current = current->next;
                    delete temp; // 删除当前边
                }
                else {
                    current = current->next; // 如果已经删除了，跳过
                }
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

    // 添加从 start 到 end 的边
    Edge* newEdge = new Edge{ &end, distance, array[start.id].head };
    array[start.id].head = newEdge;

    // 由于是无向图，还需要添加从 end 到 start 的边
    Edge* reverseEdge = new Edge{ &start, distance, array[end.id].head };
    array[end.id].head = reverseEdge;
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


std::pair<double, std::vector<Point>> Graph::Navigation(const Point& start, const Point& end) {
    std::vector<double> dist(num, std::numeric_limits<double>::infinity());
    std::vector<const Point*> prev(num, nullptr);
    std::vector<bool> visited(num, false);

    dist[start.id] = 0;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;
    pq.emplace(0, start.id);

    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();

        if (visited[current]) continue; // 避免重复处理节点
        visited[current] = true;

        if (current == end.id) {
            break; // 如果达到终点，提前退出
        }

        for (Edge* e = array[current].head; e != nullptr; e = e->next) {
            int neighbor = e->point->id;
            double weight = e->distance;
            if (dist[current] + weight < dist[neighbor]) {
                dist[neighbor] = dist[current] + weight;
                prev[neighbor] = array[current].point;
                pq.emplace(dist[neighbor], neighbor);
            }
        }
    }

    // 构建路径
    std::vector<Point> path;
    if (dist[end.id] == std::numeric_limits<double>::infinity()) {
        return std::make_pair(-1, std::vector<Point>());  // 无法到达终点
    }

    for (const Point* at = &end; at != nullptr; at = prev[at->id]) {
        path.push_back(*at);
        if (at->id == start.id) break;
    }
    std::reverse(path.begin(), path.end());

    return std::make_pair(dist[end.id], path);
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

std::pair<double, std::vector<Point>> Graph::TSP(const Point& start) {
    std::vector<int> places;
    for (int i = 0; i < this->GetNum(); i++) {
        Point* p = this->SearchById(i);
        if (p && p->placeId != -1) {
            places.push_back(i);
        }
    }
    int placeNum = places.size();
    const int inf = std::numeric_limits<int>::max();
    std::vector<std::vector<double>> dp(1 << placeNum, std::vector<double>(placeNum, inf));
    std::vector<std::vector<int>> parent(1 << placeNum, std::vector<int>(placeNum, -1));
    int startIdx = std::find(places.begin(), places.end(), start.id) - places.begin();

    dp[1 << startIdx][startIdx] = 0;

    for (int mask = 1; mask < (1 << placeNum); mask++) {
        for (int i = 0; i < placeNum; i++) {
            if (mask & (1 << i)) {
                for (int j = 0; j < placeNum; j++) {
                    if (!(mask & (1 << j))) {
                        int nextMask = mask | (1 << j);
                        double distance = Navigation(*SearchById(places[i]), *SearchById(places[j])).first;
                        double cost = dp[mask][i] + distance;
                        if (cost < dp[nextMask][j]) {
                            dp[nextMask][j] = cost;
                            parent[nextMask][j] = i;
                        }
                    }
                }
            }
        }
    }

    double minCost = inf;
    int lastIndex = -1;
    for (int i = 0; i < placeNum; i++) {
        double distance = Navigation(*SearchById(places[i]), start).first;
        double cost = dp[(1 << placeNum) - 1][i] + distance;
        if (cost < minCost) {
            minCost = cost;
            lastIndex = i;
        }
    }

    std::vector<Point> path;
    int mask = (1 << placeNum) - 1;
    while (lastIndex != -1) {
        int pIndex = parent[mask][lastIndex];
        if (pIndex != -1) {
            std::vector<Point> segment = Navigation(*SearchById(places[pIndex]), *SearchById(places[lastIndex])).second;
            std::reverse(segment.begin(), segment.end());
            // 根据需求，我们需要从返回的路径segment中去掉第一个点，因为它是上一段的终点
            if (segment.size() > 1) {
                path.insert(path.end(), segment.begin() + 1, segment.end());
            }
        }
        mask ^= (1 << lastIndex);
        lastIndex = pIndex;
    }
    std::reverse(path.begin(), path.end());
    // 添加从最后一个点回到起始点的路径
    std::vector<Point> lastSegment = Navigation(path.back(), start).second;
    if (lastSegment.size() > 1) {
        path.insert(path.end(), lastSegment.begin() + 1, lastSegment.end());
    }
    return { minCost, path };
}


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
