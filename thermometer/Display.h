#include <SPI.h>
#include <GxEPD.h>
#include <GxGDEW042T2/GxGDEW042T2.cpp>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include "WeatherIcons.h"
#include <Ticker.h>
#include <Time.h>
#include <ArduinoJson.h>

GxIO_Class io(SPI, D8, D3, D4); 
GxEPD_Class display(io, D4, D6);
Ticker disp_update; 


void update_display() {
  int weather_icon_x = 5;
  int weather_icon_y = 135;
  int weather_icon_size_x = 144;
  int weather_icon_size_y = 144;
  bool day = false;
  
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(0, 0, 300, 2, GxEPD_BLACK);
  display.fillRect(0, 0, 2, 400, GxEPD_BLACK);
  display.fillRect(298, 0, 2, 400, GxEPD_BLACK);
  display.fillRect(0, 398, 300, 2, GxEPD_BLACK);

  String inside_temp = String(temperature);
  inside_temp = inside_temp.substring(0, inside_temp.length()-1) + "~C";
  String inside_hum = String(humidity);
  inside_hum = inside_hum.substring(0, inside_hum.length()-1) + "%";
  String outside_temp = String(current_weather.temp) + "~C";

  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(130, 15);
  display.println("Inside:");
  display.fillRect(0, 17, 300, 2, GxEPD_BLACK);
  
  display.setFont(&FreeSansBold24pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.drawBitmap(55, 22, thermometer, 50, 50, GxEPD_BLACK);
  display.drawBitmap(200, 22, humidity_icon, 50, 50, GxEPD_BLACK);
  display.setCursor(5, 108);
  display.println(inside_temp);
  display.setCursor(165, 108);
  display.println(inside_hum);
  display.fillRect(0, 114, 300, 2, GxEPD_BLACK);

  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(120, 129);
  display.println("Outside:");
  display.fillRect(0, 131, 300, 2, GxEPD_BLACK);

  display.fillRect(0, 381, 300, 2, GxEPD_BLACK);
  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(5, 396);
  String status_string = "IP: " + String(ipaddress) + "  Updated: " + String(hour(current_weather.dt)) + ":" + String(minute(current_weather.dt));
  display.println(status_string);



  if (current_weather.dt >= current_weather.sys_sunrise &&
      current_weather.dt <= current_weather.sys_sunset) {
      day = true;
  }

  if (current_weather.error_data == "") { // don't draw the online stuff if there was an error
    
    if      (current_weather.weather_id[0] == '2') { //thunderstorms
      
      display.drawBitmap(weather_icon_x, weather_icon_y, weather_storm, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
    }
    else if (current_weather.weather_id[0] == '3') { //drizzle
              
      display.drawBitmap(weather_icon_x, weather_icon_y, weather_showers_scattered, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
    }
    else if (strcmp(current_weather.weather_id, "500") == 0 || //light rain
             strcmp(current_weather.weather_id, "501") == 0 ||
             strcmp(current_weather.weather_id, "520") == 0 ||  
             strcmp(current_weather.weather_id, "521") == 0) {
          
      display.drawBitmap(weather_icon_x, weather_icon_y, weather_showers_scattered, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
    }
    else if (strcmp(current_weather.weather_id, "502") == 0 || //rain
             strcmp(current_weather.weather_id, "503") == 0 ||
             strcmp(current_weather.weather_id, "504") == 0 ||
             strcmp(current_weather.weather_id, "522") == 0 ||
             strcmp(current_weather.weather_id, "531") == 0) {
          
      display.drawBitmap(weather_icon_x, weather_icon_y,weather_showers, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
    }
    else if (strcmp(current_weather.weather_id, "600") == 0 || // snow
             strcmp(current_weather.weather_id, "601") == 0 ||
             strcmp(current_weather.weather_id, "602") == 0) {
          
      display.drawBitmap(weather_icon_x, weather_icon_y, weather_snow, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
    }
    else if (current_weather.weather_id[0] == '6' || // snow + rain
      strcmp(current_weather.weather_id, "511") == 0) {
            
      display.drawBitmap(weather_icon_x, weather_icon_y, weather_icy_showers, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
    }
    else if (current_weather.weather_id[0] == '7') { // atmosphere & fog
          
      display.drawBitmap(weather_icon_x, weather_icon_y, weather_fog, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
    }
    else if (strcmp(current_weather.weather_id, "800") == 0) { //clear
      
      if (day) {
          display.drawBitmap(weather_icon_x, weather_icon_y, weather_clear, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
      } else {
        display.drawBitmap(weather_icon_x, weather_icon_y, weather_night_clear, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
      }
      
    }  
    else if (strcmp(current_weather.weather_id, "801") == 0 || //few clouds
             strcmp(current_weather.weather_id, "802") == 0) {
              
      if (day) {
          display.drawBitmap(weather_icon_x, weather_icon_y, weather_few_clouds, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
      } else {
        display.drawBitmap(weather_icon_x, weather_icon_y, weather_few_clouds_night, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
      }   
    }
    else if (strcmp(current_weather.weather_id, "803") == 0 || // cloudy
             strcmp(current_weather.weather_id, "804") == 0) {
              
      if (day) {
          display.drawBitmap(weather_icon_x, weather_icon_y, weather_cloudy, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
      } else {
        display.drawBitmap(weather_icon_x, weather_icon_y, weather_clouds_night, weather_icon_size_x, weather_icon_size_y, GxEPD_BLACK);
      }    
    }
  
    display.setFont(&FreeSansBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.drawBitmap(192, 158, thermometer, 50, 50, GxEPD_BLACK);
    display.setCursor(159, 248);
    display.println(outside_temp);
  
    display.setFont(&FreeSansBold12pt7b);
    display.drawBitmap(5, 277, wind, 50, 50, GxEPD_BLACK);
    display.setCursor(65, 310);
    String wind_spd = String(current_weather.wind_speed);
    wind_spd = wind_spd.substring(0, wind_spd.length()-1) + "km/h";
    display.println(wind_spd);
  
    display.drawBitmap(180, 277, humidity_icon, 50, 50, GxEPD_BLACK);
    display.setCursor(230, 310);
    String humidity_out = String(current_weather.humidity);
    humidity_out = humidity_out + "%";
    display.println(humidity_out);
  
    display.drawBitmap(5, 327, min_max, 50, 50, GxEPD_BLACK);
    display.setCursor(65, 360);
    String str_minmax = String(current_weather.temp_min)  + "/" + String(current_weather.temp_max) + "~C";
    display.println(str_minmax);
  
    display.setCursor(180, 360);
    display.println("hPa:");
    display.setCursor(230, 360);
    display.println(current_weather.pressure);

  } else {
    const size_t bufferSize = JSON_ARRAY_SIZE(2);
    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject& root = jsonBuffer.parseObject(current_weather.error_data);
    
    display.setFont(&FreeSansBold12pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(5, 155);
    String message = root["message"];
    display.println(message);
  }

  display.update();
  time_to_update_display = false;
}

void update_display_flag() {
  time_to_update_display = true;
}

void setup_display(int clock_time) { 
  display.init();
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeSansBold18pt7b);
  display.setCursor(80, 155);
  display.println("ESP8266");
  display.setCursor(20, 191);
  display.println("Weatherstation");
  display.setCursor(60, 300);
  display.println("initializing...");
  
  display.update();
  delay(5000);
  disp_update.attach(clock_time, update_display_flag);
}
