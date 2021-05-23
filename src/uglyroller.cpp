#include <iostream>

#include <fstream>

using namespace std;

class Controller_plus_MAIL {
  public:
  string dereturn;
  string mail_de_scris;
  explicit Controller_plus_MAIL() {}

  // Setting the value for one of the settings. Hardcoded for the switchoffing option
  string set(std::string name, std::string value) {
    if (name == "e_mail") {
      e_mail.name = name;
      this -> dereturn = value;
      e_mail.value = true;
    }
    return this -> dereturn;

  }

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
  explicit Controller_ADD_REMOVE() {}

  string set(std::string name, std::string value) {
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
  explicit Controller_INTEROGARE_MAIL() {}

  // Setting the value for one of the settings. Hardcoded for the switchoffing option
  string set(std::string name, std::string value) {
    if (name == "e_mail") {
      e_mail.name = name;
      this -> dereturn = value;
      e_mail.value = true;
    }
    return this -> dereturn;

  }

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
  explicit Controller_SWITCH_F() {}

  // Setting the value for one of the settings. Hardcoded for the switchoffing option
  string set(std::string name, std::string value) {
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
  string get(std::string name) {
    return this -> dereturn;
  }

  private:
    // Defining and instantiating settings.
    struct boolSetting {
      std::string name;
      bool value;
    }
  controlul_ferestrei;
};