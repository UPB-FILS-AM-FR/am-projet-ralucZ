# Digital Clock

| | |
|-|-|
|`Author` | Zarnescu Raluca-Maria

## Description
For this project, I aim to create a versatile digital clock using readily available components like the Raspberry Pi Pico microcontroller, an OLED display (such as SSD1306), and optionally, a Real-Time Clock (RTC) module for precise timekeeping. 
The digital clock will not only display the current time but also offer additional features like showcasing the date, temperature, and humidity readings. 
## Motivation
The motivation behind this project is to provide an accessible and educational example of integrating hardware components with the Raspberry Pi Pico microcontroller. 
By designing a digital clock, we aim to demonstrate the practical application of microcontrollers in everyday devices, 
Additionally, by incorporating features such as temperature and humidity readings, we enhance the utility of the digital clock, making it more than just a timekeeping device.
## Architecture

### Block diagram

<!-- Make sure the path to the picture is correct -->
![Block Diagram](schema_finala.png)

### Schematic

![Schematic](schematics/kicad_schematic.png)

### Components

| Component | Description |
|---------|-------------|
| Placa UNO R3 | Main microcontroller board
| 16x2 LED display |	Display module for showcasing time, date, and more	
| RTC Module (DS1307 I2C) |	Module for accurate timekeeping	
| DHT11 Temperature and Humidity Sensor |	Digital sensor for environmental data	
| Push Buttons or Rotary Encoder |	Input devices for user interaction	
| Breadboard and Jumper Wires	| Prototyping tools for circuit connections	
| Resistors and Capacitors |	Passive components for signal conditioning	
| Power Source | USB power adapter or battery pack	
| Enclosure | Protective housing for the project	
| Header Pins	| Male or female connectors for wiring	
| LEDS | Indicating different humidity levels


### Libraries

<!-- This is just an example, fill in the table with your actual components -->

| Library | Description | Usage |
|---------|-------------|-------|
| LiquidCrystal | Library for controlling liquid crystal displays (LCDs) in Arduino projects  |  Used for controlling Liquid Crystal Displays (LCDs) by initializing the display and updating content. |
| DHT | Library for interfacing DHT series temperature and humidity sensors with Arduino | Used for accessing temperature and humidity data from DHT series sensors, initializing them, and retrieving sensor readings. |

## Log

<!-- write every week your progress here -->

### Week 6 - 12 May

### Week 7 - 19 May

### Week 20 - 26 May


## Reference links

<!-- Fill in with appropriate links and link titles -->

[Tutorial 1](https://ro.onetransistor.eu/2017/12/indice-confort-termic-dht11-arduino.html)

[Article 1](http://nicuflorica.blogspot.com/)

[Article 2](http://arduinotehniq.blogspot.com/)

