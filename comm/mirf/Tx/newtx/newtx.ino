#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <Wire.h>


int command;
int last_command;


void setup() {
  
  Mirf.spi = &MirfHardwareSpi;  
  Mirf.init(); 
  Mirf.setTADDR((byte *)"serv1");
  Mirf.payload = sizeof(int);
  Mirf.config(); 
  Serial.begin(19200);
   Serial.println("Enter your msg");
}

void loop() {
  int s=8080;
 
  if(Serial.available()){
  s=Serial.parseInt();
  }
  if(s!=8080){
  dataS(s);
  }
  delay(100);



  }

void dataS(int cmd){
    command=cmd;
    //delay(500);
    Mirf.send((byte *)&command);
    while(Mirf.isSending()){
    }
    Serial.print("data packet sent ");
    Serial.println(command);
  }




void stormSend(){
  String storm_cmd;
  for(int i=1;i<=5;i++){
    storm_cmd =command;
    delay(500);
    Mirf.send((byte *)&storm_cmd);
    
    while(Mirf.isSending()){
    }
    
  }
  Serial.println("storm data packet sent");
  delay(1000);

  
  }
