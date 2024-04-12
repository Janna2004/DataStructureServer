#include "Graph.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <utility> // ����std::pair
#include <limits>  // ����std::numeric_limits
#include <algorithm> // ����std::sort
#include <cmath>     // ����std::sqrt��std::pow

Graph::Graph() : num(0), array(nullptr) {
    // ���ڳ�ʼʱͼ��û�е㣬num����Ϊ0��array����Ϊnullptr
}

Graph::~Graph() {
    if (array != nullptr) {
        for (int i = 0; i < num; ++i) {
            Edge* current = array[i].head;
            while (current != nullptr) {
                Edge* temp = current;
                current = current->next;
                delete temp; // ɾ����ǰ��
            }
        }
        delete[] array; // ɾ�������ڽӱ�����
    }
}


void Graph::AddEdge(Point& start, Point& end, double distance) {

    if (start.id < 0 || start.id >= num || end.id < 0 || end.id >= num) {
        std::cerr << "�����յ㲻������ͼ�С�" << std::endl;
        return;
    }

    Edge* newEdge = new Edge{ &end, distance, array[start.id].head };
    array[start.id].head = newEdge;
}

void Graph::DeleteEdge(Point& start, Point& end) {
    if (start.id < 0 || start.id >= num || end.id < 0 || end.id >= num) {
        std::cerr << "�����յ㲻������ͼ�С�" << std::endl;
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

    std::cerr << "û���ҵ�����㵽�յ�ıߡ�" << std::endl;
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
        std::cerr << "�� " << a.id << " �Ѿ�������ͼ�С�" << std::endl;
    }
}

void Graph::DeletePoint(const Point& a) {
    if (a.id < 0 || a.id >= num || array[a.id].point == nullptr) {
        std::cerr << "�㲻������ͼ�С�" << std::endl;
        return;
    }

    // ɾ�����г���
    Edge* current = array[a.id].head;
    while (current != nullptr) {
        Edge* temp = current;
        current = current->next;
        delete temp;
    }
    array[a.id].head = nullptr;

    // ɾ���������
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
        std::cerr << "�ṩ��ID����ͼ�ķ�Χ��" << std::endl;
        return nullptr;
    }

    return array[id].point;
}


