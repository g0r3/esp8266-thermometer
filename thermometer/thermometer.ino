
// max storage for each config entry
#define city_len 256                  //byte
#define countrycode_len 3             //byte
#define apikey_len 33                 //byte
#define temp_measurement_clock 5      //seconds
#define weather_api_max_field_size 64 //seconds
#define fetch_weather_clock 60        //seconds
#define update_display_clock 300      //seconds

#include "Log.h"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "StructDefinitions.h"

//------------------ global vars needed for further imports-------------------

struct config_data configuration;
struct weather_data current_weather;
float humidity;
float temperature;
bool time_to_read_temps = false;
bool time_to_fetch_weather_data = false;
bool time_to_update_display = false;
String ipaddress;

// Set web server port number to 80
WiFiServer server(80);
#include "Display.h"
#include "OpenWeatherMapDataProvider.h"
#include "WebHandler.h"
#include "TemperatureSensor.h"



//------------------------------------done------------------------------------



void setup() {
  write_header("setup()");
  Serial.begin(115200);
  WiFi.hostname("ESP8266-Weatherstation");
  EEPROM.begin(city_len + countrycode_len + apikey_len);
  setup_display(update_display_clock);
  WiFiManager wifiManager;
  wifiManager.setTimeout(300);

  if(!wifiManager.autoConnect("AutoConnectAP", "diesistdaspasswort")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
  } 

  Serial.println("Connected.");
  ipaddress = WiFi.localIP().toString(); 
  read_config_from_storage();
  setup_temperature_sensor(temp_measurement_clock);
  read_temp();
  setup_weather_provider(fetch_weather_clock);
  fetch_weather_data();
  update_display();
  Serial.println();
  server.begin();
}

void loop(){
  if (time_to_read_temps) {
    read_temp();
  }
  if (time_to_fetch_weather_data) {
    fetch_weather_data();
  }
  if (time_to_update_display) {
    update_display();
  }
  handle_network_tasks();
}
