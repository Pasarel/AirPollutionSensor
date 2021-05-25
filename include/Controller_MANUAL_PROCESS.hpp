#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>
#include <boost/interprocess/sync/file_lock.hpp>
#include "Viewer_MANUAL_PROCESS.hpp"
#include "Window_Model.hpp"
#include "Image.hpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
using namespace Pistache;

class Controller_Manual_Process {
  const double geoLatitude = 47.430603;
  const double geoLongitude = 26.126609;
  vector<Window> windowsArray;
  Viewer_Manual_Process view;
  ImageClient img;

  void readFromJSON(json & j);
  string closestSensor(pair < double, double > coords);
  pair < double, double > getCoordsFromRequest(const Rest::Request & request);

public:
  Controller_Manual_Process();
  void openWindow(const Rest::Request & request, Http::ResponseWriter response);
  void closeWindow(const Rest::Request & request, Http::ResponseWriter response);
  // check if the window is open or not
  void checkWindow(const Rest::Request & request, Http::ResponseWriter response);
  void changeWindowCoords(const Rest::Request & request, Http::ResponseWriter response);
  void getAirQuality(const Rest::Request & request, Http::ResponseWriter response);
  void getPM25(const Rest::Request & request, Http::ResponseWriter response);

  void imgZoomIn(const Rest::Request & request, Http::ResponseWriter response);
  void imgZoomOut(const Rest::Request & request, Http::ResponseWriter response);
  void resetZoom(const Rest::Request & request, Http::ResponseWriter response);
  void getImage(const Rest::Request & request, Http::ResponseWriter response);
};