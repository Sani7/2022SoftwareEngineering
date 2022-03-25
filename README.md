# Project

## Requirements
### Hardware
* display (Waveshare 3.7inch e_Paper HAT)
* ESP32 Developement Board
* Rotary Encoder\
[diagram](./Docs/Hardware.pdf)
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

[Usecasediagram](./Docs/UsecaseDiagram.pdf)

## Function List
* Draw a graph on the display
    * Draw y and x-axis with information about what is on the axis
    * Draw Title
    * Draw Draw data
* Get Data from PHP server
    * Send a request with mode (day, week, month, year)
    * Receive data
    * Convert data to int and store in an array
* Mode select via a rotary encoder
    * Read rotary encoder via interrupt
    * change data accordingly
* Read config from sd card
    * Mount sd card to file system
    * use stdio functions for reading data
    * convert lines to config data
    * unmount\
[function diagram](./Docs/UsecaseDiagram.pdf)

## Tasklist
* Graph
    * SPI interface
    * Low-level system functions for e-paper
    * High-level functions to draw on the e-paper
* Config
    * read sd card
    * convert string to config data
* HTTP Request
    * Connect to WIFI
    * Create URI
    * Process HTTP Request
    * process data
* Rotary encoder
    * ISR
    * Software Deboucing
    * change mode
* Data conversion
    * String splitting on identifier
    * convert string to int

## Flow diagram
[doc](./Docs/Flowdiagram.pdf)

## Documentation

See [Documentation](./Docs.md)