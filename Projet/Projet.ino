//=============================Module Bluetooth=====================
#include <SoftwareSerial.h>

#define rxPin 13
#define txPin 12
#define baudrate 9600
SoftwareSerial mySerial(rxPin,txPin);

//=============================RotoEncodeur=========================
#define encoderPinA  2  //le signal d'horloge du rotoEncodeur doit être calbé sur le pin 2 pour y mettre l'interrupt
#define encoderPinB  10  //DT du rotoEncodeur
#define Switch 11 // SW du rotoEncodeur

int aState; //état du pin A du rotoEncodeur
int aLastState;  //état précedent du pin A du rotoEncodeur
int btnState = HIGH; //état du bouton
int btnLastState = HIGH;
//=============================écran Oled===========================
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels


// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//=============================Mesure===============================
#define pinAnalog A0
float mesure = 0.0;
float valeur = 0.0;
float resistance = 0.0;
float moyenne[10];
float moy = 0.0;
float R0 = 1000; 

float sensi = 8.55;
float offset = -25.1;
int etalonnage2Points = 0;
float courbure = 0;
float mesurePoint1;
float mesurePoint2;
float courburePoint1;
float courburePoint2;
//=============================Timer================================
int btnT0 = millis();
int btnTimer = millis();

int rotoT0 = millis();
int rotoTimer = millis();

int T0 = millis();
int Timer = millis();
//=============================Menu=================================
int mode = 1;
int selection = 1;
int varSelection = 0;

/*Fonction Setup appelée une fois au démarrage*/
void setup(){
  //Initialisation du module Bluetooth
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(baudrate);
  
  //Initialisation du rotoEncodeur
  pinMode(encoderPinA, INPUT);
  digitalWrite(encoderPinA, HIGH);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinB, HIGH);
  attachInterrupt(digitalPinToInterrupt(2), doEncoder, RISING); //On attache l'exécution de la fonction "doEncoder" lors de la détection d'un front montant sur le pin digital 2
  aLastState = digitalRead(encoderPinA);

  pinMode(Switch, INPUT_PULLUP);

  //Initialisation de l'écran Oled
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
  delay(2000);
  Introduction();
  delay(2000);
  
  // Clear the buffer
  display.clearDisplay();
  
  Serial.begin(baudrate);
  valeur = (float)(analogRead(pinAnalog) * (float)(5.0/1024.0));
  R0 = (float)((1.0+100000.0/1000.0)*100000.0*(5.0/valeur)-100000.0-10000.0)/1000000.0 ;
}


/*Fonction loop appelée en continue*/
void loop() {
  //On met à jour le timer
  btnTimer = millis();
  rotoTimer = millis();

  int i = 0;
  char textSerial[32] = {0};
  char textBluetooth[32] = {0};

  valeur = (float)(analogRead(pinAnalog) * (float)(5.0/1024.0));
  
  for(int j = 0; j < 9;  j++){
    moyenne[j] = moyenne[j+1];
  }
  moyenne[9] = valeur;
  moy = 0.0;
  for(int j = 0; j < 10;  j++){
    moy += moyenne[j];
  }
  moy = (float)moy/10.0;
  
  if(moy != 0){
    resistance = (float)((1.0+100000.0/1000.0)*100000.0*(5.0/moy)-100000.0-10000.0)/1000000.0 ;
  }else{
    resistance = -1;
  }
  mesure = (float)((float)((resistance-R0)/R0)*100.0 * sensi) + offset;
  
  Serial.println(resistance);
  
  /*Du module bluetooth vers la carte arduino*/
  //Tant que la communication entre la carte et le module bluetooth est ouverte 
  while (mySerial.available()){
    //On vient afficher sur le port série (carte vers ordinateur) ce qu'on lit sur le module bluetooth
    i = 0;
    do{
      textBluetooth[i++] = mySerial.read();
      delay(3);
    }while(mySerial.available());
    //Serial.println(textBluetooth);
    String test = (String)textBluetooth;
    test.trim();
    if(test == "mesure"){
      mySerial.print(resistance);
    }
  }
  
  btnLastState = btnState;
  btnState = digitalRead(Switch);
  if(btnState == LOW and btnLastState != LOW){
    if(btnTimer - btnT0 > 500){
      /*Chose a faire quand le bouton est enfoncé*/
      btnT0 = millis();
    }
  }
   
  display.clearDisplay();
  switch (mode){
    case 1:
      MenuMesure();
      if(btnState == LOW and btnLastState != LOW){mode = 5;}
      break;
    case 2:
      MenuEtalonnage();
      if(btnState == LOW and btnLastState != LOW){mode = 5;}
      break;
    case 3:
      MenuEtalonnage2Points();
      if(btnState == LOW and btnLastState != LOW){
        etalonnage2Points += 1;
        if(etalonnage2Points == 3){
          mode = 5;
          etalonnage2Points = 0;
          sensi = ((((mesurePoint2 - mesurePoint1)/R0)*100.0)/(courburePoint2 - courburePoint1));
          offset = (mesurePoint1 - (sensi * courburePoint1/R0));
          selection = 1;
        }
      }
      break;
    case 5:
      MenuChoix();
      if(btnState == LOW and btnLastState != LOW){mode = selection; etalonnage2Points = 0; courbure = 0;}
      break;
    default:
      MenuMesure();
      if(btnState == LOW and btnLastState != LOW){mode = 5;}
      break;
  }

  varSelection = 0;
  
}

