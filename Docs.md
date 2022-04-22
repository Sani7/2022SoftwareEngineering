# Documentation

## Install espidf
install vscode\
(see [link](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html))


## Build tool espidf
get into env            ```get_idf```\
set target (set board)  ```ìdf.py set-target esp32```\
make project            ```idf.py create-project```
build project           ```ìdf.py build```\
flash project           ```ìdf.py -p /dev/ttyUSBX flash```\
monitor / debug port    ```ìdf.py monitor -p /dev/ttyUSBX```\

## Config File

How it is constructed:
[ConfigName]=[Input]'\n'\
* SSID      => Wifi SSID
* PSWD      => Wifi Pasword
* IP        => Server IP
* Port      => Server Port
* URI       => where and which file to open (PHP,html, ...)
* APIKey    => Verification Key

## How to use
1) Flick the powerswitch
2) wait till system boots and is connected to wifi\
if it can't connect to the server it will give an error on the screen
3) It will show the daydata by default
4) To switch to other modes use the knob and turn it to the right or left
5) Press the knob to select the mode and it will change the graph

## Error's
### Hard errors
* SD card not found
* Config file not found
* Config rule missing
* If the device can't connect to the Wifi it will show a message on the screen:\
```Cant connect to [SSID]```
* If the server isn't accesible it will display:\
```Can't connect to server [IP]:[PORT]/[URI]```
* If the URI is faulty it will display:\
```Server returned an error while accessing [URI]```
### Soft errors
* HTTP request returns not a 200 OK
* one or more data point are missing

## Epaper Library
The library contains 3 sub libraries:\
* First layer: The Hardware library i.e. How to acces the microcontrollers spi interface, initializing the pins, etc.
* Middle layer: The commands for the specific display i.e. initializing, displaying pixels, etc.
* Top layer: The draw functions that are used to display more elaborate things then pixels ex. Lines, text, images


## HTTPS GET/POST library
