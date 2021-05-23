#include "../include/Viewer_AUTO_PROCESS.hpp"

string Viewer_ADD_MAIL::get(std::string name) {
	if (name == "e_mail") {
		return (this -> view);
	} else {

		/*

		curl -X POST  http://localhost:9080/action/e_mail/add_mail
		curl -X POST  http://localhost:9080/action/e_mail/remove_mail
		curl  http://localhost:9080/action/e_mail/

		*/
		return this -> view;
	}
}

string Viewer_ADD_REMOVE_USER::get(std::string name) {
	if (name == "e_mail") {
		return (this -> view);
	} else {
		return this -> view;
	}
}

string Viewer_SWITCH_AUTO_MANUAL::get(std::string name) {
	if (name == "controlul_ferestrei") {
		return (this -> view);
	} else {
		return this -> view;
	}
}