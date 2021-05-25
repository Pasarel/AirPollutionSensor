#pragma once
#include "APIClient.hpp"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <regex>
#include <boost/interprocess/sync/file_lock.hpp>

using namespace Poco::Net;
using namespace Poco;
using ojson = nlohmann::ordered_json;

class ImageClient {
private:
	APIClient c;
	ojson geojson;
	double zoom;

	ojson readFromJson();
	std::string parseCoords(ojson coord, double& lat, double& lng);
	std::string getMarkerColor(bool open);
	std::string getSensorColor(ojson sensor);
	void populateGeoJson(double w_lat, double w_lng, bool open);
	std::string getGeoJson();

public:
	ImageClient();
	void zoom_out();
	void zoom_in();
	void reset_zoom();
	std::string getImage(double w_lat, double w_lng, bool open);
};

void hexchar(unsigned char c, unsigned char &hex1, unsigned char &hex2);
std::string urlencode(std::string s);