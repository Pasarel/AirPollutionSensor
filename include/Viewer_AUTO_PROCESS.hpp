#pragma once
#include <iostream>

using namespace std;

class Viewer_ADD_MAIL {
public:
  string view;
  string get(std::string name);
};

class Viewer_ADD_REMOVE_USER {
public:
  string view;
  string get(std::string name);
};

class Viewer_SWITCH_AUTO_MANUAL {
public:
  string view;
  string get(std::string name);
};