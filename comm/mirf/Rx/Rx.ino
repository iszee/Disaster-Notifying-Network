#include "SPI.h"
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#include <LiquidCrystal.h> 


int led1=A0,led2=A1,led3=A2,led4=A3,led5=A4,buz=A5;
int rs=3,e=2,d4=5,d5=6,d6=9,d7=10;
unsigned long buzTime;
bool buzMode=false;
int prevData=0;

long data;
long tmp2=0;
long command;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);


void setup() {
   Mirf.spi = &MirfHardwareSpi;
   Mirf.init();
   Mirf.setRADDR((byte *)"serv1");
   Mirf.payload = sizeof(long);
   Mirf.config();
   lcd.begin(16, 2);
   Serial.begin(115200);
   analogWrite(led1,0);
   analogWrite(led2,0);
   analogWrite(led3,0);
   analogWrite(led4,0);
   analogWrite(led5,0);
   analogWrite(buz,0);
   buzMode=false;
   prevData = 0;


}

void loop() {
  receive();
  alarm();
  delay(500);
  
}

void receive(){
  if(!Mirf.isSending() && Mirf.dataReady()){
    Mirf.getData((byte *)&data);
    Mirf.rxFifoEmpty();
    Serial.print(data);
    Serial.print(" :: ");
    
    
    if((data==5||data==4||data==3||data==2||data==1)&&(prevData!=data)){
      Serial.println(data);
      prevData=data;
      dataS(data);
      delay(100);
      dataS(data);
      delay(100);
      dataS(data);
      delay(100);
      setLED(data);
      
    }
    switch(data){
    case 1:lcd.setCursor(0, 0);lcd.print("WARNING");break;
    case 2:lcd.setCursor(0, 0);lcd.print("BE AWARE");break;
    case 3:lcd.setCursor(0, 0);lcd.print("CAUTION");break;
    case 4:lcd.setCursor(0, 0);lcd.print("DANGER");break;
    case 5:lcd.setCursor(0, 0);lcd.print("LIFE THREAT");break;
    
    }

  }
  }
  


void dataS(long cmd){
    command=cmd;
    delay(500);
    Mirf.send((byte *)&command);
    while(Mirf.isSending()){
    }
    Serial.println("data packet sent new tx");
  }

void setLED(long d){
  lcd.begin(16, 2);
  buzMode = true;
  buzTime = millis();
  switch(d){
    case 1:analogWrite(led1,255);break;
    case 2:analogWrite(led1,255);analogWrite(led2,255);break;
    case 3:analogWrite(led1,255);analogWrite(led2,255);analogWrite(led3,255);break;
    case 4:analogWrite(led1,255);analogWrite(led2,255);analogWrite(led3,255);analogWrite(led4,255);break;
    case 5:analogWrite(led1,255);analogWrite(led2,255);analogWrite(led3,255);analogWrite(led4,255);analogWrite(led5,255);break;
    
    }
  
  
}

void alarm(){
  if(buzMode){
    if(data==1){
      if(millis()-buzTime>2500){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
      
    }else if(data==2){
      if(millis()-buzTime>2000){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
    }
    else if(data==3){
      if(millis()-buzTime>1500){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
    }
    else if(data==4){
      if(millis()-buzTime>1000){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
      
    }
    else if(data==5){
      analogWrite(buz,255);
      delay(100);
      analogWrite(buz,0);
    }
    
    
    }
  
  }

