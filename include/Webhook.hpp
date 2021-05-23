#pragma once
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <time.h>
#include <pistache/client.h>
#include <unordered_map>
#include <boost/interprocess/sync/file_lock.hpp>
#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>
#include "json.hpp"

using namespace Pistache;
using json = nlohmann::json;

class Webhook {
private:
	const char* filename = "data.json";
	std::ofstream output;
	boost::interprocess::file_lock output_lock;

public:
	Webhook();
	void push(const Rest::Request & request, Http::ResponseWriter response);
};