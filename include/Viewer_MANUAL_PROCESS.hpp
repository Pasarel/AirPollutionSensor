#pragma once
#include <iostream>

using namespace std;

class Viewer_Manual_Process {

public:
  explicit Viewer_Manual_Process();
  // view for "check window action"
  string checkWindow(string s);

  //view for "close window action" 
  string closeWindow(string s);

  //view for "opene window action"
  string openWindow(string s);

  //view for showing the air quality
  string getAirQuality(string s);

  //view for showing the pm2.5 value
  string getPM25(string s);

};