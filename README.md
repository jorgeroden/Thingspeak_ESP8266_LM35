# Thingspeak_ESP8266_LM35

This is a temperature logger with ESP8266 (D1 Linknode), LM35 Temperature sensor and Thinkspeak cloud service.
This program deals with several topics:
- Set up ESP8266 Wifi connectivity.
- Read analog signal on ESP8266 1v ADC. A linear regression has been done because of inaccuracies on ADC.
- OTA (On the air): Updating board software through WIFI (no USB cable needed to reload the program).
- Send data to the cloud service Thingspeak.com 

Educational purpose.
