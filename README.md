# Projet Capteur  
This project is a Master's degree project at INSA - Toulouse.

  - [Installation](#installation)
  - [Usage](#usage)
  - [Arduino Code](#arduino-code)
  - [Shield](#shield)
  - [APK](#apk)
  - [Datasheet](#datasheet)
  - [Credits](#credits)
  - [Roadmap](#roadmap)

## Installation

  * Use the Gerber folder to realise the Arduino Shield.  
  * Once you have it use the datasheet scheatics to solder all the component in place.  
  * Once the shield is complete plug it onto your Arduino Uno.  
  * Transfer the program into the arduino.  
  * Then you're good to go, you can start measuring with your sensor.  

## Usage

The Arduino is able to measure the tension and the resistance of your sensor as is, 
but if you want to measure the deflection you will need to use the 2 points calibration menu,
also known as "Etalon 2 points". Once in this menu you have to press the roto encoder's button once to calibrate the resistance at rest of your sensor, 
then twice to calibrate the resistance for two known deflections. Use the roto encoder to define the deflection. Once the calibration is done, you can check the results in the calibration menu, also known as "Menu Calibration".

## Arduino Code
All the Arduino code is available in the GitHub "Projet" folder, all of it is commented and can be modified as wanted to.

## Shield
All the details relative to the shield are available in the "Shield Uno" folder and the "Gerber" folder, it can be modified as wanted to by using Kicad.

## APK
The APK was made using MIT AppInventor, you will find the APK file in the Master folder of the GitHub as well as the .aia. You can import the .aia to MIT AppInventor to modify it.

## Datasheet
All the details concerning the sensor is available in the datasheet, it's available in .pdf and in .xlsx .

## Credits
BOULARD Tanguy  
BOURDON Léo  
TARADE Manon  

## Roadmap
  - [x] Realize the shield
  - [x] Realize a first version of the Arduino Code
  - [x] Realize the APK
  - [x] Create a datasheet of the sensor
  - [x] Add a 2 points calibration menu in the Arduino Code
  - [ ] Add a Test Bench menu on the Arduino
  - [ ] Make the sensor more reliable
