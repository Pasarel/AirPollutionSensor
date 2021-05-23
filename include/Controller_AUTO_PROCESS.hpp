#pragma once
#include <iostream>
#include <fstream>
#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>
#include "Viewer_AUTO_PROCESS.hpp"

using namespace std;
using namespace Pistache;

class Controller_plus_MAIL {
public:
	string dereturn;
	string mail_de_scris;
	explicit Controller_plus_MAIL();

	// Setting the value for one of the settings. Hardcoded for the switchoffing option
	string set(std::string name, std::string value);

private:
	struct boolSetting {
		std::string name;
		bool value;
	}
	e_mail;
};

class Controller_ADD_REMOVE {
public:
	string dereturn;
	explicit Controller_ADD_REMOVE();
	string set(std::string name, std::string value);

private:
	// Defining and instantiating settings.
	struct boolSetting {
		std::string name;
		bool value;
	}
	e_mail;
};

class Controller_INTEROGARE_MAIL {
public:
	string dereturn;
	string mail_de_scris;
	explicit Controller_INTEROGARE_MAIL();

	// Setting the value for one of the settings. Hardcoded for the switchoffing option
	string set(std::string name, std::string value);

private:
	// Defining and instantiating settings.
	struct boolSetting {
			std::string name;
			bool value;
		}
	e_mail;
};

class Controller_SWITCH_F {
public:
	string dereturn;
	explicit Controller_SWITCH_F();

	// Setting the value for one of the settings. Hardcoded for the switchoffing option
	string set(std::string name, std::string value);
	// Getter
	string get(std::string name);

private:
		// Defining and instantiating settings.
	struct boolSetting {
			std::string name;
			bool value;
		}
	controlul_ferestrei;
};

class Controller_Auto_Process {
	// Create the lock which prevents concurrent editing og the same variable 
	string mail_de_scris;
	using Lock = std::mutex;
	using Guard = std::lock_guard < Lock > ;
	Lock sensorLock;

	Controller_SWITCH_F ob_on_of;
	Controller_plus_MAIL ad_ob1;
	Controller_INTEROGARE_MAIL rem_ob1;
	Viewer_ADD_MAIL ob_view_mail;
	Viewer_SWITCH_AUTO_MANUAL ob_view_on_of;
	Viewer_ADD_REMOVE_USER view_rem_ob;
public:
	void setAction_fereastra(const Rest::Request & request, Http::ResponseWriter response);
	void getAction_fereastra(const Rest::Request & request, Http::ResponseWriter response);
	void setAction_addremove(const Rest::Request & request, Http::ResponseWriter response);
	void getAction_addremove(const Rest::Request & request, Http::ResponseWriter response);
	void setAction_interogare(const Rest::Request & request, Http::ResponseWriter response);
	void getAction_interogare(const Rest::Request & request, Http::ResponseWriter response);
	void setAction(const Rest::Request & request, Http::ResponseWriter response);
};