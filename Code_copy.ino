#include <Arduino.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define API_KEY "AIzaSyAnjtzqXfZSKERATWqAXbaytOfM507iXJU"
#define DATABASE_URL "https://esp32firebase-6052e-default-rtdb.europe-west1.firebasedatabase.app/"
#define TRIGGER_PIN 4
#define LED_PIN 2
HardwareSerial MySerial(2);

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
int isPair;

int pin34,pin35,pin36,pin39,pin25,pin23,pin22,pin21,pin19,pin18,pin17,pin16;
String type34,type35,type36,type39,type25,type23,type22,type21,type19,type18,type17,type16,sertype,ser;

String device_name=WiFi.macAddress();

WiFiManager wifiManager;

void setup(){
  ser="B";
  Serial.begin(115200);
  //Serial2.begin(9600);
  pinMode(LED_PIN, OUTPUT );                                                                                                                               
  digitalWrite(LED_PIN, HIGH);
  wifiManager.autoConnect("ESP32Tunnel ","");

  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */                                                                                                    
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
  digitalWrite(LED_PIN, LOW); 
    
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  String dt=device_name+"/Debug/Status";
  if(Firebase.RTDB.setString(&fbdo, dt, "On"))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    dt=device_name+"/Debug/Restart";
    if(Firebase.RTDB.setString(&fbdo, dt, "N")){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
  init();
  pinMode(34, INPUT_PULLDOWN);
  pinMode(35, INPUT_PULLDOWN);
  pinMode(36, INPUT_PULLDOWN);
  pinMode(39, INPUT_PULLDOWN);
  //pinMode(32, INPUT_PULLDOWN);
  //pinMode(33, INPUT_PULLDOWN);
  pinMode(25, INPUT_PULLDOWN);
  pinMode(23, INPUT_PULLDOWN);
  pinMode(22, INPUT_PULLDOWN);
  pinMode(4, INPUT_PULLUP);
  pinMode(21, INPUT_PULLDOWN);
  pinMode(19, INPUT_PULLDOWN);
  pinMode(18, INPUT_PULLDOWN);
  pinMode(17, INPUT_PULLDOWN);
  pinMode(16, INPUT_PULLDOWN);
  
  pin34=0;
  pin36=0;
  pin39=0;
  pin35=0;
  //pin32=0;
  //pin33=0;
  pin25=0;
  pin23=0;
  pin22=0;
  pin21=0;
  pin19=0;
  pin18=0;
  pin17=0;
  pin16=0;
  UploadConfig();
}


void loop(){ // D digital A analog N nothing

  //if(type32=="D")pin32=uploaddigitaldata(32,pin32); //UART RX
  //if(type32=="A")pin32=uploadanalogdata(32,pin32); //UART RX
  //if(type33=="D")pin33=uploaddigitaldata(33,pin33); //UART TX
  //if(type33=="A")pin33=uploadanalogdata(33,pin33); //UART TX
 Restart();
 UploadConfig();
 //if(Serial2.available()>0)
 //{if((sertype=="S"&&Serial2.readString()!=ser)&&(Firebase.RTDB.setString(&fbdo, "ESPDEMO/Pins/Serial", Serial2.readString())))ser=Serial2.readString();}
 String dt=device_name+"/Pins/D36";
 if ((type36=="A"&&pin36!=analogRead(36))&&(Firebase.RTDB.setInt(&fbdo, dt, analogRead(36))))pin36=analogRead(36); //A1
 dt=device_name+"/Pins/D39"; 
 if ((type39=="A"&&pin39!=analogRead(39))&&(Firebase.RTDB.setInt(&fbdo, dt, analogRead(39))))pin39=analogRead(39); //A2
 dt=device_name+"/Pins/D34";
 if ((type34=="A"&&pin34!=analogRead(34))&&(Firebase.RTDB.setInt(&fbdo, dt, analogRead(34))))pin34=analogRead(34); //A3
 dt=device_name+"/Pins/D35";
 if ((type35=="A"&&pin35!=analogRead(35))&&(Firebase.RTDB.setInt(&fbdo, dt, analogRead(35))))pin35=analogRead(35); //A4
 dt=device_name+"/Pins/D16";
 if ((type16=="D"&&pin16!=digitalRead(16))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(16))))pin16=digitalRead(16); //Din1
 dt=device_name+"/Pins/D17";
 if ((type17=="D"&&pin17!=digitalRead(17))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(17))))pin17=digitalRead(17); //Din2
 dt=device_name+"/Pins/D18";
 if ((type18=="D"&&pin18!=digitalRead(18))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(18))))pin18=digitalRead(18); //Din3
 dt=device_name+"/Pins/D19";
 if ((type19=="D"&&pin19!=digitalRead(19))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(19))))pin19=digitalRead(19); //Din4
 dt=device_name+"/Pins/D21";
 if ((type21=="D"&&pin21!=digitalRead(21))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(21))))pin21=digitalRead(21); //Dout1
 dt=device_name+"/Pins/D22";
 if ((type22=="D"&&pin22!=digitalRead(22))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(22))))pin22=digitalRead(22); //Dout2
 dt=device_name+"/Pins/D23";
 if ((type23=="D"&&pin23!=digitalRead(23))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(23))))pin23=digitalRead(23); //Dout3
 dt=device_name+"/Pins/D25";
 if ((type25=="D"&&pin25!=digitalRead(25))&&(Firebase.RTDB.setInt(&fbdo, dt, digitalRead(25))))pin25=digitalRead(25); //Dout4

 if ( digitalRead(TRIGGER_PIN) == LOW){wifiManager.resetSettings();ESP.restart();}
 }

