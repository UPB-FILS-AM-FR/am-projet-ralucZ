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
schema.png

### Schematic

![Schematic](schematics/kicad_schematic.png)

### Components

| Component | Description |
|---------|-------------|
| Raspberry Pi Pico	| Main microcontroller board with RP2040 chip	
| OLED Display (SSD1306) |	Display module for showcasing time, date, and more	
| RTC Module (DS3231) |	Optional module for accurate timekeeping	
| DHT22 Temperature and Humidity Sensor |	Digital sensor for environmental data	
| Push Buttons or Rotary Encoder |	Input devices for user interaction	
| Breadboard and Jumper Wires	| Prototyping tools for circuit connections	
| Resistors and Capacitors |	Passive components for signal conditioning	
| Power Source | USB power adapter or battery pack	
| Enclosure | Protective housing for the project	
| Header Pins	| Male or female connectors for wiring	


### Libraries

<!-- This is just an example, fill in the table with your actual components -->

| Library | Description | Usage |
|---------|-------------|-------|
| Adafruit SSD1306 | Library for controlling SSD1306-based OLED displays  | Used for interfacing with the OLED display, initializing it, and updating display content. |
| Adafruit CircuitPython DS3231 | Library for interacting with DS3231 RTC (Real-Time Clock) modules | Used for accessing and managing the RTC module, retrieving time and date information, and synchronizing the clock display.  |

## Log

<!-- write every week your progress here -->

### Week 6 - 12 May

### Week 7 - 19 May

### Week 20 - 26 May


## Reference links

<!-- Fill in with appropriate links and link titles -->

[Tutorial 1](https://www.youtube.com/watch?v=wdgULBpRoXk&t=1s&ab_channel=BenEater)

[Article 1](https://www.explainthatstuff.com/induction-motors.html)

[Link title](https://projecthub.arduino.cc/)
