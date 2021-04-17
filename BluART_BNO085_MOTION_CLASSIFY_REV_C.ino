

#include <Adafruit_BNO08x.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"


//------------------SETUP for 9DOF Sensor -----------------------
// For SPI mode, we need a CS pin
#define BNO08X_CS 10
#define BNO08X_INT 9

//RESET Pin for I2C 
#define BNO08X_RESET -1

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;


//-------------------SETUP for Feather M0 Bluefruit LE----------------
#define FACTORYRESET_ENABLE 1
#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#define MODE_LED_BEHAVIOUR "MODE"

#define BLUEFRUIT_SPI_CS 8
#define BLUEFRUIT_SPI_IRQ 7
#define BLUEFRUIT_SPI_RST 4 

#define BUFSIZE 128   // Size of the read buffer for incoming data
#define VERBOSE_MODE true  // If set to 'true' enables debug output


/*Configure Hardware SPI to communicate with Bluefruit using SCK/MOSI/MISO hardware SPI pins on 
nRF52xxx internally connected and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}


void setup(void)
{
  delay(500);
  //-------------Initialize Bluefruit Module for communication-------------------
  //Module initializes in Command Mode, allowing to communicate Hayes AT commands
  Serial.begin(115200);
  Serial.print(F("Initialising Porch Pirate IoT BLE module: "));

  if (!ble.begin(VERBOSE_MODE)) //Enables Debug output to ensure proper initialization
  {
    error(F("Couldn't find Bluefruit, make sure it's in Command mode"));
  }

  if (FACTORYRESET_ENABLE) //Perform Factory Reset at startup to ensure module is in a known state
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if (!ble.factoryReset()){
      error(F("Couldn't factory reset"));
    }
  }

  //Disable command echo from module
  ble.echo(false);

  //Pings Bluefruit for information
  Serial.println("Requesting Bluefruit information:");
  ble.info();
  ble.verbose(false);  //Disable debug info after proper intiialization

  //Wait until ble module is connected to Bluefruit App or equivalent client
  while (!ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("---------------------------------"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR); //Default Mode inidicates operating condition indication
    //Can also be set to BLEUART mode to show activity on BLE UART
  }

  // Set module to DATA mode which make module a transparent UART module, allowing direct bridge to Bluefruit App
  Serial.println(F("Switching to DATA mode!")); 
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("Module Initialized Properly, Ready for Pairing"));
  Serial.println(F("----------------------------------"));

  //---------------------------------------------------------------
  //----------------------9 DOF Sensor Setup-----------------------
  
  // Initialize the sensor with I2C
  if (!bno08x.begin_I2C()) {
    Serial.println("Unable to detect BNO085");
    while (1) {
      delay(10);
    }

    Serial.println("BNO085 Detected");
    Initialize_Reports();
  
    Serial.println("Reading events");
    delay(100);
    }
}





void Initialize_Reports(void) {
  //Enables Accelerometer reading reports
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  }

  //Enables stability classification report (Stable, On Ground, In Motion)
  if (!bno08x.enableReport(SH2_STABILITY_CLASSIFIER)) {
    Serial.println("Could not enable stability classifier");
  }

  //Enables shake detection report, TO BE TESTED
  if (!bno08x.enableReport(SH2_SHAKE_DETECTOR)) {
    Serial.println("Could not enable shake detector");
  }
}


/*------------TODO---------
 * Recieve User Input through UART from Bluefruit App (Blueprint for user input already listed below)
 * Set Device in ARM and DISARM mode of those strings are recieved
 * Device will loop and check for State bool of ARM is true or false, and 
 * sets 9DOF sensor in standby mode or active more (Check Datasheet for low power/deepsleep mode)
 */
void loop(void)
{
//Check to see if the BNO085 sensor was hard reset, if so, reinitialize reports
  delay(200);
  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    Initialize_Reports();
  }

  //Check what this is for
  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

  //Listen for sensorID's recieved from 9DOF sensor and match it to cases below
  if(sensorValue.sensorId){
    if(SH2_STABILITY_CLASSIFIER){ //If SensorID Returns Stability Classifier 
      sh2_StabilityClassifier_t stability = sensorValue.un.stabilityClassifier;
      switch (stability.classification) {
        case STABILITY_CLASSIFIER_UNKNOWN:
          ble.println("Unknown");
          break;
        case STABILITY_CLASSIFIER_ON_TABLE: //This classification is when there is no movement on any axis, and the device is place somewhere
          ble.println("On Table/Ground");
          break;
        case STABILITY_CLASSIFIER_STABLE: //This classification activates when the device is in a constant acceleration with little deviation, but not in motion, EX: Held steady by hand
          ble.println("Stable");
          break;
        case STABILITY_CLASSIFIER_MOTION: //This classification activates when the device has considerable changes in any axes
          ble.println("In Motion");
          break;
      }
  }

  /*-----------TODO: SECTION TO RECIEVE USER INPUT for ARMING/DISARMING Device after connection---------------
  // Check for user input
  char n, inputs[BUFSIZE+1]; //UART Buffer Size

  if (Serial.available())
  {
    n = Serial.readBytes(inputs, BUFSIZE); //Decode the characters and push these to the the nRF51 module
    inputs[n] = 0;
    // Send characters to Bluefruit
    Serial.print("Sending: ");
    Serial.println(inputs);

    // Send input data to host via Bluefruit
    ble.print(inputs);
  }
  // This function executes when ble has recieved input from user

  while ( ble.available() )
  {
  }*/


  
  delay(500);
  }
}
