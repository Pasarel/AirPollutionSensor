
//#include "Controller_ADD_REMOVE_USER.cpp"
#include <iostream>
using namespace std;

    class Viewer_ADD_REMOVE_USER {
    public:
        string view;
        // Getter
		
	//void get_string_value(Controller_ADD_REMOVE ob)
	//{
	//	this->view = ob.dereturn;
	//}
		
        string get(std::string name){
            if (name == "e_mail"){
                return (this->view);
            }
            else{
                return this->view;
            }
        }
	};
