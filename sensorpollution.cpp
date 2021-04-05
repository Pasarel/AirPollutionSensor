#include <algorithm>

#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/peer.h>
#include <pistache/http_headers.h>
#include <pistache/cookie.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/common.h>
#include "Controller_SWITCH_AUTO_MANUAL.cpp"
#include "Controller_ADD_MAIL.cpp"
#include "Controller_INTEROGARE_MAIL.cpp"
#include "Viewer_ADD_MAIL.cpp"
#include "Viewer_SWITCH_AUTO_MANUAL.cpp"
#include "Viewer_ADD_REMOVE_USER.cpp"
#include <signal.h>
string mail_de_scris;
using namespace std;
using namespace Pistache;

//Definition of the manualProcesses of the sensor class

class MPSensorEndPoint {
    public:
        explicit MPSensorEndPoint(Address addr)
            : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
        { }

        //Initialization of the server. Additional options can be provided here
        void init(size_t thr = 2) {

            auto  opts = Http::Endpoint::options().threads(static_cast<int>(thr));
            httpEndpoint->init(opts);

            //Server routes are loaded up
            setupRoutes();
        }

        //Server is started threaded
        void start() {
            httpEndpoint->setHandler(router.handler());
            httpEndpoint->serveThreaded();

        }

        //When signaled server shuts down
        void stop() {
            httpEndpoint->shutdown();
        }
    
    private:
        void setupRoutes(){
            using namespace Rest;
            //Defining various endpoints
            // Generally say that when http://localhost:9080/actions/:actionName is called, the shut_down function should be called.
            Routes::Get(router, "/actions/:actionName", Routes::bind(&MPSensorEndPoint::getAction, this));

            Routes::Post(router, "/action1/:switchoff/:value", Routes::bind(&MPSensorEndPoint::setAction_fereastra, this));
            Routes::Get(router, "/action1/:switchoff/", Routes::bind(&MPSensorEndPoint::getAction_fereastra, this));
        
            Routes::Post(router, "/action/:addremove/:value", Routes::bind(&MPSensorEndPoint::setAction_addremove, this));
            Routes::Get(router, "/action/:addremove/", Routes::bind(&MPSensorEndPoint::getAction_addremove, this));
         Routes::Post(router, "/action2/:addremove/:value", Routes::bind(&MPSensorEndPoint::setAction_interogare, this));
            Routes::Get(router, "/action2/:addremove/", Routes::bind(&MPSensorEndPoint::getAction_interogare, this));
      
       
        }
// Endpoint ========================================================pachet1
    // Endpoint ========================================================pachet1
    void setAction_fereastra(const Rest::Request& request, Http::ResponseWriter response){
        // You don't know what the parameter content that you receive is, but you should
        // try to cast it to some data structure. Here, I cast the settingName to string.
        auto switchoff = request.param(":switchoff").as<std::string>();
    
        // This is a guard that prevents editing the same value by two concurent threads. 
        Guard guard(sensorLock);

        
        string val = "";
        if (request.hasParam(":value")) {
            auto value = request.param(":value");
            val = value.as<string>();
        }

        // Setting the not microwave's setting to value
        string setResponse = ob_on_of.set(switchoff, val);

        // Sending some confirmation or error response.
        if (setResponse == "manual") {
            response.send(Http::Code::Ok, switchoff + " A FOST STETAT IN MODUL [" + val+"]");
        }
if (setResponse == "automat") {
            response.send(Http::Code::Ok, switchoff + " A FOST STETAT IN MODUL [" + val+"]");
        }




        else {
            response.send(Http::Code::Not_Found, switchoff + "  '" + val + "' was not a valid value ");
        }

    }


