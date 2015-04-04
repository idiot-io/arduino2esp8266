//based on that temprature logger instructable
/* 
move following SSID, PASS and thingspek key defines to a seperate file, like
#include "creds.h"
#define SSID "TelAvivMakers"
#define PASS ""
#define thingspeak_key "your thingspeak_key"
*/

#include "creds.h"
#define IP "184.106.153.149" // thingspeak.com static ip
String GET_pre = "GET /update?key=";
String GET_pos = "&field1=";

//we are working with the mega so no need for software serial
//SoftwareSerial monitor(10, 11); // RX, TX

void setup()
{
  Serial.begin(115200);
  Serial3.begin(115200); //esp8266
  Serial.println("restart....");
  delay(2000);
  sendDebug("AT");
  delay(5000);
  if(Serial3.find("OK")){
    Serial.println("RECEIVED: OK");
    connectWiFi();
  }
}

void loop(){
  float tempC = random(0.3, 30.0);;
  char buffer[10];
  String tempF = dtostrf(tempC, 3, 1, buffer);
  updateTemp(tempF);
  delay(3000);
}

void updateTemp(String tenmpF){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  sendDebug(cmd);
  delay(2000);
  if(Serial3.find("Error")){
    Serial.print("RECEIVED: Error");
    return;
  }
  cmd = GET_pre;
  cmd += thingspeak_key;
  cmd += GET_pos;
  cmd += tenmpF;
  cmd += "\r\n";
  Serial3.print("AT+CIPSEND=");
  Serial3.println(cmd.length());
  if(Serial3.find(">")){
    Serial.print(">");
    Serial.print(cmd);
    Serial3.print(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(Serial3.find("OK")){
    Serial.println("RECEIVED: OK");
  }else{
    Serial.println("RECEIVED: Error");
  }
}

void sendDebug(String cmd){
  Serial.print("SEND: ");  Serial.println(cmd);
  Serial3.println(cmd);
} 
 
boolean connectWiFi(){
  Serial3.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(Serial3.find("OK")){
    Serial.println("RECEIVED: OK");
    return true;
  }else{
    Serial.println("RECEIVED: Error");
    return false;
  }
}

/*
// VIA http://www.cse.dmu.ac.uk/~sexton/ESP8266/
// Serial pass through for teensy 

void setup()
{
  Serial.begin(115200);
  Serial3.begin(115200);
}

void loop()
{
  // send everything received from the hardware uart to usb serial & vv 
  if (Serial.available() > 0) {
    char ch = Serial.read();
    Serial3.print(ch);
  }
  if (Serial3.available() > 0) {
    char ch = Serial3.read();
    Serial.print(ch);
  }
}
*/