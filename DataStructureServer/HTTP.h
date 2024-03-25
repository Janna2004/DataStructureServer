#pragma once
#include <iostream>
#include "httplib.h" // cpp-httplib¿â
#include <thread>

using namespace httplib;

void handleRequest(const httplib::Request& req, httplib::Response& res);