String readconfig(int pinnumber){
  String gpiopath = device_name+"/Config/GPIO"+pinnumber;
  gpiopath.concat(pinnumber);
  String output=Firebase.RTDB.getString(&fbdo, gpiopath) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
  Serial.println(output);
  return output;
}
void Restart(){
  String dt=device_name+"/Debug/Restart";
  String rest = Firebase.RTDB.getString(&fbdo, dt) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
  dt=device_name+"/Debug/Status";
  String dt1 = device_name+"/Config/UPLOAD";
  if((rest=="Y")&&(Firebase.RTDB.setString(&fbdo,dt, "Off")&&Firebase.RTDB.setString(&fbdo, dt1, "Y")))ESP.restart();
  }
void UploadConfig(){
  String dt=device_name+"/Config/UPLOAD";
  String up = Firebase.RTDB.getString(&fbdo, dt) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
  dt=device_name+"/Config/UPLOAD";
  if(up=="Y")
  if(Firebase.RTDB.setString(&fbdo, dt, "N")){
      type34=readconfig(34);
      type36=readconfig(36);
      type39=readconfig(39);
      type35=readconfig(35);
      type25=readconfig(25);
      type23=readconfig(23);
      type22=readconfig(22);
      type21=readconfig(21);
      type19=readconfig(19);
      type18=readconfig(18);
      type17=readconfig(17);
      type16=readconfig(16);
      sertype=readconfig(32);
  }
}

void init(){
  String dt=device_name+"/Config/GPIO36";
  int Ok=0;
  if (Firebase.RTDB.setString(&fbdo, dt, "A")) Ok=1;
  dt=device_name+"/Config/GPIO39";
  if (Firebase.RTDB.setString(&fbdo, dt, "A")) Ok=1;
  dt=device_name+"/Config/GPIO34";
  if (Firebase.RTDB.setString(&fbdo, dt, "A")) Ok=1;
  dt=device_name+"/Config/GPIO35";
  if (Firebase.RTDB.setString(&fbdo, dt, "A")) Ok=1;
  dt=device_name+"/Config/GPIO16";
  if (Firebase.RTDB.setString(&fbdo, dt, "D")) Ok=1;
  dt=device_name+"/Config/GPIO17";
  if (Firebase.RTDB.setString(&fbdo, dt, "D")) Ok=1;
  dt=device_name+"/Config/GPIO18";
  if (Firebase.RTDB.setString(&fbdo, dt, "D")) Ok=1;
  dt=device_name+"/Config/GPIO19";
  if (Firebase.RTDB.setString(&fbdo, dt, "D")) Ok=1;
  dt=device_name+"/Config/GPIO21";
  if (Firebase.RTDB.setString(&fbdo, dt, "D")) Ok=1;
  dt=device_name+"/Config/GPIO22";
  if (Firebase.RTDB.setString(&fbdo, dt, "D")) Ok=1;
  dt=device_name+"/Config/GPIO23";
  if (Firebase.RTDB.setString(&fbdo,dt, "D")) Ok=1;
  dt=device_name+"/Config/GPIO25";
  if (Firebase.RTDB.setString(&fbdo, dt, "D")) Ok=1;
  dt=device_name+"/Config/UPLOAD";
  if (Firebase.RTDB.setString(&fbdo, dt, "N")) Ok=1;
}