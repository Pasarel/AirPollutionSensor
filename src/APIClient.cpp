#include "../include/APIClient.hpp"

// output fisier: harta romania + senzori + ferestre

APIClient::APIClient() {
	srand(time(NULL));
	service_to_coords["waqi"] = new std::vector < std::pair < double, double >> ;
	service_to_coords["aerlive"] = new std::vector < std::pair < double, double >> ;
	try {
		populate();
	}
	catch (...) {
		std::cerr << "failed to populate" << std::endl;
		return;
	}
	fetch();
}

APIClient::~APIClient() {
	for (auto city = cities.begin(); city != cities.end(); city++) {
		delete city -> second;
	}
	for (auto stc = service_to_coords.begin(); stc != service_to_coords.end(); stc++) {
		delete stc -> second;
	}
}

std::string APIClient::send_request(std::string method, std::string url, std::string body, std::map<std::string,std::string> headers, bool SSL) {
	try
	{
		URI uri(url);

		HTTPClientSession* session;
		if (SSL) {
			session = new HTTPSClientSession(uri.getHost(), uri.getPort());
		}
		else {
			session = new HTTPClientSession(uri.getHost(), uri.getPort());
		}
		
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";
		
		if (method == "POST") {
			HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
			req.setContentType("application/x-www-form-urlencoded");
			
			for(std::map<std::string,std::string>::iterator it = headers.begin();
				it != headers.end(); it++) {
				req.set(it->first, it->second);
			}
		
			req.setContentLength( body.length() );
			std::ostream& os = session->sendRequest(req);
			os << body;  // sends the body
		}
		else if (method == "GET") {
			HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
			std::ostream& os = session->sendRequest(req);
		}
		
		HTTPResponse res;
		
		std::istream &is = session->receiveResponse(res);
		std::stringstream ss;
		StreamCopier::copyStream(is, ss);

		delete session;
		return ss.str();
	}
	catch (Exception &ex)
	{
		std::cerr << ex.displayText() << std::endl;
		return "";
	}
}

std::string APIClient::send_request(std::string method, std::string url, std::string body, std::map<std::string,std::string> headers) {
	return send_request(method, url, body, headers, true);
}

std::string APIClient::send_request(std::string method, std::string url) {
	std::string body = "placeholder";
	std::map<std::string,std::string> headers;
	return send_request(method, url, body, headers);
}

void APIClient::clear_points() {
	for (auto it = coords.begin(); it != coords.end(); ++it) {
		try {
			city = it->second["city"];
		}
		catch (...) {
			city = "none";
		}
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
				},
				"city",
				{
					city
				}
			}
		};
		it->second = details;
	}
}

void APIClient::new_point(std::pair < double, double > & points) {
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
		reverse_geocode(points);
	}
}

void APIClient::reverse_geocode(std::pair < double, double > & pct) {
	std::string lat = std::to_string(pct.first);
	std::string lng = std::to_string(pct.second);
	std::string url = endpoints["reverse_geocode"] + lat + "+" + lng;
	auto resp = send_request("GET", url);// client.get(url).send();
	
	auto body = json::parse(resp)["results"][0]["components"];
	std::string name = "none";
	if (body.find("city") != body.end()) {
		name = body["city"];
	} else if (body.find("town") != body.end()) {
		name = body["town"];
	} else if (body.find("village") != body.end()) {
		name = body["village"];
	} else {
		name = "none";
	}
	if (name == "Bucharest") {
		name = "București";
	}
	coords[pct]["city"] = name;
	if (cities.find(name) == cities.end()) {
		cities[name] = new std::vector < std::pair < double, double >> ;
	}
	cities[name] -> push_back(pct);
}

void APIClient::populate() {
	json waqi_sensors;
	json aerlive_sensors;

	auto body = send_request("GET", "https://server" + std::to_string(rand() % 9 + 1) + "." + endpoints["waqi"] + endpoints["waqi_full_ro"] + "&" + tokens["waqi"]);
	waqi_sensors = json::parse(body);

	for (auto sensor: waqi_sensors["data"]) {
		double lat = double(sensor["lat"]);
		double lng = double(sensor["lon"]);
		std::pair < double, double > * points = new std::pair < double, double > (lat, lng);
		new_point( * points);
		service_to_coords["waqi"] -> push_back( * points);
	}

	std::string key = "key=d09668ea-def5-44ea-8c77-ae32e9fa5572";
	std::map<std::string,std::string> headers;
	headers["Content-Type"] = "application/x-www-form-urlencoded";
	body = send_request("POST", "https://apps.roiot.ro/aerlive/api/data.php", key, headers);
	aerlive_sensors = json::parse(body);

	for (auto sensor: aerlive_sensors["data"]) {
		if (sensor["devices"].empty()) {
			continue;
		}
		double lat = std::stod(std::string(sensor["lat"]));
		double lng = std::stod(std::string(sensor["long"]));
		std::pair < double, double > * points = new std::pair < double, double > (lat, lng);
		new_point( * points);
		service_to_coords["aerlive"] -> push_back( * points);
	}
}

void APIClient::fetchWaqi() {
	for (auto it: * service_to_coords["waqi"]) {
		std::string url = "https://server" + std::to_string(rand() % 9 + 1) + "." + endpoints["waqi"] + endpoints["waqi_station"] + std::to_string(it.first) + ";" + std::to_string(it.second) + "/?" + tokens["waqi"];
		auto body = send_request("GET", url); // client.get(url).send();

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
	}
}

void APIClient::fetchAerlive() {
	json aerlive_sensors;
	std::string body = "key=d09668ea-def5-44ea-8c77-ae32e9fa5572";
	std::map<std::string,std::string> headers;
	headers["Content-Type"] = "application/x-www-form-urlencoded";
	body = send_request("POST", "https://apps.roiot.ro/aerlive/api/data.php", body, headers);
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
	std::map<std::string,std::string> headers;
	headers["Test"] = "Fest";
	auto body = send_request("POST", "http://localhost:9080/webhook", data.dump(), headers, false);
	std::cout << body << std::endl;
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
		try {
			fetchWaqi();
			fetchAerlive();
			compute_means();
			json data = jsonize();
			notify_webhook(data);
			sleep(sleep_time);
		} catch (...) {
			std::cerr << "got an error" << std::endl;
			clear_points();
			sleep(sleep_time);
			continue;
		}
	}
}

int main(int argc, char * argv[]) {
	APIClient c;
}