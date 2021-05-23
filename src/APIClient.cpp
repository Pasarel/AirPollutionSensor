#include "../include/APIClient.hpp"

// output fisier: harta romania + senzori + ferestre

APIClient::APIClient() {
	srand(time(NULL));
	auto opts = Http::Client::options().threads(2).maxConnectionsPerHost(8);
	client.init(opts);
	service_to_coords["waqi"] = new std::vector < std::pair < double, double >> ;
	service_to_coords["aerlive"] = new std::vector < std::pair < double, double >> ;
	populate();
	fetch();
}

APIClient::~APIClient() {
	client.shutdown();
	for (auto city = cities.begin(); city != cities.end(); city++) {
		delete city -> second;
	}
	for (auto stc = service_to_coords.begin(); stc != service_to_coords.end(); stc++) {
		delete stc -> second;
	}
}

void APIClient::new_point(std::pair < double, double > & points, std::vector < Async::Promise < Http::Response >> & responses) {
	if (coords.find(points) == coords.end()) {
		json details = {
			{
				"mean",
				{
					{
						"PM1",
						{
							0,
							0
						}
					},
					{
						"PM2.5",
						{
							0,
							0
						}
					},
					{
						"PM10",
						{
							0,
							0
						}
					},
					{
						"NO2",
						{
							0,
							0
						}
					},
					{
						"SO2",
						{
							0,
							0
						}
					},
					{
						"CO",
						{
							0,
							0
						}
					}
				}
			}
		};
		coords[points] = details;
		reverse_geocode(points, responses);
	}
}

void APIClient::reverse_geocode(std::pair < double, double > & pct, std::vector < Async::Promise < Http::Response >> & responses) {
	std::string lat = std::to_string(pct.first);
	std::string lng = std::to_string(pct.second);
	std::string url = endpoints["reverse_geocode"] + lat + "+" + lng;
	auto resp = client.get(url).send();
	resp.then(
		[ & ](Http::Response response) {
			auto body = json::parse(response.body())["results"][0]["components"];
			std::string name;
			if (body.find("city") != body.end()) {
				name = body["city"];
			} else if (body.find("town") != body.end()) {
				name = body["town"];
			} else if (body.find("village") != body.end()) {
				name = body["village"];
			} else {
				name = "none";
			}
			coords[pct]["city"] = name;
			if (name == "Bucharest") {
				name = "București";
			}
			if (cities.find(name) == cities.end()) {
				cities[name] = new std::vector < std::pair < double, double >> ;
			}
			cities[name] -> push_back(pct);
			delete & pct;
		},
		[ & ](std::exception_ptr exc) {
			PrintException excPrinter;
			excPrinter(exc);
		}
	);
	responses.push_back(std::move(resp));
}

void APIClient::populate() {
	json waqi_sensors;
	json aerlive_sensors;

	std::vector < Async::Promise < Http::Response >> responses;

	auto resp = client.get(endpoints["waqi"] + endpoints["waqi_full_ro"] + "&" + tokens["waqi"]).send();
	resp.then(
		[ & ](Http::Response response) {
			auto body = response.body();
			waqi_sensors = json::parse(body);
		},
		[ & ](std::exception_ptr exc) {
			PrintException excPrinter;
			excPrinter(exc);
		});
	responses.push_back(std::move(resp));

	auto mime = Http::Mime::MediaType::fromString("application/x-www-form-urlencoded");
	auto header = Http::Header::ContentType(mime);
	resp = client.post("server" + std::to_string(rand() % 9 + 1) + endpoints["aerlive"]).body(tokens["aerlive"]).header < Http::Header::ContentType > (header).send();
	resp.then(
		[ & ](Http::Response response) {
			auto body = response.body();
			aerlive_sensors = json::parse(body);
		},
		[ & ](std::exception_ptr exc) {
			PrintException excPrinter;
			excPrinter(exc);
		});
	responses.push_back(std::move(resp));

	auto sync = Async::whenAll(responses.begin(), responses.end());
	Async::Barrier < std::vector < Http::Response >> barrier(sync);
	barrier.wait_for(std::chrono::seconds(sleep_time));

	responses.clear();

	for (auto sensor: waqi_sensors["data"]) {
		double lat = double(sensor["lat"]);
		double lng = double(sensor["lon"]);
		std::pair < double, double > * points = new std::pair < double, double > (lat, lng);
		new_point( * points, responses);
		service_to_coords["waqi"] -> push_back( * points);
	}

	for (auto sensor: aerlive_sensors["data"]) {
		if (sensor["devices"].empty()) {
			continue;
		}
		double lat = std::stod(std::string(sensor["lat"]));
		double lng = std::stod(std::string(sensor["long"]));
		std::pair < double, double > * points = new std::pair < double, double > (lat, lng);
		new_point( * points, responses);
		service_to_coords["aerlive"] -> push_back( * points);
	}

	sync = Async::whenAll(responses.begin(), responses.end());
	Async::Barrier < std::vector < Http::Response >> barrier2(sync);
	barrier2.wait_for(std::chrono::seconds(sleep_time));
}

