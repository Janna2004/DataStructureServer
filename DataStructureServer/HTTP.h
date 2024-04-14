#pragma once
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>
#include <mutex>
#include <future>
#include <vector>
#include "httplib.h" // cpp-httplib库
#include "List.h"
#include "Graph.h"

using json = nlohmann::json;
using namespace httplib;

extern List list;
extern Graph graph;

void handleRequest(const httplib::Request&, std::promise<json>);