std::pair<int, std::vector<Point>> Graph::Navigation(const Point& start, const Point& end) {
    std::vector<int> dist(num, std::numeric_limits<int>::max());  // �洢��㵽ÿ�������̾���
    std::vector<int> prev(num, -1);  // �洢����ÿ��������·���ϸõ��ǰһ����
    std::vector<bool> visited(num, false);  // ���ÿ�����Ƿ��Ѿ��ҵ������·��

    // ʹ�����ȶ����Ż�ѡ�����
    auto cmp = [&dist](int left, int right) { return dist[left] > dist[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    dist[start.id] = 0;  // ��㵽�Լ�����̾���Ϊ0
    pq.push(start.id);

    while (!pq.empty()) {
        int u = pq.top();  // ѡ��һ��δ����ľ�����С�Ķ���
        pq.pop();

        if (visited[u]) continue;  // �����������Ѿ��������������
        visited[u] = true;

        for (Edge* e = array[u].head; e != nullptr; e = e->next) {
            int v = e->point->id;
            int alt = dist[u] + static_cast<int>(e->distance);  // �����µľ���
            if (alt < dist[v]) {  // ����ҵ��˸��̵�·���������
                dist[v] = alt;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    // �ؽ�·��
    std::vector<Point> path;
    for (int at = end.id; at != -1; at = prev[at]) {
        path.push_back(*array[at].point);
    }
    std::reverse(path.begin(), path.end());  // ��ת·������Ϊ�����Ǵ��յ��������ݵ�

    if (path.size() == 1 && path[0].id != start.id) {  // ���ֻ��һ�����Ҹõ㲻����㣬��û��·��
        return { 0, {} };
    }

    return { dist[end.id], path };
}

std::vector<Point> Graph::SearchByRange(const Point& center, int range) {
    std::vector<Point> pointsWithinRange; // �洢�ڷ�Χ�ڵĵ�
    std::vector<bool> visited(num, false); // ����ѷ��ʵĵ�
    std::queue<std::pair<int, int>> q; // �洢���ID�ʹ����ĵ㵽�õ�ľ���

    visited[center.id] = true; // ������ĵ�Ϊ�ѷ���
    q.push(std::make_pair(center.id, 0)); // �����ĵ㿪ʼ������Ϊ0

    while (!q.empty()) {
        int currentId = q.front().first; // ��ǰ���ID
        int dist = q.front().second; // �����ĵ㵽��ǰ��ľ���
        q.pop();

        // �����ǰ���Ǿ�������ָ����Χ�ڣ���ӵ�����б�
        if (array[currentId].point->placeId != -1 && dist <= range) {
            pointsWithinRange.push_back(*array[currentId].point);
        }

        // ������ǰ��������ڽӵ�
        for (Edge* e = array[currentId].head; e != nullptr; e = e->next) {
            int nextId = e->point->id;
            int nextDist = dist + static_cast<int>(e->distance); // �ۼӾ���

            if (!visited[nextId] && nextDist <= range) {
                visited[nextId] = true; // ���Ϊ�ѷ���
                q.push(std::make_pair(nextId, nextDist)); // ���ڽӵ㼰��������
            }
        }
    }

    return pointsWithinRange;
}

void Graph::SortByDistance(std::vector<Point>& points) {
    Point referencePoint(0, 0, 0); // ����ο�����(0,0)
    std::sort(points.begin(), points.end(), [&referencePoint](const Point& a, const Point& b) {
        return std::hypot(a.x - referencePoint.x, a.y - referencePoint.y) <
            std::hypot(b.x - referencePoint.x, b.y - referencePoint.y);
        });
}

//void Graph::SortByDistance(std::vector<Point>& points) {
//    // �������е��ƽ�����꣬�����ο���
//    double sumX = 0, sumY = 0;
//    int count = 0;
//    for (const auto& pt : points) {
//        sumX += pt.x;
//        sumY += pt.y;
//        count++;
//    }
//    Point referencePoint = Point(-1, sumX / count, sumY / count); // �����������Ĳο���
//
//    // ����ȽϺ��������ڱȽ���������ο���ľ���
//    auto compareByDistance = [&referencePoint](const Point& a, const Point& b) -> bool {
//        double distA = std::sqrt(std::pow(a.x - referencePoint.x, 2) + std::pow(a.y - referencePoint.y, 2));
//        double distB = std::sqrt(std::pow(b.x - referencePoint.x, 2) + std::pow(b.y - referencePoint.y, 2));
//        return distA < distB; // ��������������
//    };
//
//    // ʹ��std::sort�ͱȽϺ�����points��������
//    std::sort(points.begin(), points.end(), compareByDistance);
//}

std::vector<Point> Graph::GetInformationByDeepth() {
    std::vector<Point> result; // ���ڴ洢�������
    std::vector<bool> visited(num, false); // ��¼ÿ�����Ƿ��ѷ���
    std::stack<int> stack; // ʹ��ջ��ģ��ݹ��DFS����

    // �������е���ȷ���ܷ��ʵ�������ͨ����
    for (int start = 0; start < num; ++start) {
        if (!visited[start] && array[start].point != nullptr) {
            stack.push(start); // ��δ���ʵ���ʼ��ѹ��ջ

            while (!stack.empty()) {
                int current = stack.top(); // ����ջ��Ԫ��
                stack.pop();

                if (!visited[current]) {
                    visited[current] = true; // ��ǵ�ǰ��Ϊ�ѷ���
                    result.push_back(*array[current].point); // �ռ���ǰ�����Ϣ

                    // ������δ���ʵ��ڽӵ�ѹ��ջ��
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

    return result; // ���ذ������е���Ϣ������
}

std::vector<Point> Graph::GetInformationByWidth() {
    std::vector<Point> result; // ���ڴ洢�������
    std::vector<bool> visited(num, false); // ��¼ÿ�����Ƿ��ѷ���
    std::queue<int> queue; // ʹ�ö��н��й�����ȱ���

    // �������е���ȷ���ܷ��ʵ�������ͨ����
    for (int start = 0; start < num; ++start) {
        if (!visited[start] && array[start].point != nullptr) {
            queue.push(start); // ��δ���ʵ���ʼ��������

            while (!queue.empty()) {
                int current = queue.front(); // ���ʶ��е�ǰ��Ԫ��
                queue.pop();

                if (!visited[current]) {
                    visited[current] = true; // ��ǵ�ǰ��Ϊ�ѷ���
                    result.push_back(*array[current].point); // �ռ���ǰ�����Ϣ

                    // ������δ���ʵ��ڽӵ�������
                    for (Edge* e = array[current].head; e != nullptr; e = e->next) {
                        if (!visited[e->point->id]) {
                            queue.push(e->point->id);
                        }
                    }
                }
            }
        }
    }

    return result; // ���ذ������е���Ϣ������
}
