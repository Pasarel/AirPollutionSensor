   
using namespace std;
using namespace Pistache;
  

    class Controller_SWITCH_F {
    public:
        string dereturn;
        explicit Controller_SWITCH_F(){ }

        // Setting the value for one of the settings. Hardcoded for the switchoffing option
        string set(std::string name, std::string value){
            if(name == "controlul_ferestrei"){
                controlul_ferestrei.name = name;
                if(value == "manual"){
                    controlul_ferestrei.value =  true;

          this->dereturn = "manual";
                 return this->dereturn;

                   
                }
                if(value == "automat"){
                    controlul_ferestrei.value = false;

   this->dereturn="automat";
   
                    return this->dereturn;
                }
            }
            return 0;
        }

        // Getter
        string get(std::string name){
            if (name == "controlul_ferestrei"){
                return (this->dereturn);
            }
            else{
                return this->dereturn;
            }
        }

    private:
        // Defining and instantiating settings.
        struct boolSetting{
            std::string name;
            bool value;
        }controlul_ferestrei;
    };
