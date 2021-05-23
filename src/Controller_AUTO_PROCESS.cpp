#include "../include/Controller_AUTO_PROCESS.hpp"

Controller_plus_MAIL::Controller_plus_MAIL() {}

string Controller_plus_MAIL::set(std::string name, std::string value) {
	if (name == "e_mail") {
		e_mail.name = name;
		this -> dereturn = value;
		e_mail.value = true;
	}
	return this -> dereturn;
}

Controller_ADD_REMOVE::Controller_ADD_REMOVE() {}

string Controller_ADD_REMOVE::set(std::string name, std::string value) {
	if (name == "e_mail") {
		e_mail.name = name;
		if (value == "remove_mail") {
			e_mail.value = true;

			this -> dereturn = "remove_mail";

			/*

			 curl -X POST  http://localhost:9080/action/e_mail/add_mail
			curl -X POST  http://localhost:9080/action/e_mail/remove_mail
			curl  http://localhost:9080/action/e_mail/

			*/

			return this -> dereturn;

		}
		if (value == "add_mail") {
			e_mail.value = true;

			this -> dereturn = "add_mail";
			return this -> dereturn;
		}
	}
	return 0;
}

Controller_INTEROGARE_MAIL::Controller_INTEROGARE_MAIL() {}

// Setting the value for one of the settings. Hardcoded for the switchoffing option
string Controller_INTEROGARE_MAIL::set(std::string name, std::string value) {
	if (name == "e_mail") {
		e_mail.name = name;
		this -> dereturn = value;
		e_mail.value = true;
	}
	return this -> dereturn;

}

Controller_SWITCH_F::Controller_SWITCH_F() {}

// Setting the value for one of the settings. Hardcoded for the switchoffing option
string Controller_SWITCH_F::set(std::string name, std::string value) {
	if (name == "controlul_ferestrei") {
		controlul_ferestrei.name = name;
		if (value == "manual") {
			controlul_ferestrei.value = true;

			this -> dereturn = "manual";
			return this -> dereturn;

		}
		if (value == "automat") {
			controlul_ferestrei.value = false;

			this -> dereturn = "automat";

			return this -> dereturn;
		}
	}
	return 0;
}

// Getter
string Controller_SWITCH_F::get(std::string name) {
	return this -> dereturn;
}

void Controller_Auto_Process::setAction_fereastra(const Rest::Request & request, Http::ResponseWriter response) {
	// You don't know what the parameter content that you receive is, but you should
	// try to cast it to some data structure. Here, I cast the settingName to string.
	auto switchoff = request.param(":switchoff").as < std::string > ();

	// This is a guard that prevents editing the same value by two concurent threads. 
	Guard guard(sensorLock);

	string val = "";
	if (request.hasParam(":value")) {
		auto value = request.param(":value");
		val = value.as < string > ();
	}

	// Setting the not microwave's setting to value
	string setResponse = ob_on_of.set(switchoff, val);

	// Sending some confirmation or error response.
	if (setResponse == "manual") {
		response.send(Http::Code::Ok, switchoff + " A FOST STETAT IN MODUL [" + val + "]");
	}
	if (setResponse == "automat") {
		response.send(Http::Code::Ok, switchoff + " A FOST STETAT IN MODUL [" + val + "]");
	} else {
		response.send(Http::Code::Not_Found, switchoff + "  '" + val + "' was not a valid value ");
	}

}

// Setting to get the settings value of one of the configurations of the not Microwave
void Controller_Auto_Process::getAction_fereastra(const Rest::Request & request, Http::ResponseWriter response) {
	auto switchoff = request.param(":switchoff").as < std::string > ();

	Guard guard(sensorLock);
	//ob_view_on_of.get_string_value(ob_on_of);
	string valueSetting = ob_view_on_of.get("switchoff");

	if (valueSetting != "") {

		// In this response I also add a couple of headers, describing the server that sent this response, and the way the content is formatted.
		using namespace Http;
		response.headers()
			.add < Header::Server > ("pistache/0.1")
			.add < Header::ContentType > (MIME(Text, Plain));

		//Mecanism1 ob_on_of;
		//ob_view_on_of.get_string_value(ob_on_of);
		string setResponse = ob_view_on_of.get("controlul_ferestrei");
		response.send(Http::Code::Ok, " SENZORUL DE POLUARE A STETAT FEREASTRA IN MODUL [" + setResponse + "]");

	} else {

		// Mecanism1 ob_on_of;
		//ob_view_on_of.get_string_value(ob_on_of);
		string setResponse = ob_view_on_of.get("switchoff");
		response.send(Http::Code::Ok, " SENZORUL DE POLUARE A STETAT FEREASTRA IN MODUL[" + setResponse + "]");

	}
}
// Endpoint ========================================================pachet12
// Endpoint ========================================================pachet12
void Controller_Auto_Process::setAction_addremove(const Rest::Request & request, Http::ResponseWriter response) {
	// You don't know what the parameter content that you receive is, but you should
	// try to cast it to some data structure. Here, I cast the settingName to string.
	auto addremove = request.param(":addremove").as < std::string > ();

	// This is a guard that prevents editing the same value by two concurent threads. 
	Guard guard(sensorLock);

	string val = "";
	if (request.hasParam(":value")) {
		auto value = request.param(":value");
		mail_de_scris = value.as < string > ();
	}

	// Setting the microwave's setting to value
	string setResponse = ad_ob1.set(addremove, mail_de_scris);

	// Sending some confirmation or error response.
	if (setResponse == mail_de_scris) {

		std::ofstream txtFile("email.txt", std::ios_base::app);
		txtFile << mail_de_scris << std::endl;

		txtFile.close();

		response.send(Http::Code::Ok, " A FOST adaugat [" + mail_de_scris + "]");

	} else {

		std::ofstream txtFile("email.txt", std::ios_base::app);

		txtFile << mail_de_scris << std::endl;

		txtFile.close();
		response.send(Http::Code::Not_Found, mail_de_scris + " adaugat");
	}

}

