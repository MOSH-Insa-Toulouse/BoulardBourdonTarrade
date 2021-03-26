# Projet Capteur  

This project is a research project submitted in partial fulfilment of the requirement for the award of the Physics Engineering's Degree at INSA - Toulouse.

  - [Installation](#installation)
  - [Usage](#usage)
  - [Arduino Code](#arduino-code)
  - [Shield](#shield)
  - [APK](#apk)
  - [Datasheet](#datasheet)
  - [Credits](#credits)
  - [Roadmap](#roadmap)

## Installation

  * Use the Gerber folder to create the Arduino Shield.  
  * Once created, use the datasheet schematics to solder all the components onto the shield.  
  * When the shield is completed, plug it onto an Arduino Uno.  
  * Transfer the program, found in Project in the master level, into the Arduino.
  * You are then good to go! You can now start measuring different electrical resistances with your sensor. 
  * In addition to displaying the datas onto the OLED screen, you can also choose to display them on the Serial Plotter of the Arduino, or you can download our mobile    phone app (see section APK). 

## Usage

The Arduino Uno is able to measure the tension and the resistance of your sensor as is. 

However, if you want to measure the deflection of the sensor, you will need to use the 2 points calibration menu, named "Etalon 2 points". 
First, simply straighten the sensor and press the rotary encoder once. This will calibrate the resistance at the sensor's rest position (R0). 
Afterwards, rotate the same encoder to a chosen angle of deflection, before applying said angle onto the sensor. Once applied, press the encoder once. Repeat this step and press the encoder again to finalise the calibration. 

After completing the calibration, all calibration values (R0, slope and offset) can be found in the menu entitled "Menu Calibration".

## Arduino Code

The full Arduino code is available in the GitHub "Projet" folder, it is entirely commented and can be modified at will.

## Shield

All the details related to the shield are available in the "Shield Uno" and the "Gerber" folders, which can be modified at will by using the electronic design automation software called KiCad.

## APK

The mobile application, defined by its extension .apk, was created using the tools on MIT's AppInventor website. If necessary, the application can be modified using this same tool. However, to do so, you will need the .aia file used to create the application. Both the .apk and .aia files can be found in the Master level of the GitHub.

## Datasheet

All the details concerning the sensor is available in our datasheet, which is composed of a primary pdf file and a complementary Excel sheet (.xlsx).

## Created by

BOULARD Tanguy  
BOURDON Léo  
TARRADE Manon  

## Roadmap

  - [x] Build the shield
  - [x] Program a first version of the Arduino Code
  - [x] Create the mobile application which displays and saves the datas measured by the sensor
  - [x] Create the datasheet of the sensor
  - [x] Add a 2 points calibration menu in the Arduino Code
  - [ ] Add a Test Bench menu on the Arduino
  - [ ] Make the sensor more reliable