void APIClient::fetchWaqi(std::vector < Async::Promise < Http::Response >> & responses) {
	for (auto it: * service_to_coords["waqi"]) {
		std::string url = endpoints["waqi"] + endpoints["waqi_station"] + std::to_string(it.first) + ";" + std::to_string(it.second) + "/?" + tokens["waqi"];
		auto resp = client.get(url).send();
		resp.then(
			[ & ](Http::Response response) {
				auto body = response.body();
				auto data = json::parse(body)["data"];
				auto pct = std::pair < double, double > (double(data["city"]["geo"][0]), double(data["city"]["geo"][1]));
				data = data["iaqi"];
				if (data.contains("pm1")) {
					coords[pct]["mean"]["PM1"][0] = double(coords[pct]["mean"]["PM1"][0]) + double(data["pm1"]["v"]);
					coords[pct]["mean"]["PM1"][1] = int(coords[pct]["mean"]["PM1"][1]) + 1;
				}
				if (data.contains("pm25")) {
					coords[pct]["mean"]["PM2.5"][0] = double(coords[pct]["mean"]["PM2.5"][0]) + double(data["pm25"]["v"]);
					coords[pct]["mean"]["PM2.5"][1] = int(coords[pct]["mean"]["PM2.5"][1]) + 1;
				}
				if (data.contains("pm10")) {
					coords[pct]["mean"]["PM10"][0] = double(coords[pct]["mean"]["PM10"][0]) + double(data["pm10"]["v"]);
					coords[pct]["mean"]["PM10"][1] = int(coords[pct]["mean"]["PM10"][1]) + 1;
				}
				if (data.contains("no2")) {
					coords[pct]["mean"]["NO2"][0] = double(coords[pct]["mean"]["NO2"][0]) + double(data["no2"]["v"]);
					coords[pct]["mean"]["NO2"][1] = int(coords[pct]["mean"]["NO2"][1]) + 1;
				}
				if (data.contains("so2")) {
					coords[pct]["mean"]["SO2"][0] = double(coords[pct]["mean"]["SO2"][0]) + double(data["so2"]["v"]);
					coords[pct]["mean"]["SO2"][1] = int(coords[pct]["mean"]["SO2"][1]) + 1;
				}
				if (data.contains("co")) {
					coords[pct]["mean"]["CO"][0] = double(coords[pct]["mean"]["CO"][0]) + double(data["co"]["v"]);
					coords[pct]["mean"]["CO"][1] = int(coords[pct]["mean"]["CO"][1]) + 1;
				}
			},
			[ & ](std::exception_ptr exc) {
				PrintException excPrinter;
				excPrinter(exc);
			});
		responses.push_back(std::move(resp));
	}
}

void APIClient::fetchAerlive(std::vector < Async::Promise < Http::Response >> & responses) {
	json aerlive_sensors;
	auto mime = Http::Mime::MediaType::fromString("application/x-www-form-urlencoded");
	auto header = Http::Header::ContentType(mime);
	auto resp = client.post("server" + std::to_string(rand() % 9 + 1) + endpoints["aerlive"]).body(tokens["aerlive"]).header < Http::Header::ContentType > (header).send();
	resp.then(
		[ & ](Http::Response response) {
			auto body = response.body();
			aerlive_sensors = json::parse(body);
			for (auto sensor: aerlive_sensors["data"]) {
				double lat = std::stod(std::string(sensor["lat"]));
				double lng = std::stod(std::string(sensor["long"]));
				std::pair < double, double > pct = std::pair < double, double > (lat, lng);
				for (auto data: sensor["devices"]) {
					if (data.contains("pm1") && double(data["pm1"]) != 0.0) {
						coords[pct]["mean"]["PM1"][0] = double(coords[pct]["mean"]["PM1"][0]) + double(data["pm1"]);
						coords[pct]["mean"]["PM1"][1] = int(coords[pct]["mean"]["PM1"][1]) + 1;
					}
					if (data.contains("pm25") && double(data["pm25"]) != 0.0) {
						coords[pct]["mean"]["PM2.5"][0] = double(coords[pct]["mean"]["PM2.5"][0]) + double(data["pm25"]);
						coords[pct]["mean"]["PM2.5"][1] = int(coords[pct]["mean"]["PM2.5"][1]) + 1;
					}
					if (data.contains("pm10") && double(data["pm10"]) != 0.0) {
						coords[pct]["mean"]["PM10"][0] = double(coords[pct]["mean"]["PM10"][0]) + double(data["pm10"]);
						coords[pct]["mean"]["PM10"][1] = int(coords[pct]["mean"]["PM10"][1]) + 1;
					}
					if (data.contains("no2") && double(data["no2"]) != 0.0) {
						coords[pct]["mean"]["NO2"][0] = double(coords[pct]["mean"]["NO2"][0]) + double(data["no2"]);
						coords[pct]["mean"]["NO2"][1] = int(coords[pct]["mean"]["NO2"][1]) + 1;
					}
					if (data.contains("so2") && double(data["so2"]) != 0.0) {
						coords[pct]["mean"]["SO2"][0] = double(coords[pct]["mean"]["SO2"][0]) + double(data["so2"]);
						coords[pct]["mean"]["SO2"][1] = int(coords[pct]["mean"]["SO2"][1]) + 1;
					}
					if (data.contains("co") && double(data["co"]) != 0.0) {
						coords[pct]["mean"]["CO"][0] = double(coords[pct]["mean"]["CO"][0]) + double(data["co"]);
						coords[pct]["mean"]["CO"][1] = int(coords[pct]["mean"]["CO"][1]) + 1;
					}
				}
			}
		},
		[ & ](std::exception_ptr exc) {
			PrintException excPrinter;
			excPrinter(exc);
		});
	responses.push_back(std::move(resp));
}