/*Fonction qui donne la valeur du rotoEncodeur*/
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

void Affichage(String text, int x, int y, int taille, boolean actualiser){
  display.setTextSize(taille);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(actualiser){
    display.display();
  }
}

void MenuMesure(){
  if(Timer - T0 > 300){
    Affichage("Tension :", 4, 3, 1, false);
    String text = String(valeur * 1000.0);
    String text1 = String(resistance);
    String text2 = String(mesure);
    Affichage(text, 60, 3, 1, false);
    Affichage("mV", 100, 3, 1, false);
    Affichage("Res:", 4, 11, 1, false);
    Affichage(text1, 40, 11, 1, false);
    Affichage("MOhm", 100, 11, 1, false);
    Affichage(text2, 60, 21,1,false);
    Affichage("Courbure:", 4, 21, 1, false);
    Affichage("mm", 100, 21, 1, false);
    display.drawRect(1, 1, 126,31, WHITE);
    display.display();
    T0 = millis();
  }
  Timer = millis();
}

void MenuChoix(){
  Affichage("Menu Mesure", 4, 3, 1, false);
  Affichage("Menu Calibration", 4, 11, 1, false);
  Affichage("Etalon 2 Points", 4, 21, 1, false);
  
  switch (selection){
    case 1:
      Affichage("<", 110, 3, 1, false);
      break;
    case 2:
      Affichage("<", 110, 11, 1, false);
      break;
    case 3:
      Affichage("<", 110, 21, 1, false);
      break;
    default:
      Affichage("<", 110, 3, 1, false);
      break;
  }
  display.drawRect(1, 1, 126,31, WHITE);
  display.display();
}


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
  Affichage("MOhm", 100, 21, 1, false);
  display.drawRect(1, 1, 126,31, WHITE);
  display.display();
}

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
    Affichage("mV", 110, 21, 1, false);
  }else{
    String text1 = String(courbure);
    Affichage("Courbure :", 4, 3, 1, false);
    Affichage(text1, 72, 3, 1, false);
    Affichage("Appuyez", 4, 11, 1, false);
    Affichage("Mesure:", 4, 21, 1, false);
    Affichage(text, 72, 21, 1, false);
    Affichage("mV", 110, 21, 1, false);  
  }
  if(etalonnage2Points == 1){
    mesurePoint1 = resistance;
    courburePoint1 = courbure;
  }
  if(etalonnage2Points == 2){
    mesurePoint2 = resistance;
    courburePoint2 = courbure;
  }
  
  
  
  display.drawRect(1, 1, 126,31, WHITE);
  display.display();
}

void Introduction(){
  display.clearDisplay();
  Affichage("  LES ROUTEURS DE", 4, 7, 1, false);
  Affichage("     L'EXTREME", 4, 15, 1, false);
  display.fillRect(4, 23, display.width()-8, 8, SSD1306_INVERSE);
  display.drawRect(1, 1, 126,31, WHITE);
  display.setTextColor(BLACK);
  display.setCursor(4,23);
  display.setTextSize(0.5);
  display.println("            Bouf INC");
  display.display();
}