    // Setting to get the settings value of one of the configurations of the not Microwave
    void getAction_fereastra(const Rest::Request& request, Http::ResponseWriter response){
        auto switchoff = request.param(":switchoff").as<std::string>();

        Guard guard(sensorLock);
	//ob_view_on_of.get_string_value(ob_on_of);
        string valueSetting = ob_view_on_of.get("switchoff");

        if (valueSetting != "") {

            // In this response I also add a couple of headers, describing the server that sent this response, and the way the content is formatted.
            using namespace Http;
            response.headers()
                        .add<Header::Server>("pistache/0.1")
                        .add<Header::ContentType>(MIME(Text, Plain));

//Mecanism1 ob_on_of;
//ob_view_on_of.get_string_value(ob_on_of);
string setResponse = ob_view_on_of.get("controlul_ferestrei");
        response.send(Http::Code::Ok, " SENZORUL DE POLUARE A STETAT FEREASTRA IN MODUL [" + setResponse + "]");



        }
        else {


// Mecanism1 ob_on_of;
//ob_view_on_of.get_string_value(ob_on_of);
string setResponse = ob_view_on_of.get("switchoff");
        response.send(Http::Code::Ok, " SENZORUL DE POLUARE A STETAT FEREASTRA IN MODUL[" + setResponse + "]");


        }
    }
// Endpoint ========================================================pachet12
    // Endpoint ========================================================pachet12
void setAction_addremove(const Rest::Request& request, Http::ResponseWriter response){
        // You don't know what the parameter content that you receive is, but you should
        // try to cast it to some data structure. Here, I cast the settingName to string.
        auto addremove = request.param(":addremove").as<std::string>();
    
        // This is a guard that prevents editing the same value by two concurent threads. 
        Guard guard(sensorLock);

        
        string val = "";
        if (request.hasParam(":value")) {
            auto value = request.param(":value");
            mail_de_scris = value.as<string>();
        }

        // Setting the microwave's setting to value
        string setResponse = ad_ob1.set(addremove, mail_de_scris);

        // Sending some confirmation or error response.
        if (setResponse == mail_de_scris) {


        std::ofstream txtFile ("email.txt" ,std::ios_base::app); 
txtFile << mail_de_scris<< std::endl;

txtFile.close();


            response.send(Http::Code::Ok, " A FOST adaugat [" + mail_de_scris+"]");







        }

        else {
            

                 std::ofstream txtFile ("email.txt" ,std::ios_base::app); 

txtFile << mail_de_scris<< std::endl;

txtFile.close();
            response.send(Http::Code::Not_Found,  mail_de_scris + " adaugat");
        }

    }


    // Setting to get the settings value of one of the configurations of the Microwave
    void getAction_addremove(const Rest::Request& request, Http::ResponseWriter response){
        auto addremove = request.param(":addremove").as<std::string>();

        Guard guard(sensorLock);
	//ob_view_mail.get_string_value(ad_ob1);
        string valueSetting = ob_view_mail.get("addremove");

        if (valueSetting != "") {

            // In this response I also add a couple of headers, describing the server that sent this response, and the way the content is formatted.
            using namespace Http;
            response.headers()
                        .add<Header::Server>("pistache/0.1")
                        .add<Header::ContentType>(MIME(Text, Plain));

//ob_view_mail.get_string_value(ad_ob1);
string setResponse = ob_view_mail.get("e_mail");
        response.send(Http::Code::Ok, " a fost adaugat mailul [" + setResponse + "]");


        }
        else {
//ob_view_mail.get_string_value(ad_ob1);
string setResponse = ob_view_mail.get("e_mail");
        response.send(Http::Code::Ok, " a fost adaugat mailul [" + setResponse + "]");

        }
    }
// Endpoint ========================================================pachet13
    // Endpoint ========================================================pachet13
void setAction_interogare(const Rest::Request& request, Http::ResponseWriter response){
        // You don't know what the parameter content that you receive is, but you should
        // try to cast it to some data structure. Here, I cast the settingName to string.
        auto addremove = request.param(":addremove").as<std::string>();
    
        // This is a guard that prevents editing the same value by two concurent threads. 
        Guard guard(sensorLock);

        
        string val = "";
        if (request.hasParam(":value")) {
            auto value = request.param(":value");
            mail_de_scris = value.as<string>();
        }

        // Setting the microwave's setting to value
        string setResponse = rem_ob1.set(addremove, mail_de_scris);

        // Sending some confirmation or error response.
      
  if (setResponse == mail_de_scris) {
 
 



bool flag_gasit=false;
  
    string line;

fstream newfile;
    newfile.open("email.txt",ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){ 
    
    while (getline(newfile, line)){
        
        if (line.find(setResponse) != string::npos) {

flag_gasit=true;
                    response.send(Http::Code::Ok, " mailul [" + line +" ]ESTE ABONAT LA SERVICII "  );
                    


        }

    

    }


}
newfile.close();
if (flag_gasit==false)
{


   response.send(Http::Code::Ok, " mailul [" + setResponse + " ]NU ESTE ABONAT LA SERVICII" );

}


  }
}

    // Setting to get the settings value of one of the configurations of the Microwave
    void getAction_interogare(const Rest::Request& request, Http::ResponseWriter response){
        auto addremove = request.param(":addremove").as<std::string>();

        Guard guard(sensorLock);
//view_rem_ob.get_string_value(rem_ob1);
string setResponse = view_rem_ob.get("e_mail");

       

            // In this response I also add a couple of headers, describing the server that sent this response, and the way the content is formatted.
            using namespace Http;
            response.headers()
                        .add<Header::Server>("pistache/0.1")
                        .add<Header::ContentType>(MIME(Text, Plain));


bool flag_gasit=false;
  
    string line;

fstream newfile;
    newfile.open("email.txt",ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){ 
    
    while (getline(newfile, line)){
        
        if (line.find(setResponse) != string::npos) {

flag_gasit=true;
                    response.send(Http::Code::Ok, " mailul [" + line +" ]ESTE ABONAT LA SERVICII "  );
                    


        }

    

    }


}
newfile.close();
if (flag_gasit==false)
{


   response.send(Http::Code::Ok, " mailul [" + setResponse + " ]NU ESTE ABONAT LA SERVICII" );

}




}


