#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Ticker.h>
#include <math.h>

Ticker get_weather;
                          
void process_current_weather_json(String current_weather_json) {
  if (current_weather.error_data != "") {
    return;
  }
  
  const size_t bufferSize = JSON_ARRAY_SIZE(1) + 
                          3*JSON_OBJECT_SIZE(1) + 
                          2*JSON_OBJECT_SIZE(2) + 
                          JSON_OBJECT_SIZE(4) + 
                          JSON_OBJECT_SIZE(5) + 
                          JSON_OBJECT_SIZE(6) + 
                          JSON_OBJECT_SIZE(14) + 
                          430; //size calculation for our specific json type
                          
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(current_weather_json);
  
  strncpy(current_weather.weather_id, root["weather"][0]["id"], weather_api_max_field_size);
  strncpy(current_weather.weather_main, root["weather"][0]["main"], weather_api_max_field_size);
  strncpy(current_weather.weather_description, root["weather"][0]["description"], weather_api_max_field_size);
  strncpy(current_weather.weather_icon, root["weather"][0]["icon"], weather_api_max_field_size);

  current_weather.temp = root["main"]["temp"];
  current_weather.pressure = root["main"]["pressure"];
  current_weather.humidity = root["main"]["humidity"];
  current_weather.wind_speed = root["wind"]["speed"];
  current_weather.wind_deg = root["wind"]["deg"];
  current_weather.clouds_all = root["clouds"]["all"];
  current_weather.rain = root["rain"]["rain.3h"];
  current_weather.snow = root["snow"]["snow.3h"];
  current_weather.dt = root["dt"];
  current_weather.sys_sunrise = root["sys"]["sunrise"]; 
  current_weather.sys_sunset = root["sys"]["sunset"];
}

void process_forecast_weather_json(String forecast_json) {

  if (current_weather.error_data != "") {
    return;
  }
  
  String temp_data = "";
  int end = 0;
  int start = 0;
  float min = 100;
  float max = -100;
  float f_temp_data = 0.0;
  int count = 0;
  for (int i = 0; i <=8; i++) {
    start = forecast_json.indexOf("\"temp\":", start);
    count = 0;
    while (forecast_json.charAt(start + count) != ',') {
      count ++;
    }
    end = start + count;
    temp_data = forecast_json.substring(start+7, end);
    Serial.println(temp_data);
    start = end;
    f_temp_data = temp_data.toFloat();
    if (min > f_temp_data) {
      min = f_temp_data;
    }
    if (max < f_temp_data) {
      max = f_temp_data;
    }
  }
  current_weather.temp_min = round(min);
  current_weather.temp_max = round(max);
}

String load_data(String url) {
  HTTPClient http;
  String ret = "";
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      current_weather.error_data = "";
      Serial.println("reset error cache");
      ret = http.getString();
      Serial.println("got data");
    }
    else {
      current_weather.error_data = http.getString();
      Serial.println(current_weather.error_data);
    }
  } else {
    current_weather.error_data = "[HTTP] GET... failed, error: " + http.errorToString(httpCode);
    Serial.println(current_weather.error_data);
  }
  Serial.println("trying to end http");
  http.end();
  Serial.println("ended http");
  return ret;
}

void fetch_weather_data() {
  
  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + String(configuration.city) + "," + String(configuration.countrycode)+ "&units=metric&APPID=" + String(configuration.apikey);
  Serial.println(url);
  String str_current_weather = load_data(url);
  Serial.println("Stepped out 1");
  process_current_weather_json(str_current_weather);
  str_current_weather = "";
  url = ""; 

  String forecast_url = "http://api.openweathermap.org/data/2.5/forecast?q=" + String(configuration.city) + "," + String(configuration.countrycode)+ "&units=metric&APPID=" + String(configuration.apikey)  + "&cnt=9";
  Serial.println(forecast_url);
  String forecast_weather = load_data(forecast_url);
  Serial.println("Stepped out 2");

  process_forecast_weather_json(forecast_weather);
    

  Serial.println(current_weather.weather_id);
  Serial.println(current_weather.weather_main);
  Serial.println(current_weather.weather_description);
  Serial.println(current_weather.weather_icon);
  Serial.println(current_weather.temp);
  Serial.println(current_weather.pressure);
  Serial.println(current_weather.humidity);
  Serial.println(current_weather.temp_min);
  Serial.println(current_weather.temp_max);
  Serial.println(current_weather.wind_speed);
  Serial.println(current_weather.wind_deg);
  Serial.println(current_weather.clouds_all);
  Serial.println(current_weather.rain);
  Serial.println(current_weather.snow);
  Serial.println(current_weather.dt);
  Serial.println(current_weather.sys_sunrise);
  Serial.println(current_weather.sys_sunset);
  
  time_to_fetch_weather_data = false;
}

void set_weather_data_fetch_flag() {
  time_to_fetch_weather_data = true;
}

void setup_weather_provider(int clock_time) {
  get_weather.attach(clock_time, set_weather_data_fetch_flag);
}
