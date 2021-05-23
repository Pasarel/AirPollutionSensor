#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <boost/interprocess/sync/file_lock.hpp>
#include "Viewer_MANUAL_PROCESS.cpp"
#include "Window_Model.cpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
using namespace Pistache;

class Controller_Manual_Process {
  const double geoLatitude = 47.430603;
  const double geoLongitude = 26.126609;
  vector<Window> windowsArray;
  Viewer_Manual_Process view;

  void readFromJSON(json & j);
  string closestSensor(pair < double, double > coords);
  pair < double, double > getCoordsFromRequest(const Rest::Request & request);
  void closeWindow(const Rest::Request & request, Http::ResponseWriter response);
  void checkWindow(const Rest::Request & request, Http::ResponseWriter response);
  void getAirQuality(const Rest::Request & request, Http::ResponseWriter response);
  void getPM25(const Rest::Request & request, Http::ResponseWriter response);
public:
  Controller_Manual_Process();
  void openWindow(const Rest::Request & request, Http::ResponseWriter response);
  void closeWindow(const Rest::Request & request, Http::ResponseWriter response);
  // check if the window is open or not
  void checkWindow(const Rest::Request & request, Http::ResponseWriter response);
  void getAirQuality(const Rest::Request & request, Http::ResponseWriter response);
  void getPM25(const Rest::Request & request, Http::ResponseWriter response);
};