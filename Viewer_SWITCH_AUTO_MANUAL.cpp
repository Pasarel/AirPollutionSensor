#include <iostream>
//#include "Controller_SWITCH_AUTO_MANUAL.cpp"
using namespace std;

    class Viewer_SWITCH_AUTO_MANUAL {
    public:
        string view;
 
        //void get_string_value(Controller_SWITCH_F ob)
	//{
	//	this->view = ob.dereturn;
	//}
		// Getter
        string get(std::string name){
            if (name == "controlul_ferestrei"){
                return (this->view);
            }
            else{
                return this->view;
            }
        }
    };

