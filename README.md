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
  * Once you have it, use the datasheet schematics to solder all the component together and create your own sensors.  
  * Once the shield is completed, plug it onto an Arduino Uno.  
  * Transfer the program into the arduino.
  * You are then good to go, you can start measuring different electrical resistences with your sensor. 
  * In addition to the LCD screen display of the datas, you can then choose to display them too on the Serial Plotter of Arduino, or you can download our app to display it on your mobile phone (see section APK). 

## Usage

The Arduino Uno is able to measure the tension and the resistance of your sensor as is, 
but if you want to measure the deflection you will need to use the 2 points calibration menu,
named "Etalon 2 points". Once in this menu you will have to press the roto encoder's button once to calibrate the resistance at rest of your sensor, 
then press twice to calibrate the resistance for two known deflections. Use the roto encoder to define the deflection. Once calibration is done, you can check the results in the calibration menu, named "Menu Calibration".

## Arduino Code
The full Arduino code is available in the GitHub "Projet" folder, it is entierly commented and can be modified as wished.

## Shield
All the details relative to the shield are available in the "Shield Uno" and the "Gerber" folders, it can be modified as wished by using the electronic design automation software called Kicad.

## APK
The mobile application, .apk, was created by using the tools on MIT AppInventor's website, you will find the APK file in the Master folder of the GitHub as well as the .aia file. You can import the .aia to the website MIT AppInventor, once youhave created an account, to modify and download it on your mobile phone.

## Datasheet
All the details concerning the sensor is available in our datasheet, which is composed of the pdf file (mainly) and the xlsx sheet (as a complement).

## Credits go to
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
