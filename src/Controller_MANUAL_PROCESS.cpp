#include "../include/Controller_MANUAL_PROCESS.hpp"

void Controller_Manual_Process::readFromJSON(json & j) {
	const char * filename = "data.json";
	boost::interprocess::file_lock flock(filename);
	flock.lock();
	ifstream reader(filename);
	reader >> j;
	reader.close();
	flock.unlock();
}

string Controller_Manual_Process::closestSensor(pair < double, double > coords) {
	double geoLatitude = coords.first;
	double geoLongitude = coords.second;
	json j;
	readFromJSON(j);

	if (j.contains("coords_array")) {
		double coord[2];
		double longitude, minLongitude = 100.00;
		double latitude, minLatitude = 100.00;
		int nCoords = 0;
		double diffMinim = 1001.001;

		//iterate through "coords_array" from json, the array thar contains 
		// the coords for every pollution sensor
		while (nCoords < j["coords_array"].size()) {
			latitude = j["coords_array"][nCoords][0];
			longitude = j["coords_array"][nCoords][1];

			if (abs(latitude - geoLatitude) + abs(longitude - geoLongitude) < diffMinim) {

				diffMinim = abs(latitude - geoLatitude) + abs(longitude - geoLongitude);
				minLatitude = latitude;
				minLongitude = longitude;

			}

			nCoords++;
		}

		string coordsKey = to_string(minLatitude);
		string coords1 = to_string(minLongitude);
		coordsKey = coordsKey + "," + coords1;
		return coordsKey;
	}
	return "Not found";
}

pair < double, double > Controller_Manual_Process::getCoordsFromRequest(const Rest::Request & request) {
	if (!request.hasParam(":coords")) {
		return pair < double, double > (this -> geoLatitude, this -> geoLongitude);
	}
	string param = request.param(":coords").as < string > ();
	if (regex_match(param, regex("^(-?)(0|([1-9][0-9]*))(\\.[0-9]+)?,(-?)(0|([1-9][0-9]*))(\\.[0-9]+)?$"))) {
		auto comma = param.find(",");
		return pair < double, double > (stod(param.substr(0, comma)), stod(param.substr(comma + 1, param.length())));
	} else {
		throw (string("Coordonatele trebuie introduse în formatul: latitudine,longitudine"));
	}
}

Controller_Manual_Process::Controller_Manual_Process() {
	windowsArray = vector < Window > (101);
}

void Controller_Manual_Process::openWindow(const Rest::Request & request, Http::ResponseWriter response) {
	Viewer_Manual_Process v;
	json message;

	if (request.hasParam(":id")) {
		auto id = request.param(":id").as < int > ();
		if (id < 1 or id >= 100) {
			message["code"] = 400;
			message["state"] = "id invalid";
		} else {
			windowsArray[id].setState(true);
			message["code"] = 200;
			message["state"] = "deschis";
		}
	} else {
		message["code"] = 400;
		message["state"] = "Nu s-a transmis id-ul geamului!";
	}
	response.send(Http::Code::Ok, v.checkWindow(to_string(message)));
}

void Controller_Manual_Process::closeWindow(const Rest::Request & request, Http::ResponseWriter response) {

	Viewer_Manual_Process v;
	json message;

	if (request.hasParam(":id")) {
		auto id = request.param(":id").as < int > ();
		if (id < 1 or id >= 100) {
			message["code"] = 400;
			message["state"] = "id invalid";
		} else {
			windowsArray[id].setState(false);
			message["code"] = 200;
			message["state"] = "inchis";
		}
	} else {
		message["code"] = 400;
		message["state"] = "Nu s-a transmis id-ul geamului!";
	}
	response.send(Http::Code::Ok, v.checkWindow(to_string(message)));
}
// check if the window is open or not
void Controller_Manual_Process::checkWindow(const Rest::Request & request, Http::ResponseWriter response) {
	Viewer_Manual_Process v;

	json message;
	string id = "";
	if (request.hasParam(":id")) {
		auto id = request.param(":id").as < int > ();
		if (id >= 100) {
			message["code"] = 400;
			message["state"] = "id invalid";
		} else {
			message["code"] = 200;
			message["state"] = (windowsArray[id].getState() == true) ? "deschis" : "inchis";
		}
	} else {
		message["code"] = 400;
		message["state"] = "Nu s-a transmis id-ul geamului!";
	}
	response.send(Http::Code::Ok, v.checkWindow(to_string(message)));
}

//get the air quality value registered by the colsest pollution sensor
void Controller_Manual_Process::getAirQuality(const Rest::Request & request, Http::ResponseWriter response) {
	Viewer_Manual_Process view;

	string message = "";
	try {
		pair < double, double > coords = getCoordsFromRequest(request);
		json j;
		readFromJSON(j);

		message = "Senzorul de poluare din zona dumneavoastra a inregistrat urmatoarele date despre aer : ";
		message += to_string(j[closestSensor(coords)]["mean"]);
		double airQualityValue;
		string closestsensor = closestSensor(coords);
		airQualityValue = j[closestsensor]["mean"]["CO"][0];
		airQualityValue = j[closestsensor]["mean"]["CO"][1];
		message += "\n" + closestsensor + " (" + to_string(j[closestsensor]["city"]) + ")";
		if (airQualityValue <= 0.1) {
			message += "\n\nAnalizand datele de mai sus, se poate spune ca aerul la momentul actual din zona dvs. nu este poluat!";
		} else {
			message += "\n\n In urma analizarii datelor de mai sus, cu parere de rau, va anuntam ca aerul din zona dvs. este poluat la momentul actual. Va sugeram sa evitati iesirile in aer liber si geamul sa fie inchis.";

		}
		response.send(Http::Code::Ok, view.getAirQuality(message));
	} catch (string bad_params) {
		response.send(Http::Code::Ok, view.getAirQuality(bad_params));
	}
}

//get the PM2.5 value from the closest pollution sensor
void Controller_Manual_Process::getPM25(const Rest::Request & request, Http::ResponseWriter response) {
	json j;
	readFromJSON(j);

	try {
		pair < double, double > coords = getCoordsFromRequest(request);
		string message = "Valoarea pm2.5 inregistrata in zona dvs. este: ";
		message += to_string(j[closestSensor(coords)]["mean"]["PM2.5"]);
		response.send(Http::Code::Ok, view.getPM25(message));
	} catch (string bad_params) {
		response.send(Http::Code::Ok, view.getAirQuality(bad_params));
	}
}