# Porch Pirate IoT BLE Device

# Description
Porch Pirate IoT BLE Device for Senior Design Project Semester 1 @ Wichita State University. This product allows the Wichita Police Department to track stolen assets/packages and retrieve them. This device is an IoT node that can be placed directly onto critical assets/packages to track the status of the package and alert interested parties through a bluetooth application for both iPhone (Need Version) and Android 10+. 

# Parts List
* Adafruit BNO085 9 DOF Breakout Board
* Adafruit Feather M0+ Bluefruit BLE
* 3.7 - 4.2 V LiPoly Battery

# Method of Operation
This device uses a Adafruit BNO085 9 DOF sensor to percieve its current state and translates the raw data using the Adafruit Sensor Libraries into motion classifications such as On Table/Ground, Stable, and In Motion. The data is then forwarded to the Adafruit Feather M0 BLE where it is processed and printed through UART to any device that is has an active ongoing connection to the nRF51822 BLE chip that is onboard the Feather M0 through the Bluefruit Connect Application.


# Schematic
Shown below is the schematic for the Semester 1 Version of the device. Please be mindful of the + and - wires when connecting the LiPoly battery to the JST connectors on the Feather the wires may be reversed in some cases and would need to be switched. The proper configuration can be found on the Adafruit Feather M0 Guide page linked below a seperate repo will be made to reflect the changes of Semester 2.
![image](https://user-images.githubusercontent.com/69644136/115117631-3256e180-9f65-11eb-9061-1e0749e0403c.png)

# Dependancies
The following dependancies need to be installed in for proper operation of the device, this device uses the Arduino IDE and other Adafruit Libraries for functioning properly.
* Arduino IDE
* Adafuit BLE Library
* Adafruit SAMD21 Boards Library (Feather M0 IDE Setup)
* Adafruit BNO08x Library


# Installation
To properly install the dependacies, please follow the guides linkes below.
* [Arduino IDE 1.8.13](https://www.arduino.cc/en/software)
* [Adafruit Feather M0 IDE Setup](https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/setup)
* [Adafruit Feather M0 BLE Library Setup](https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/installing-ble-library)
* [Adafruit BNO08x Library Setup](https://learn.adafruit.com/adafruit-9-dof-orientation-imu-fusion-breakout-bno085/arduino)

# Authors
Abishek Gomes, Devan Mears, Joseph Wackowski, Austin Major

