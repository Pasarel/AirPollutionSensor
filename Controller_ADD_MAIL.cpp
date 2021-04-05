
using namespace std;

#include <iostream>
#include <fstream>
using namespace std;


    class Controller_plus_MAIL {
    public:
        string dereturn;
        string mail_de_scris;
        explicit Controller_plus_MAIL(){ }

        // Setting the value for one of the settings. Hardcoded for the switchoffing option
        string set(std::string name, std::string value){
            if(name == "e_mail"){
                e_mail.name = name;
                         this->dereturn=value;
                    e_mail.value =  true;


                          


// ifstream MyReadFile("filename.txt");

// // Use a while loop together with the getline() function to read the file line by line
// while (getline (MyReadFile, myText)) {
//   // Output the text from the file
//   cout << myText;
// }

// // Close the file
// MyReadFile.close(); 





                }
                                    return this->dereturn;

          
}
        // Getter
 /*       string get(std::string name){
            if (name == "e_mail"){
                return (this->dereturn);
            }
            else{
                return this->dereturn;
            }
        }*/

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
