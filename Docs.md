# Documentation
## Config File
How it is constructed:
[ConfigName]=[Input]'\n'\
And consists of:
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
* If the device can't connect to the Wifi it will show a message on the screen:\
```Cant connect to [SSID]```
* If the server isn't accesible it will display:\
```Can't connect to server [IP]:[PORT]/[URI]```
* If the URI is faulty it will display:\
```Server returned an error while accessing [URI]```