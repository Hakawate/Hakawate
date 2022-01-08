#define BLYNK_TEMPLATE_ID           "TMPLB9CdtyC-"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "0lUiFk8Jx3LjCOjGfP_ku1qgi-jgpKVT"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#include <Keypad.h>
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns
byte pin_rows[ROW_NUM]      = {32, 33, 25, 26}; // GIOP19, GIOP18, GIOP5, GIOP17 connect to the row pins
byte pin_column[COLUMN_NUM] = {27, 14, 12};   // GIOP16, GIOP4, GIOP0, GIOP2 connect to the column pins
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
//#define BLYNK_DEBUG
//#define APP_DEBUG
//#define USE_WROVER_BOARD
#include <PZEM004Tv30.h>
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif
#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif
#if defined(ESP32)
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#elif defined(ESP8266)
//PZEM004Tv30 pzem(Serial1);
#else
PZEM004Tv30 pzem(PZEM_SERIAL);
#endif
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
    float voltage;
    float current;
    float power;
    float energy;
    float frequency ;
    float pf; 
// define the GPIO connected with Relays and switches
#define RelayPin1 5  //D23
#define RelayPin2 18  //D22
#define RelayPin3 19  //D21
#define RelayPin4 23  //D19
#define wifiLed    2   //D2
char key;
#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_BUTTON_4    V4
#define VPIN_BUTTON_5    V5
#define VPIN_BUTTON_6    V6 
#define VPIN_BUTTON_7    V7
#define VPIN_BUTTON_8    V8
#define VPIN_BUTTON_C    V9
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; //Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW; //Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW; //Define integer to remember the toggle state for relay 4
// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;
double bill=0;
#include "BlynkEdgent.h"
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  if(toggleState_1 == 1){
    digitalWrite(RelayPin1, LOW);
  }
  else { 
    digitalWrite(RelayPin1, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  if(toggleState_2 == 1){
    digitalWrite(RelayPin2, LOW);
  }
  else { 
    digitalWrite(RelayPin2, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleState_3 = param.asInt();
  if(toggleState_3 == 1){
    digitalWrite(RelayPin3, LOW);
  }
  else { 
    digitalWrite(RelayPin3, HIGH);
  }
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleState_4 = param.asInt();
  if(toggleState_4 == 1){
    digitalWrite(RelayPin4, LOW);
  }
  else { 
    digitalWrite(RelayPin4, HIGH);
  }
}

//BlynkTimer timer;
void setup()
{
  Serial.begin(115200);
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(wifiLed, OUTPUT);
   pinMode(13, OUTPUT);
  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, LOW);
  digitalWrite(RelayPin2, LOW);
  digitalWrite(RelayPin3, LOW);
  digitalWrite(RelayPin4, LOW);
  BlynkEdgent.begin();
//timer.setInterval(50L, myTimerEvent);

          lcd.init(); 
          lcd.backlight();
  
}

void loop() {
     
      BlynkEdgent.run(); 
      manual_control();
      sensor_data();
        sensorData1 = voltage;
  Blynk.virtualWrite(V5, sensorData1);
  sensorData2 = current;
  Blynk.virtualWrite(V6, sensorData2);
  sensorData3 = power;
  Blynk.virtualWrite(V7, sensorData3);
  sensorData4 = energy;
  Blynk.virtualWrite(V8, sensorData4);
  double tv_trash=3.5;
  double p1_160=0;
  double p161_300=0;
  double p301_500=0;
  if(energy<=160){
     p1_160=0;
   p1_160=energy*(33/1000);}
   else if(energy>160&&energy<=300){
    p1_160=0;
    p161_300=0;
    p1_160=160*(33/1000);
    p161_300=(energy-160)*(72/1000);
    }
     else if(energy>300&&energy<=500){
    p301_500=0;
    p1_160=0;
    p161_300=0;
    p1_160=160*(33/1000);
    p161_300=140*(72/1000);
    p301_500=(energy-300)*(86/1000);
    }
  bill=tv_trash+ p1_160+p161_300+p301_500;
  Blynk.virtualWrite(V0, bill);
  /*if(current>70){
    lcd.clear();
     lcd.setCursor(0,0);
  lcd.print("OVER LIMITED CURRENT");
  lcd.setCursor(0,1);
   lcd.print("CHECK YOUR DEVICE");
   toggleState_1 = 0;
   toggleState_2 = 0;
   toggleState_3 = 0;
   toggleState_4 = 0;
     digitalWrite(RelayPin1, toggleState_1);
      digitalWrite(RelayPin2, toggleState_2);
      digitalWrite(RelayPin3, toggleState_3);
      digitalWrite(RelayPin4, toggleState_4);
       Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
        Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
         Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
          Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    for(int i=0;i=1;){
      key = keypad.getKey();
      if (key == '0') {
    break;
  }}}*/
  
//      myTimerEvent();
      //timers();
}
//void timers(){
//timer.run();}
