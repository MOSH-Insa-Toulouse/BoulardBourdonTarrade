//=============================Bluetooth Module=====================
#include <SoftwareSerial.h>

#define rxPin 13
#define txPin 12
#define baudrate 9600
SoftwareSerial mySerial(rxPin,txPin);

//=============================Roto Encoder=========================
#define encoderPinA  2  //The clock signal of the Roto Encoder must be on the pin 2 since we want to use an attachInterrupt on it 
#define encoderPinB  10  //DT signal of the Roto Encoder
#define Switch 11 //SW signal of the Roto Encoder

int aState; //State of the pin A of the Roto Encoder
int aLastState;  //Last state of the pin A of the Roto Encoder
int btnState = HIGH; //State of the button 
int btnLastState = HIGH; //Last state of the button

//=============================OLED Screen==========================
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 //OLED display width, in pixels
#define SCREEN_HEIGHT 32 //OLED display height, in pixels

#define OLED_RESET -1 //Reset pin shared with the arduino reset button
#define SCREEN_ADDRESS 0x3C //Adress of the OLED Screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//=============================Measure==============================
#define pinAnalog A0 //Pin used to measure the Sensor

//Var used to measure
float mesure = 0.0; //Deflection
float valeur = 0.0; //Tension (0 = 0V; 1024 = 5V)
float resistance = 0.0; //Resistance of the Sensor
float moyenne[10]; //Last ten values of the tension
float moy = 0.0; //Mean value of the tension
float R0 = 0.0; //Resistance at rest of the Sensor

//we suppose that deflection = Slope * DeltaR + Offset
float sensi = 8.55; //Slope of the Sensor 
float offset = -25.1; //Offset of the Sensor

//Var used for the two points calibration
int etalonnage2Points = 0; 
float courbure = 0.0;
float mesurePoint1 = 0.0;
float mesurePoint2 = 0.0;
float courburePoint1 = 0.0;
float courburePoint2 = 0.0;

//=============================Timer================================
int rotoT0 = millis();
int rotoTimer = millis();

int T0 = millis();
int Timer = millis();

//=============================Menu=================================
int mode = 1; //Var that tells in which menu you are
int selection = 1; //Var used to select which menu you want to go to

//=============================Setup================================
/*Function used at the start of the Arduino*/
void setup(){
  //Initialization of the Bluetooth module
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(baudrate);
  
  //Initialization of the Roto Encoder
  pinMode(encoderPinA, INPUT);
  digitalWrite(encoderPinA, HIGH);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinB, HIGH);
  attachInterrupt(digitalPinToInterrupt(2), doEncoder, RISING);
  aLastState = digitalRead(encoderPinA);

  pinMode(Switch, INPUT_PULLUP);

  //Initialization of the OLED Screen
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
  delay(2000);
  
  for(int i = 0; i < 60; i++){
    Introduction(i);
  }
  delay(2000);
  
  Serial.begin(baudrate);

  //We measure the resistance at rest of the Sensor
  valeur = (float)(analogRead(pinAnalog) * (float)(5.0/1024.0));
  R0 = (float)((1.0+100000.0/1000.0)*100000.0*(5.0/valeur)-100000.0-10000.0)/1000000.0 ;
}

//=============================Main Loop============================
/*Function used every X ms*/
void loop() {
  //Updating timers
  rotoTimer = millis();

  //We measure the tension on the pin A0
  valeur = (float)(analogRead(pinAnalog) * (float)(5.0/1024.0));

  //We store the value in a table which contains the last ten values of the tension
  for(int j = 0; j < 9;  j++){
    moyenne[j] = moyenne[j+1];
  }
  
  //We calculate the average value of this table
  moyenne[9] = valeur;
  moy = 0.0;
  for(int j = 0; j < 10;  j++){
    moy += moyenne[j];
  }
  moy = (float)moy/10.0;

  //We calculate the value of the resistance
  if(moy != 0){
    resistance = (float)((1.0+100000.0/1000.0)*100000.0*(5.0/moy)-100000.0-10000.0)/1000000.0 ;
  }else{
    resistance = -1;
  }
  mesure = (float)((float)((resistance-R0)/R0)*100.0 * sensi) + offset;

  //We send the value to the computer
  if(Serial.available()){Serial.println(resistance);}

  int i = 0;
  char textBluetooth[32] = {0};
  /*Communication from the bluetooth module to the arduino*/
  while (mySerial.available()){ //While the com is open
    i = 0;
    do{ //We read what the bluetooth module send to the arduino
      textBluetooth[i++] = mySerial.read();
      delay(3);
    }while(mySerial.available());
    //if the message read is "mesure" it means the phone is waiting for the resistance's value
    String test = (String)textBluetooth;
    test.trim();
    if(test == "mesure"){
      //We send the resistance's value to the bluetooth module
      mySerial.print(resistance);
    }
  }

  //We uptdate the button state
  btnLastState = btnState;
  btnState = digitalRead(Switch);

  /*Switch that draws on the screen the right menu*/
  display.clearDisplay();
  display.drawRect(1, 1, 126,31, WHITE);
  switch (mode){
    case 1:
      //Measurement menu
      MenuMesure();
      if(btnState == LOW and btnLastState != LOW){mode = 5;}
      break;
    case 2:
      //Calibration menu
      MenuEtalonnage();
      if(btnState == LOW and btnLastState != LOW){mode = 5;}
      break;
    case 3:
      //2 points calibration menu
      MenuEtalonnage2Points();
      if(btnState == LOW and btnLastState != LOW){
        etalonnage2Points += 1;
        if(etalonnage2Points == 3){
          mode = 5;
          etalonnage2Points = 0;
          //Once the calibration is done we calculate the slope and the offset
          sensi = ((((mesurePoint2 - mesurePoint1)/R0)*100.0)/(courburePoint2 - courburePoint1));
          offset = (mesurePoint1 - (sensi * courburePoint1/R0));
          selection = 1;
        }
      }
      break;
    case 5:
      //Menu in which you can choose which menu to go to 
      MenuChoix();
      if(btnState == LOW and btnLastState != LOW){mode = selection; etalonnage2Points = 0; courbure = 0;}
      break;
    default:
      MenuMesure();
      if(btnState == LOW and btnLastState != LOW){mode = 5;}
      break;
  }
}


