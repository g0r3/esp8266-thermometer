struct config_data {
    char city[city_len]="";
    char countrycode[countrycode_len]="";
    char apikey[apikey_len]="";
};

struct weather_data {
    char weather_id[weather_api_max_field_size] = "";
    char weather_main[weather_api_max_field_size] = "";
    char weather_description[weather_api_max_field_size] = "";
    char weather_icon[weather_api_max_field_size] = "";
    int temp = 0;
    int pressure = 0;
    int humidity = 0;
    int temp_min = 0;
    int temp_max = 0;
    float wind_speed = 0;
    int wind_deg = 0;
    int clouds_all = 0;
    int rain = 0;
    int snow = 0;
    long dt = 0;
    long sys_sunrise = 0; 
    long sys_sunset = 0;
    String error_data = "";
};
