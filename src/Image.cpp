#include "../include/Image.hpp"

void hexchar(unsigned char c, unsigned char &hex1, unsigned char &hex2)
{
    hex1 = c / 16;
    hex2 = c % 16;
    hex1 += hex1 <= 9 ? '0' : 'a' - 10;
    hex2 += hex2 <= 9 ? '0' : 'a' - 10;
}

std::string urlencode(std::string s)
{
    const char *str = s.c_str();
    std::vector<char> v(s.size());
    v.clear();
    for (size_t i = 0, l = s.size(); i < l; i++)
    {
        char c = str[i];
        if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '-' || c == '_' || c == '.' || c == '!' || c == '~' ||
            c == '*' || c == '\'' || c == '(' || c == ')')
        {
            v.push_back(c);
        }
        else if (c == ' ')
        {
            v.push_back('+');
        }
        else
        {
            v.push_back('%');
            unsigned char d1, d2;
            hexchar(c, d1, d2);
            v.push_back(d1);
            v.push_back(d2);
        }
    }

    return std::string(v.cbegin(), v.cend());
}

ojson ImageClient::readFromJson()
{
	ojson data;
	const char* filename = "data.json";
	boost::interprocess::file_lock flock(filename);
	flock.lock();
	std::ifstream reader(filename);
	reader >> data;
	reader.close();
	flock.unlock();
	return data;
}
std::string ImageClient::parseCoords(ojson coord, double& lat, double& lng)
{
	std::string str = coord.dump();
	str = str.substr(1, str.length() - 2);
	auto comma = str.find(",");
	lat = std::stod(str.substr(0, comma));
	lng = std::stod(str.substr(comma + 1, str.length()));
	str = std::to_string(lat) + ',' + std::to_string(lng);
	return str;
}

std::string ImageClient::getMarkerColor(bool open)
{
	if (open) {
		return "#008000";
	}
	return "#000000";
}

std::string ImageClient::getSensorColor(ojson sensor)
{
	double pm10 = sensor["mean"]["PM10"][0];
	if (pm10 == 0) {
		return "#D3D3D3";
	}
	if (pm10 < 25) {
		return "#008000";
	}
	if (pm10 < 250) {
		return "#FFFF00";
	}
	if (pm10 < 400) {
		return "#FF0000";
	}
	return "#330000";
}

void ImageClient::populateGeoJson(double w_lat, double w_lng, bool open)
{
	ojson data = readFromJson();
	std::vector<ojson> features;

	ojson polygon;
	double lat, lng;
	std::string parsed;
	int num_sensors = 0;
	for (auto coord : data["coords_array"]) {
		parsed = parseCoords(coord, lat, lng);
		if (num_sensors == 10) {
			break;
		}
		if (lat > w_lat + 0.3 || lat < w_lat - 0.3 || lng > w_lng + 0.3 || lng < w_lng - 0.3) {
			continue;
		}
		polygon = {
			{ "type", "Feature" },
			{ "geometry",
				{
					{ "type", "Polygon" },
					{ "coordinates",
						{ { { lng - 0.075, lat - 0.05625 },
							{ lng + 0.075, lat - 0.05625 },
							{ lng + 0.075, lat + 0.05625 },
							{ lng - 0.075, lat + 0.05625 },
							{ lng - 0.075, lat - 0.05625 } } } },
				} },
			{ "properties",
				{ { "fill", getSensorColor(data[parsed]) }, { "fill-opacity", 0.5 } } }
		};
		features.push_back(polygon);
		num_sensors++;
	}

	ojson marker = {
		{ "type", "Feature" },
		{ "geometry", { { "type", "Point" }, { "coordinates", { w_lng, w_lat } } } },
		{ "properties",
			{
				{ "marker-color", getMarkerColor(open) },
			} }
	};
	features.push_back(marker);
	geojson["features"] = features;
}
std::string ImageClient::getGeoJson()
{
	std::regex re("([0-9]+\\.[0-9]{2})([0-9]+)");
	std::string js = geojson.dump();
	return std::regex_replace(js, re, "$1");
}

ImageClient::ImageClient()
{
	geojson["type"] = "FeatureCollection";
	geojson["features"] = {};
	zoom = 9;
}

void ImageClient::zoom_out()
{
	if (zoom > 0)
		zoom -= 1;
}
void ImageClient::zoom_in()
{
	if (zoom < 20)
		zoom += 1;
}
void ImageClient::reset_zoom() { zoom = 9; }

std::string ImageClient::getImage(double w_lat, double w_lng, bool open)
{
	populateGeoJson(w_lat, w_lng, open);
	std::string ret = getGeoJson();
	geojson["features"].clear();
	std::string uri = "/styles/v1/mapbox/streets-v11/static/geojson(" + urlencode(ret) + ")/" + std::to_string(w_lng) + "," + std::to_string(w_lat) + "," + std::to_string(zoom) + ",0/"
																																												   "300x300?access_token=pk."
																																												   "eyJ1IjoicGFzYXJlbCIsImEiOiJja3AzNWdpOGowN3Q1MnByMnp1bWRxZn"
																																												   "dpIn0.NYrA0fRjCx8UnBEP7ccsKg";
	return c.b64_get("https://api.mapbox.com", uri);
}