#pragma once
#include <stdlib.h>
#include <fstream>
#include <chrono>
#include <time.h>
#include <pistache/client.h>
#include <unordered_map>
#include <boost/interprocess/sync/file_lock.hpp>
#include "json.hpp"

using json = nlohmann::json;
using namespace Pistache;

class APIClient {
public:
	APIClient();
	~APIClient();
private:
	Http::Client client;
	std::map<std::pair<double, double>, json> coords;
	std::unordered_map<std::string, std::vector<std::pair<double, double>>*> cities;
	std::unordered_map<std::string, std::vector<std::pair<double, double>>*> service_to_coords;
	std::unordered_map<std::string, std::string> endpoints = {
		{"waqi", "api.waqi.info/"},
		{"waqi_station", "feed/geo:"},
		{"waqi_full_ro", "map/bounds/?latlng=43.666667,29.65,48.25,24.325"},
		{"aerlive", ".kproxy.com/servlet/redirect.srv/stx/sbxszd/sgefr/p2/aerlive/api/data.php"},
		{"reverse_geocode", "api.opencagedata.com/geocode/v1/json?key=4471dab40df24fbcac3155b20b5fd2c7&q="}
	};
	std::unordered_map<std::string, std::string> tokens = {
		{"waqi", "token=d5267f003a98fc2edfc09abb8fa7be4a6e1f77f6"},
		{"aerlive", "key=d09668ea-def5-44ea-8c77-ae32e9fa5572"}
	};
	int sleep_time = 30;

	void new_point(std::pair<double, double>& points, std::vector<Async::Promise<Http::Response>>& responses);
	void reverse_geocode(std::pair<double, double>& pct, std::vector<Async::Promise<Http::Response>>& responses);
	void populate();
	void fetchWaqi(std::vector<Async::Promise<Http::Response>>& responses);
	void fetchAerlive(std::vector<Async::Promise<Http::Response>>& responses);
	void compute_means();
	void notify_webhook(json data);
	json jsonize();
	void fetch();
};