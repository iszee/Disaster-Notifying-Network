#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <Wire.h>


long command;
long last_command;


void setup() {
  
  Mirf.spi = &MirfHardwareSpi;  
  Mirf.init(); 
  Mirf.setTADDR((byte *)"serv1");
  Mirf.payload = sizeof(long);
  Mirf.config(); 
  Serial.begin(115200);
}

void loop() {
  int s=0;
  //Serial.println("Enter Your msg: ");
  //s=Serial.input();
  s=1;
  if(s!=0){
  dataS(s);
  }
  delay(500);



  }

void dataS(long cmd){
    command=cmd;
    //delay(500);
    Mirf.send((byte *)&command);
    while(Mirf.isSending()){
    }
    Serial.println("data packet sent");
  }

void stormSend(){
  long storm_cmd;
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
