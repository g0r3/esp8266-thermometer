#define MAX_REQUEST_LENGTH 1024

#include "WebPage.h"
#include "ConfigHandler.h"
#include <IPAddress.h>

void handle_network_tasks() {
  
  String header;
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        
        if (header.length() <= MAX_REQUEST_LENGTH) {
           header += c;
        } else { 
          Serial.println("\nRequest too long. Dropping connection.");
          break; 
        }
        
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            if (header.substring(0, 16) == "GET /submit.php?") {
              process_data(header);
              set_weather_data_fetch_flag();
              update_display_flag();
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println(print_page(configuration.city, configuration.countrycode, configuration.apikey));   
            }
            else if (header.substring(0, 17) == "GET /measurements") {
                char output[500];
                StaticJsonBuffer<500> jsonBuffer;
              
                JsonObject& root = jsonBuffer.createObject();
                
                JsonObject& inside = root.createNestedObject("inside");
                inside.set("temperature", temperature);
                inside.set("humidity", humidity);
              
                JsonObject& outside = root.createNestedObject("outside");
                outside.set("temperature", current_weather.temp);
                outside.set("humidity", current_weather.humidity);
                outside.set("wind", current_weather.wind_speed);
                outside.set("temp_min", current_weather.temp_min);
                outside.set("temp_max", current_weather.temp_max);
                outside.set("pressure", current_weather.pressure);
              
                root.prettyPrintTo(output);
                client.println(output);
            } else {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              client.println(print_page(configuration.city, configuration.countrycode, configuration.apikey));   
            }
            
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  } 
}
