#include <Wire.h>
#include "EmonLib.h"
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "ecor-786d5.firebaseio.com"
#define FIREBASE_AUTH "bq2thPayoKfCchANvM0zyTkIL2k74t0zntHYbJk"
#define WIFI_SSID "Harjuna"
#define WIFI_PASSWORD "belalekkk"
 
EnergyMonitor emon1; 
int tegangan = 220.0;
String str;
int pin_sct = A0;
 
void setup()
{
Serial.begin(9600);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
emon1.current(pin_sct, 60);
}
 
void loop()
{ 
  double Irms = emon1.calcIrms(1480);
  //menampilkan di serial monitor
  Serial.print("Arus : ");
  Serial.print(Irms);
  Serial.print(" Daya : ");
  Serial.println(Irms*tegangan);
  float watt = Irms*tegangan;
  if (Firebase.getFloat("arus")== 0){
  Firebase.setFloat("arus",Irms);
  Firebase.setFloat("daya",watt);
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  }
  if(Firebase.getFloat("arus") != 0){
     Firebase.set("arus",Irms);
     Firebase.set("daya",watt);
     if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  }
  // handle error
  Firebase.getFloat("Cost");
  float biaya = watt/1000 * 1.13;
  if (Firebase.getFloat("Cost")) == 0{
  Firebase.setFloat("Cost",biaya);
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  }

  if (Firebase.getFloat("Cost")) != 0 {
    float biaya2 = Firebase.getFloat("Cost") + biaya;
    Firebase.set("Cost",biaya2);
    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  }
  delay(3000);
}
