# Project

## Requirements
### Hardware
* display (Waveshare 3.7inch e_Paper HAT)
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
* Output => Line graph of requested data on the E_paper display for this project it is the consumtion data of an electricity meter
* The graph will refresh every 5 minutes if there is no change in mode
* What is displayed for this project is the electricity usage, but It can be anything.
* The data is send to the database every minute and is averaged out via the php script to 288 Datapoints
    * Day: 24 * 60 / 5
    * Week: 7 * 24 * 60 / 35
    * Month: 31* 24 * 60 / 155
    * Year: 355 * 24 * 60 / 1775

## Usecase diagram

start_up ________________________________________________________\_\_\_\_ get data \_\_\_\_\_\_\>\_\_\_\_\_\_ graph \_\_\_\_\_\_\_>\_\_\_\_\_\_\_\_ Chosing mode\
        &emsp;&emsp;&emsp;&emsp;&emsp;|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;|&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;|\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_<\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_|\
        &emsp;&emsp;&emsp;wifi error&emsp;&emsp;server not found&emsp;&emsp;&emsp;&emsp;bad uri&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;Or 5 minutes past

## Documentation

See [Documentation](./Docs.md)

The requirement should be more elaborate. How does the user interact with the rotary encoder, what happens, what is displayed. How accurate is the data?