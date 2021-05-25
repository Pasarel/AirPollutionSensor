#pragma once
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <time.h>
#include <map>
#include <unordered_map>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Base64Encoder.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <boost/interprocess/sync/file_lock.hpp>
#include "json.hpp"

using namespace Poco::Net;
using namespace Poco;
using json = nlohmann::json;

class APIClient {
public:
	APIClient();
	~APIClient();
	void init();
	void fetch();
	std::string send_request(std::string method, std::string url, std::string body, std::map<std::string,std::string> headers, bool SSL);
	std::string send_request(std::string method, std::string url, std::string body, std::map<std::string,std::string> headers);
	std::string send_request(std::string method, std::string url);
	std::string b64_get(std::string host, std::string path);
private:
	std::map<std::pair<double, double>, json> coords;
	std::unordered_map<std::string, std::vector<std::pair<double, double>>*> cities;
	std::unordered_map<std::string, std::vector<std::pair<double, double>>*> service_to_coords;
	std::unordered_map<std::string, std::string> endpoints = {
		{"waqi", "kproxy.com/servlet/redirect.srv/stluq/ssbmj/sxfr/p2/"},
		{"waqi_station", "feed/geo:"},
		{"waqi_full_ro", "map/bounds/?latlng=43.666667,29.65,48.25,24.325"},
		{"aerlive", "https://apps.roiot.ro/aerlive/api/data.php"},
		{"reverse_geocode", "https://api.opencagedata.com/geocode/v1/json?key=4471dab40df24fbcac3155b20b5fd2c7&q="}
	};
	std::unordered_map<std::string, std::string> tokens = {
		{"waqi", "token=d5267f003a98fc2edfc09abb8fa7be4a6e1f77f6"},
		{"aerlive", "key=d09668ea-def5-44ea-8c77-ae32e9fa5572"}
	};
	int sleep_time = 30;

	void clear_points();
	void new_point(std::pair<double, double>& points);
	void reverse_geocode(std::pair<double, double>& pct);
	void populate();
	void fetchWaqi();
	void fetchAerlive();
	void compute_means();
	void notify_webhook(json data);
	json jsonize();
};