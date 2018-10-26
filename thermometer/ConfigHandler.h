
#if city_len + countrycode_len + apikey_len > 4096
  #error "Maximum flash memory amount exceeded"
#endif

#include <EEPROM.h>

void write_config(String data[], int data_length) {
  
  write_header("write_config()");
  String key = "";
  String value = "";
  char city_trn[city_len]="";
  char countrycode_trn[countrycode_len]="";
  char apikey_trn[apikey_len]="";

  for (int i = 0; i < data_length; i++) {
      key = data[i].substring(0, data[i].indexOf("="));
      value = data[i].substring(data[i].indexOf("=")+1, data[i].length());
      Serial.println(key);
      Serial.println(value);
      String sanitized_value = "";
      int char_at_i = 0;
      for (int i = 0; i< value.length(); i++) {
        char_at_i = value.charAt(i) + 0;
        Serial.println(char_at_i);
        if ((char_at_i >= 48 && char_at_i <= 57) ||  //number
            (char_at_i >= 65 && char_at_i <= 90) ||  //lowercase letter
            (char_at_i >= 97 && char_at_i <= 122)) { //uppercase letter
             sanitized_value += value.charAt(i);
           }
      }

      //copy values
      if (key == "city") {
        sanitized_value.toCharArray(city_trn, city_len);            // truncate possibly too long value to length
        strncpy(configuration.city, city_trn, city_len);            // copy value into config struct
      }
      else if (key == "countrycode") {
        sanitized_value.toCharArray(countrycode_trn, countrycode_len);
        strncpy(configuration.countrycode, countrycode_trn, countrycode_len);
      }
      else if (key == "apikey") {
        sanitized_value.toCharArray(apikey_trn, apikey_len);
        strncpy(configuration.apikey, apikey_trn, apikey_len);
      }
  }

  EEPROM.put(0,configuration);
  EEPROM.commit();  
}

void read_config_from_storage() {
  
  write_header("read_config_from_storage()");
  EEPROM.get(0,configuration);
  
  Serial.println("Current configuration is:");
  Serial.println(configuration.city);
  Serial.println(configuration.countrycode);
  Serial.println(configuration.apikey);
}

void process_data(String header) {
  
  write_header("process_data()");
  String data = header.substring(header.indexOf("?")+1, header.indexOf(" HTTP/1.1"));
  String remaining_data = data;
  String data_pair = "";
  
  Serial.print("Data received: ");
  Serial.println(data);
  
  int data_pair_count = 1;
  if (data.length() > 0) { 
    for (int i = 0; i < data.length(); i++) {
      if (data.charAt(i) == '&') {
        data_pair_count++;
      }
    }
    Serial.println("data_pairs detected: ");
    Serial.println(data_pair_count);
    
    String data_pairs[data_pair_count];
    int index = 0;
    while (remaining_data.indexOf("&")>=0) {
      data_pair = remaining_data.substring(0, remaining_data.indexOf("&"));
      remaining_data = remaining_data.substring(remaining_data.indexOf("&")+1, remaining_data.length());
      data_pairs[index] = data_pair;
      index++;
    }
    data_pairs[index] = remaining_data;

    write_header("Values start");
    for (int i = 0; i < data_pair_count; i++) {
      Serial.println(data_pairs[i]);
    }
    write_header("Values stop");
    write_config(data_pairs, data_pair_count);
  }
}
