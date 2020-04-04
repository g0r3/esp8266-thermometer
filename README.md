
# ESP8266 based wifi room thermometer and weatherstation

![title](https://github.com/g0r3/esp8266-thermometer/blob/master/title.png)

A room thermometer which uses a DHT22 sensor for measuring the room temperature and fetches weather data via Openweathermap. Its features:
* 4.2 inch epaper display
* Wifi access point functionality for initial setup (thanks to Wifi Manager)
* Web interface for configuring the data needed for Openweathermap
* Web interface with JSON output to use the measurements via the network

**Setup**

When started the first time, the thermometer creates an access point 'AutoConnectAP'. Connect to it, pick you network (only 2.4G), enter its credentials. Afterwards it reboots and should show the device IP address in the line on the bottom (DHCP is compulsory). Browse to it and you will bring you to the page to enter your Openweathermap API key (create an account: https://openweathermap.org/) and location.

**Usage**

All the relevant data is shown on the display itself. Just pick a nice place for the thermometer. Since Openweathermap does not provide a proper min/max-Temp functionality, it only shows the min/max values for the next 24 hours starting from the current time. Apart from reading them of the display you can get all the measurement data by opening the page http://yourthermometerip/measurements and use it remotely:

`{
  "inside": {
    "temperature": 25.9,
    "humidity": 35.2
  },
  "outside": {
    "temperature": 11,
    "humidity": 71,
    "wind": 0.5,
    "temp_min": 2,
    "temp_max": 15,
    "pressure": 1012
  }
}`

 In my case I am logging the temperature and humidity using a homeassistant server.

**Parts list**

1. a NodeMCU dev board. Not exactly necessary needed, but it already contains a voltage regulator, which makes things easier. And I've designed the case around it.
2. a Waveshare 4.2 inch E-paper module (https://www.waveshare.com/wiki/4.2inch_e-Paper_Module)
3. a DHT22. There is a successor using I2C. But I had the DHT22 laying around. So the DHT22 it is...
4. a case. I added my STLs for 3D printing a case to the project. You may use your design or no case at all.
5. if you use my Case: four 2 to 2.5 mm diameter screws to fixate the NodeMCU board in the base of the case. 

**Build process**

The build process is rather selfexplanatory. If you go with my case and my hardware suggestion, there are two things you have to do as a preparation:
1. Desolder the plug from the display, as it is too tall for the case. We use the solder holes in the module on the other side. You can cut off the wires and reuse them.
2. Desolder the pin headers from the NodeMCU module.

Also you should assemble the case at least once before starting the build process, to make sure the parts fit together nicely or to convince them to do so by using a file or some sandpaper.


First you have to solder the wires to the DHT22. Tinning the connectors and wires in before helps. I also used some shrinking tubes to make sure that the connectors don't touch each other. Afterwards I've glued the sensor into the backcover using hot glue.

The next step is to glue the display into place. First try to position it as exactly as possible within the frame and use some sticky tape to fixate it. Then again I glued it in place with hot glue. Try to keep the glue level lower than the frame next to it so that you don't have problems when putting backcover and front panel together.

Now it is time to join the front and backcover together. Apply some superglue on the very borders of the frontpanel for that. Then place the backcover on the front panel and push against the top so that it sits nicely within the front panel. Apply some pressure at the borders and let it dry.

After it has dried you can continue with soldering the wires to the NodeMCU. Again, tin the wires before soldering to the board. The wiring looks as follows:

Epaper | ESP8266
---------- | ----------
3.3v | 3.3V
GND | GND
CS | D8
RESET | D4
DC | D3
DIN | D7
CLK | D5
BUSY | do not connect

DHT22 | ESP8266
---------- | ----------
3.3v | 3.3V
GND | GND
Data | D1
** If your ESP doesn't reboot after connecting the display, the D8 pin might be responsible. If this is is the case, add a 2.2k pulldown resistor from D8 to GND and you should be good



Now you can glue the base in place with some super glue. Give it some time to dry, before you fixate the NodeMCU in the base using the screws. Check, that the Micro USB port aligns nicely with the hole. After placing the tap on the base (the use of sandpaper might be advisable) the assembly process is done.

**The project/code**

This was my first Arduino-ish project (besides blinky LED) and I have next to no experience with C/C++. This means, that my code most likely horrible and full of flaws, since I was dealing more with figuring out the basics instead of perfectly planning out the project itself. So feedback is welcome and even though I don't plan to support the project, the occasional pull request might be merged.

Following libraries and all their dependencies are needed:

Library | Source
---------- | ----------
GxEPD | https://github.com/ZinggJM/GxEPD
ArduinoJson | https://github.com/bblanchon/ArduinoJson or Arduino Library manager (use 5.x not 6.x)
Adafruit DHT Sensor Library | https://github.com/adafruit/DHT-sensor-library or Arduino Library manager
Adafruit GFX Library (dependency of GxEPD) | https://github.com/adafruit/Adafruit-GFX-Library or Arduino Library manager
Time | https://github.com/PaulStoffregen/Time
WifiManager | https://github.com/tzapu/WiFiManager




Since the Adafruit fonts in the Adafruit GFX library don't include any degree character I replaced the tilde glyph with the degree glyph. So to get a ° character you have to use ~ in the code. Printing a tilde on the display is not possible anymore. You have to copy the modified font files in to the Adafruit GFX library folder in order to get the ° glyph.
Also I had to make a small modification to the DHT library in order to make the sensor work with the ESP8266. Replace the default one with the one present in this project.

Some ideas I had for software improvements:
* Use HTTPS
* Password for the web UI
* Use a more framework-like system to manage settings. Currently all is rather entangled with rest.
* Add additional configuration options to the web UI, e.g for
  *  allowing the configuration of refresh timer values for the various components (display, weather update, temperature measurement)
  * Static IP support
  * imperial units (for now just metric) 
* NTP client for showing the correct display update time (for now only the time when the weather data was recorded), and in order to get the min max temps once a day for the whole day.

Hardware:
* Add a display light
* Add buttons for soft reset, light, display refresh and "factory" reset

**Credits**

The design of the configuration page was blatantly stolen from Wifi manager.

Weather icons: 
https://github.com/snwh/paper-icon-theme
"[Paper Icons](http://snwh.org/paper/icons)" by [Sam Hewitt](http://samuelhewitt.com/) is licensed under [CC-SA-4.0](http://creativecommons.org/licenses/by-sa/4.0/)

Thermometer icon: 
Icon made by [hirschwolf](https://www.flaticon.com/authors/hirschwolf) from [www.flaticon.com](http://www.flaticon.com/ "Flaticon")

Humidity icon: 
Icon made by [pixel perfect](https://www.flaticon.com/authors/pixel-perfect) from [www.flaticon.com](http://www.flaticon.com/ "Flaticon")

Wind icon:
Icon made by [yannick](https://www.flaticon.com/authors/yannick) from [www.flaticon.com](http://www.flaticon.com/ "Flaticon")

Temperature min/max icon:
przemof on https://openclipart.org/detail/205610/thermometer-icon-with-minmax-indicator