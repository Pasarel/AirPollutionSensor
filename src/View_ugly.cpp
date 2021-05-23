//#include "Controller_ADD_MAIL.cpp"
#include <iostream>
 //#include <fstream>

using namespace std;

class Viewer_ADD_MAIL {
  public:

    string view;
  //Getter
  //void get_string_value(Controller_plus_MAIL ob)
  //{
  //    this->view = ob.dereturn;
  //}
  string get(std::string name) {

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
};

class Viewer_ADD_REMOVE_USER {
  public:
    string view;
  // Getter

  //void get_string_value(Controller_ADD_REMOVE ob)
  //{
  //    this->view = ob.dereturn;
  //}

  string get(std::string name) {
    if (name == "e_mail") {
      return (this -> view);
    } else {
      return this -> view;
    }
  }
};

class Viewer_SWITCH_AUTO_MANUAL {
  public:
    string view;

  //void get_string_value(Controller_SWITCH_F ob)
  //{
  //    this->view = ob.dereturn;
  //}
  // Getter
  string get(std::string name) {
    if (name == "controlul_ferestrei") {
      return (this -> view);
    } else {
      return this -> view;
    }
  }
};