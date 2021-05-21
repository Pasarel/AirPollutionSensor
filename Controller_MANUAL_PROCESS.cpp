#include <iostream>
#include <cstdlib>
#include "Viewer_MANUAL_PROCESS.cpp"
#include "WINDOW_MODEL.cpp"
#include "json.hpp"


using namespace std; 
using json = nlohmann::json;

struct Coord {
  
};

class Controller_Manual_Process {

    
    const double geoLatitude = 47.430603;
    const double geoLongitude = 26.126609;

    Viewer_Manual_Process view;

    string closestSensor() {
       //read from "data.json" file 
      const string filename = "data.json";
      ifstream reader(filename);
      json j;
      reader >> j;
      reader.close(); 

      if(j.contains("coords_array")) {
        double coord[2];
        double longitude, minLongitude = 100.00;
        double latitude, minLatitude = 100.00;
        int nCoords = 0;
        double diffMinim = 1001.001;

        //iterate through "coords_array" from json, the array thar contains 
        // the coords for every pollution sensor
        while(nCoords < j["coords_array"].size()){
          latitude = j["coords_array"][nCoords][0];
          longitude = j["coords_array"][nCoords][1];

          if(abs(latitude - geoLatitude) + abs(longitude - geoLongitude) < diffMinim) {
            
            diffMinim = abs(latitude - geoLatitude) + abs(longitude - geoLongitude);
            minLatitude = latitude;
            minLongitude = longitude;

          }

          nCoords++;
        }
        
        string coordsKey = to_string(minLatitude);
        string coords1 =  to_string(minLongitude);
        coordsKey = coordsKey + ","  + coords1;
        return coordsKey;
      } 
        
          
      


      return "Not found";
    }

    void readFromJSON(json &j) {

      const string filename = "data.json";
      ifstream reader(filename);
      reader >> j;
      reader.close();

    }

    public:
      
       explicit Controller_Manual_Process() {

       }
       
       void openWindow(const Rest::Request& request, Http::ResponseWriter response) {
          Viewer_Manual_Process v;
          string message = "";
          
          if(request.hasParam(":id")) {
            auto idWindows = request.param(":id");
            string id = idWindows.as<string>();
            // de completat dupa ce modelez json-ul cu datele necesare
            message = "Functia de deschis geamul!(not finished)";
          }else {
            message = "Nu s-a dat parametrul pentru id-ul geamului!";
          }

          response.send(Http::Code::Ok, v.openWindow(message));

          
       }

       void closeWindow(const Rest::Request& request, Http::ResponseWriter response) {
          
          Viewer_Manual_Process v;
          string message = "";
          
          if(request.hasParam(":id")) {
            auto idWindows = request.param(":id");
            string id = idWindows.as<string>();
            //!! de completat dupa ce rezolv cu parte de json 
            message = "Functia de inchis geamul!";

            
          }else {
            message = "nu s-a dat parametrul pentru id-ul geamului! \n Coordonate cel mai aproape sensor ";
          }

          response.send(Http::Code::Ok, v.closeWindow(message));
          

       }
      // check if the window is open or not
       void checkWindow(const Rest::Request& request, Http::ResponseWriter response) {
           Viewer_Manual_Process v;
            
            string message = "";
            string id = "";
            if(request.hasParam(":id") && request.hasParam(":checkWindow")){
              auto idWindows = request.param(":id");
              id = idWindows.as<string>();
              message = "Da, geamul " + id + " a fost verificat!"; 
            }else {
                message = "Nu s-a transmis id-ului geamului!";
            }
            response.send(Http::Code::Ok, v.checkWindow(message));    
       }

       //get the air quality value registered by the colsest pollution sensor
       void getAirQaulity(const Rest::Request& request, Http::ResponseWriter response) {
         Viewer_Manual_Process view;

         string message = "";

         //read from "data.json" file 
         const string filename = "data.json";
         ifstream reader(filename);
         json j;
         reader >> j;
         reader.close(); 

         message = "Senzorul de poluare din zona dumneavoastra a inregistrat urmatoarele date despre aer : ";
         message += to_string(j[closestSensor()]["mean"]);
         double airQualityValue;
         airQualityValue = j[closestSensor()]["mean"]["CO"][0];
         airQualityValue = j[closestSensor()]["mean"]["CO"][1];
         
         if(airQualityValue <= 0.1){
           message += "\n\nAnalizand datele de mai sus, se poate spune ca aerul la momentul actual din zona dvs. nu este poluat!";
         }else {
           message += "\n\n In urma analizarii datelor de mai sus, cu parere de rau, va anuntam ca aerul din zona dvs. este poluat la momentul actual. Va sugeram sa evitati iesirile in aer liber si geamul sa fie inchis.";

         }
         response.send(Http::Code::Ok, view.getAirQuality(message));
       }

       //get the PM2.5 value from the closest pollution sensor
       void getPM25(const Rest::Request& request, Http::ResponseWriter response) {
        
        //read from json file 

       const string filename = "data.json";
         ifstream reader(filename);
         json j;
         reader >> j;
         reader.close();

        string message = "Valoarea pm2.5 inregistrata in zona dvs. este: ";
        message += to_string(j[closestSensor()]["mean"]["PM2.5"]);

        
        response.send(Http::Code::Ok, view.getPM25(message));

       }

     
      



};