void APIClient::compute_means() {
	for (auto it = coords.begin(); it != coords.end(); it++) {
		if (double((it -> second)["mean"]["PM1"][1]) > 1) {
			(it -> second)["mean"]["PM1"][0] = double((it -> second)["mean"]["PM1"][0]) / double((it -> second)["mean"]["PM1"][1]);
		}
		if (double((it -> second)["mean"]["PM2.5"][1]) > 1) {
			(it -> second)["mean"]["PM2.5"][0] = double((it -> second)["mean"]["PM2.5"][0]) / double((it -> second)["mean"]["PM2.5"][1]);
		}
		if (double((it -> second)["mean"]["PM10"][1]) > 1) {
			(it -> second)["mean"]["PM10"][0] = double((it -> second)["mean"]["PM10"][0]) / double((it -> second)["mean"]["PM10"][1]);
		}
		if (double((it -> second)["mean"]["NO2"][1]) > 1) {
			(it -> second)["mean"]["NO2"][0] = double((it -> second)["mean"]["NO2"][0]) / double((it -> second)["mean"]["NO2"][1]);
		}
		if (double((it -> second)["mean"]["SO2"][1]) > 1) {
			(it -> second)["mean"]["SO2"][0] = double((it -> second)["mean"]["SO2"][0]) / double((it -> second)["mean"]["SO2"][1]);
		}
		if (double((it -> second)["mean"]["CO"][1]) > 1) {
			(it -> second)["mean"]["CO"][0] = double((it -> second)["mean"]["CO"][0]) / double((it -> second)["mean"]["CO"][1]);
		}
	}
}

void APIClient::notify_webhook(json data) {
	std::cout << data << std::endl;
	auto resp = client.post("http://localhost:9080/webhook").body(data.dump()).send();
	resp.then(
		[ & ](Http::Response response) {
			auto body = response.body();
			std::cout << body << std::endl;
		},
		[ & ](std::exception_ptr exc) {
			PrintException excPrinter;
			excPrinter(exc);
		});
}

json APIClient::jsonize() {
	json print;
	std::vector < std::pair < double, double >> coords_to_json;
	for (auto it = coords.begin(); it != coords.end(); it++) {
		auto values = it -> second;
		auto coords = std::to_string((it -> first).first) + ',' + std::to_string((it -> first).second);
		print[coords] = values;
		coords_to_json.push_back(it -> first);
	}
	auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	print["coords_array"] = coords_to_json;
	print["updated"] = ctime( & current_time);
	return print;
}

void APIClient::fetch() {
	while (true) {
		std::vector < Async::Promise < Http::Response >> responses;
		try {
			fetchWaqi(responses);
			fetchAerlive(responses);
			auto sync = Async::whenAll(responses.begin(), responses.end());
			Async::Barrier < std::vector < Http::Response >> barrier(sync);
			barrier.wait_for(std::chrono::seconds(sleep_time));
			compute_means();
		} catch (...) {
			sleep(sleep_time);
			continue;
		}
		json data = jsonize();
		notify_webhook(data);
		sleep(sleep_time);
	}
}

int main(int argc, char * argv[]) {
	APIClient c;
}