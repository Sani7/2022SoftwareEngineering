# Project

## Requirements
### Hardware
* display (Waveshare 3.7inch e-Paper HAT)
* ESP32 Developement Board
* Rotary Encoder
### Software in C/C++
* Connecting to wifi via specified credentials in the menuconfig (see documentation)
* User input => Rotary encoder + Button to switch between modes (day, week, month, year)
    * A > B (Left): day > year > month > week > day
    * B > A (Right): day > week > month > year > day
    * Button select specified mode
    * Then the display refreshes the data
* Database input => from a HTTP get (Get Parameter => mode) request that reads out a database via a php script 
* Output => Line graph of requested data on the E-paper display for this project it is the consumtion data of an electricity meter
* The graph will refresh every 5 minutes if there is no change in mode

## Usecase diagram

start-up ------------------------------------------------------------------------------>graph---------------\
        &emsp;&emsp;&emsp;&emsp;&emsp;|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;|\
        &emsp;&emsp;&emsp;wifi error&emsp;&emsp;server not found&emsp;&emsp;&emsp;&emsp;bad uri

## Documentation

See [Documentation](./Docs.md)

The requirement should be more elaborate. How does the user interact with the rotary encoder, what happens, what is displayed. What kind of information will be displayed? How accurate is the data? How often is the data refreshed? 