   
using namespace std;
using namespace Pistache;
  

    class Controller_ADD_REMOVE {
    public:
        string dereturn;
        explicit Controller_ADD_REMOVE(){ }

        // Setting the value for one of the settings. Hardcoded for the switchoffing option
        string set(std::string name, std::string value){
            if(name == "e_mail"){
                e_mail.name = name;
                if(value == "remove_mail"){
                    e_mail.value =  true;

          this->dereturn = "remove_mail";


/*

 curl -X POST  http://localhost:9080/action/e_mail/add_mail
curl -X POST  http://localhost:9080/action/e_mail/remove_mail
curl  http://localhost:9080/action/e_mail/

*/
          
                 return this->dereturn;

                   
                }
                if(value == "add_mail"){
                    e_mail.value = true;

   this->dereturn="add_mail";
                    return this->dereturn;
                }
            }
            return 0;
        }

/*        // Getter
        string get(std::string name){
            if (name == "e_mail"){
                return (this->dereturn);
            }
            else{



 curl -X POST  http://localhost:9080/action/e_mail/add_mail
curl -X POST  http://localhost:9080/action/e_mail/remove_mail
curl  http://localhost:9080/action/e_mail/



                return this->dereturn;
            }
        }
*/
    private:
        // Defining and instantiating settings.
        struct boolSetting{
            std::string name;
            bool value;
        }e_mail;
    };



/*
// writing on a text file
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  ofstream myfile ("example.txt");
  if (myfile.is_open())
  {
    myfile << "This is a line.\n";
    myfile << "This is another line.\n";
    myfile.close();
  }
  else cout << "Unable to open file";
  return 0;
}
*/
