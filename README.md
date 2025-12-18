# W124-CAN-Sensor-Network

I started this Project, because I want to add multiple sensors to my 1993 Mercedes Benz W124 200E (M111) and want to display the measured values in the dashboard.

I am also converting from Viscous fan to Electric Fan, because my viscous coupling was dead and I HATE viscous fans.

After some consideration I decided to connect the different Modules to one another via CAN-Bus, because it is the easiest and most reliable way to do. (The whole car industry uses CAN for everything in every car)
For now I am designing these 3 Modules:
1. Fan Control + Coolant Temp Module(Measures the Coolant Temp and controls the Fan accordingly. I designed this module with both functions so it can be used standalone)
2. Sensor Module (It reads multiple sensors(2x PT100 Temp(Engine Oil and Transmission Oil) and up to 4 analog Sensors(0-5V)(I am currently only using one for the Oil pressure)
3. Display Module (Displays the different measured values. Will replace the outside Temp Display. Also electronically measures the speed and outside Temp and sends in to the CAN-Bus.)

In the Future I thought about building the following modules:
1. Control Module (Replaces the Fader Switch(Überblendregler) and has some sort of controls(Buttons or joystick) to switch between different screens or change settings)
2. IMU(Inertial Measurement Unit) Module(measures G-Forces, Rotation, maybe the earths magnetic field to know exact absolute orientation)
3. GPS Module (Measures exact GPS Speed and position to display on gauge cluster display and to have precise Time)
4. Logging Module (Logs all configured data with precise Timestamps)


I have also programmed the fan module to read the automatic transmissions Temp via the CAN-Bus and let the fan run even if the engine is not too hot, but the Automatic transmission is too hot.

The hardware currently used:
Arduino Nanos for the modules
PT100 Temp Sensors
MAX31865 PT100 Sensor Amplifiers
MCP2515 CAN Controllers
TJA1050 CAN Transceivers
ADS1115 ADC for reading analog Sensors (Pressure)
128x64 SSD1306 OLED Display

What Hardware I want to add:
Impulse counter to the speed output of the gauge cluster that goes into the Display Module.
Analog Circuitry to measure original outside Temp Sensor


What Hardware I want to change:
ESP32 for the Display Module, to make it easy to configure through a WebUI
Pi Pico for all other Modules because they are very fast and cheap.
Change 128x64 SSD1306 to two 128 x 72 SSD1320 Displays side by side.
They should fill out the visible Area of the Outside Temp Display cutout perfectly while still fitting into the original area in the Gauge Cluster

If you are interested in helping on this project, feel free to make PRs, open issues and contact me via Discord @criscrafter


Libraries I used:
https://github.com/autowp/arduino-mcp2515
https://github.com/wollewald/ADS1115_WE
https://github.com/adafruit/Adafruit_SSD1306
https://github.com/adafruit/Adafruit_MAX31865
