#include <DHT.h>
#include <Ticker.h>
#include <math.h>

#define DHT_DATA 5 //NodeMCU Pin D1
#define DHTTYPE DHT22


DHT dht (DHT_DATA, DHTTYPE);
Ticker read_temps;

void set_temp_read_flag() {
  time_to_read_temps = true;
}

void read_temp() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  int i = 0;
  while (isnan(temperature) && i > 100) {
    temperature = dht.readTemperature();
    i++;
  }
  i = 0;
  while (isnan(humidity) && i > 100) {
    humidity = dht.readHumidity();
    i++;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Temp: ");
  Serial.print(temperature);
  Serial.println(" Celsius");
  time_to_read_temps = false;
} 

void setup_temperature_sensor(int clock_time) {
  dht.begin();
  read_temps.attach(clock_time, set_temp_read_flag);
}