// Setting to get the settings value of one of the configurations of the Microwave
void Controller_Auto_Process::getAction_addremove(const Rest::Request & request, Http::ResponseWriter response) {
	auto addremove = request.param(":addremove").as < std::string > ();

	Guard guard(sensorLock);
	//ob_view_mail.get_string_value(ad_ob1);
	string valueSetting = ob_view_mail.get("addremove");

	if (valueSetting != "") {

		// In this response I also add a couple of headers, describing the server that sent this response, and the way the content is formatted.
		using namespace Http;
		response.headers()
			.add < Header::Server > ("pistache/0.1")
			.add < Header::ContentType > (MIME(Text, Plain));

		//ob_view_mail.get_string_value(ad_ob1);
		string setResponse = ob_view_mail.get("e_mail");
		response.send(Http::Code::Ok, " a fost adaugat mailul [" + setResponse + "]");

	} else {
		//ob_view_mail.get_string_value(ad_ob1);
		string setResponse = ob_view_mail.get("e_mail");
		response.send(Http::Code::Ok, " a fost adaugat mailul [" + setResponse + "]");

	}
}
// Endpoint ========================================================pachet13
// Endpoint ========================================================pachet13
void Controller_Auto_Process::setAction_interogare(const Rest::Request & request, Http::ResponseWriter response) {
	// You don't know what the parameter content that you receive is, but you should
	// try to cast it to some data structure. Here, I cast the settingName to string.
	auto addremove = request.param(":addremove").as < std::string > ();

	// This is a guard that prevents editing the same value by two concurent threads. 
	Guard guard(sensorLock);

	string val = "";
	if (request.hasParam(":value")) {
		auto value = request.param(":value");
		mail_de_scris = value.as < string > ();
	}

	// Setting the microwave's setting to value
	string setResponse = rem_ob1.set(addremove, mail_de_scris);

	// Sending some confirmation or error response.

	if (setResponse == mail_de_scris) {

		bool flag_gasit = false;

		string line;

		fstream newfile;
		newfile.open("email.txt", ios:: in ); //open a file to perform read operation using file object
		if (newfile.is_open()) {

			while (getline(newfile, line)) {

				if (line.find(setResponse) != string::npos) {

					flag_gasit = true;
					response.send(Http::Code::Ok, " mailul [" + line + " ]ESTE ABONAT LA SERVICII ");

				}

			}

		}
		newfile.close();
		if (flag_gasit == false) {

			response.send(Http::Code::Ok, " mailul [" + setResponse + " ]NU ESTE ABONAT LA SERVICII");

		}

	}
}

// Setting to get the settings value of one of the configurations of the Microwave
void Controller_Auto_Process::getAction_interogare(const Rest::Request & request, Http::ResponseWriter response) {
	auto addremove = request.param(":addremove").as < std::string > ();

	Guard guard(sensorLock);
	//view_rem_ob.get_string_value(rem_ob1);
	string setResponse = view_rem_ob.get("e_mail");

	// In this response I also add a couple of headers, describing the server that sent this response, and the way the content is formatted.
	using namespace Http;
	response.headers()
		.add < Header::Server > ("pistache/0.1")
		.add < Header::ContentType > (MIME(Text, Plain));

	bool flag_gasit = false;

	string line;

	fstream newfile;
	newfile.open("email.txt", ios:: in ); //open a file to perform read operation using file object
	if (newfile.is_open()) {

		while (getline(newfile, line)) {

			if (line.find(setResponse) != string::npos) {

				flag_gasit = true;
				response.send(Http::Code::Ok, " mailul [" + line + " ]ESTE ABONAT LA SERVICII ");

			}

		}

	}
	newfile.close();
	if (flag_gasit == false) {

		response.send(Http::Code::Ok, " mailul [" + setResponse + " ]NU ESTE ABONAT LA SERVICII");

	}

}

// Endpoint ========================================================pachet14
// Endpoint ========================================================pachet14       
//Endpoint to configure one of the Sensor's actions.
void Controller_Auto_Process::setAction(const Rest::Request & request, Http::ResponseWriter response) {

	auto actionName = request.param(":actionName").as < std::string > ();

	//This is a guard that prevents editing the same value by 2 concurent threads.
	Guard guard(sensorLock);

	// Set the window value. 
	/*
	int setResponse = sensor.action(actionName);

	//Sending some confirmation or error response.
	if (setResponse == 1) {
	  if (actionName == "shut_down")
		response.send(Http::Code::Ok, "The window is closed now!");

	} else {
	  response.send(Http::Code::Not_Found, actionName);
	}
	*/
}