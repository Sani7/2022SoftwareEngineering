# Project

## Requirements
### Hardware
* display (Waveshare 3.7inch e-Paper HAT)
* ESP32 Developement Board
* Rotary Encoder
### Software
* User input => Rotary encoder + Button to switch between  modes (day, week, month, year)
* SD card to read WiFi Credentials (SSID, PSWD, Server IP, URI, API-Key)
* Connecting to wifi
* Database input => from a HTTP get (Get Parameter => mode) request that reads out a database via a php script 
* Output => Graph on the E-paper display