    // Endpoint ========================================================pachet14
    // Endpoint ========================================================pachet14       
    //Endpoint to configure one of the Sensor's actions.
        void setAction(const Rest::Request& request, Http::ResponseWriter response){


            auto actionName = request.param(":actionName").as<std::string>();


            //This is a guard that prevents editing the same value by 2 concurent threads.
            Guard guard(sensorLock);

            
            
            // Set the window value. 
            int setResponse = sensor.action(actionName);

            //Sending some confirmation or error response.
            if(setResponse == 1){
                if(actionName == "shut_down")
                    response.send(Http::Code::Ok, "The window is closed now!");
            
            }else {
                response.send(Http::Code::Not_Found, actionName);
            }
        }
            // Setting to get the actions value of one of the configurations of the Sensor
            void getAction(const Rest::Request& request, Http::ResponseWriter response){


                auto actionName = request.param(":actionName").as<std::string>();
                
                Guard guard(sensorLock);

                string valueAction = sensor.get(actionName);
                if(valueAction != "") {
                     //Int this response I also add a couple of headers, describing the server that sent this response, abd the way the cintent is formatted.
                     using namespace Http;
                     response.headers()
                        .add<Header::Server>("pistache/0.1")
                        .add<Header::ContentType>(MIME(Text, Plain));
                    
                    response.send(Http::Code::Ok, actionName + " is done.");
                }else {
                    response.send(Http::Code::Not_Found, actionName + "was not performed.");

                }

            }
                //Defining the class of the Pollution Sensor. It should model the entire configuration of the Pollution Sensor

                class Sensor {

                    public:
                        explicit Sensor() {

                        }

                        ///Perform the action...
                        int action(std::string name) {
                            
                            if(name == "shut_window"){
                                //The "winPos" tell us if the window is open or closed.
                                //In this case, we need to comunicate with db where are datas about the window model
                                //if window is already closed, print in view("The window is already closed!")
                                // else, try to close the window, If the operation succeeded,
                                // we print in view "The window is now closed!" message, otherwise, we print
                                // "Some error appeared during closing the window process!"

                                return 1;
                            }

                            return 0;
                        }

                        //getter
                        string get(std::string name){
                            //for open or close window, we don't have getters
                            //for get_air_quality etc we have
                            

                            //TO SOLVER LATTER! WHEN REACH THESE GET ACIONS

                            return name;
                        }
                };

    // Create the lock which prevents concurrent editing og the same variable 
    using Lock = std::mutex;
    using Guard = std::lock_guard<Lock>;
    Lock sensorLock;

    //Instance of the sensor model
    Sensor sensor;
Controller_SWITCH_F ob_on_of;
Controller_plus_MAIL ad_ob1;
Controller_INTEROGARE_MAIL rem_ob1;
Viewer_ADD_MAIL ob_view_mail; 
Viewer_SWITCH_AUTO_MANUAL ob_view_on_of; 
Viewer_ADD_REMOVE_USER view_rem_ob; 


    // Defining the httpEndpoint and a router.
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

        
};


int main(int argc, char *argv[]){

    //This code is needed for gracefull shutdown of the server when no longer needed.
    sigset_t signals;
    if(sigemptyset(&signals) != 0
        || sigaddset(&signals, SIGTERM) != 0 
        || sigaddset(&signals, SIGINT) != 0
        || sigaddset(&signals, SIGHUP) != 0
        || pthread_sigmask(SIG_BLOCK, &signals, nullptr) != 0 ) {

            perror("Install signal handler failed!");
            return 1;

        }

    // Set a port on which your server to communicate
    Port port(9080);


    //Number of threads used by the server
    int thr = 2;

    if(argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if(argc == 3){
            thr = std::stoi(argv[2]);
        }
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << "threads " << endl;


    //Instance of the class that defines what the server can do.
    MPSensorEndPoint stats(addr);

    //Initialize and start the server
    stats.init(thr);
    stats.start();

    //Code that waits for the shutdown signal for the server
    int signal = 0;
    int status = sigwait(&signals, &signal);
    if(status == 0) {
        std::cout << "received signal " << signal << std::endl;
        
    } else {
        std::cerr << "sigwait returns " << status << std::endl;
    }

    stats.stop();

    
}