//=============================Roto Encoder=========================
/*Function that detects when the Roto Encoder spins*/
void doEncoder() {
  if(rotoTimer - rotoT0 > 50 ){
    if (digitalRead(encoderPinB) != digitalRead(encoderPinA) and digitalRead(encoderPinB) != digitalRead(encoderPinA)){
      selection ++;
      switch (mode){
        case 3:
          courbure+=0.25;
          break;
        default:
          break;
        }
    }else{
      selection --;
      switch (mode){
        case 3:
          courbure-=0.25;
          break;
        default:
          break;
      }
    }

    if(selection > 3){selection = 1;}
    if(selection < 1){selection = 3;}
    rotoT0 = millis();
  }
}

//=============================OLED Text Drawing====================
/*Function that draws text on the OLED Screen*/
void Affichage(String text, int x, int y, int taille, boolean actualiser){
  display.setTextSize(taille);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(actualiser){
    display.display();
  }
}

//=============================Measurement Menu=====================
/*Function that draws the measurement menu every 300 ms*/
void MenuMesure(){
  if(Timer - T0 > 300){
    Affichage("Tension :", 4, 3, 1, false);
    String text = String(valeur * 1000.0);
    String text1 = String(resistance);
    if(resistance == -1){text1 = "inf";}    
    String text2 = String(mesure);
    Affichage(text, 60, 3, 1, false);
    Affichage("mV", 100, 3, 1, false);
    Affichage("Res:", 4, 11, 1, false);
    Affichage(text1, 40, 11, 1, false);
    Affichage("MOhm", 100, 11, 1, false);
    Affichage(text2, 60, 21,1,false);
    Affichage("Courbure:", 4, 21, 1, false);
    Affichage("mm", 100, 21, 1, true);
    T0 = millis();
  }
  Timer = millis();
}

//=============================Main Menu============================
/*Function that draws the main menu*/
void MenuChoix(){
  Affichage("Menu Mesure", 4, 3, 1, false);
  Affichage("Menu Calibration", 4, 11, 1, false);
  Affichage("Etalon 2 Points", 4, 21, 1, false);
  switch (selection){
    case 1:
      Affichage("<", 110, 3, 1, true);
      break;
    case 2:
      Affichage("<", 110, 11, 1, true);
      break;
    case 3:
      Affichage("<", 110, 21, 1, true);
      break;
    default:
      Affichage("<", 110, 3, 1, true);
      break;
  }
}

//=============================Calibration Menu=====================
/*Function that draws the calibration menu*/
void MenuEtalonnage(){
  String text1 = String(sensi);
  String text2 = String(offset);
  Affichage("Sensi:", 4, 3, 1, false);
  Affichage(text1, 72, 3, 1, false);
  Affichage("Offset:", 4, 11, 1, false);
  Affichage(text2, 72, 11, 1, false);
  String R0string = String(R0);
  Affichage("R0:", 4, 21, 1, false);
  Affichage(R0string, 72, 21, 1, false);
  Affichage("MOhm", 100, 21, 1, true);
}

//=============================2 Points Calibration Menu============
/*Function that draws the 2 points calibration menu*/
void MenuEtalonnage2Points(){
  String text = String(valeur * 1000.0);
  if(etalonnage2Points == 0){
    R0 = resistance;
    String R0string = String(R0);
    Affichage("R0:", 4, 3, 1, false);
    Affichage(R0string, 72, 3, 1, false);
    Affichage("MOhm", 100, 3, 1, false);
    Affichage("Appuyez", 4, 11, 1, false);
    Affichage("Mesure:", 4, 21, 1, false);
    Affichage(text, 72, 21, 1, false);
    Affichage("mV", 110, 21, 1, true);
  }else{
    String text1 = String(courbure);
    Affichage("Courbure :", 4, 3, 1, false);
    Affichage(text1, 72, 3, 1, false);
    Affichage("Appuyez", 4, 11, 1, false);
    Affichage("Mesure:", 4, 21, 1, false);
    Affichage(text, 72, 21, 1, false);
    Affichage("mV", 110, 21, 1, true);  
  }
  if(etalonnage2Points == 1){
    mesurePoint1 = resistance;
    courburePoint1 = courbure;
  }
  if(etalonnage2Points == 2){
    mesurePoint2 = resistance;
    courburePoint2 = courbure;
  }
}

//=============================Introduction=========================
/*Function that display our team's name*/
void Introduction(int i){
  display.clearDisplay();
  Affichage("  LES ROUTEURS DE", i-64, 7, 1, false);
  Affichage("     L'EXTREME", i-64, 15, 1, false);
  display.fillRect(i-64, 23, display.width()-8, 8, SSD1306_INVERSE);
  display.setTextColor(BLACK);
  display.setCursor(i-64,23);
  display.setTextSize(0.5);
  display.println("            Bouf INC");
  display.display();
}
