#include <SPI.h>  
#include "RF24.h" 
#include <LiquidCrystal.h>

RF24 myRadio (7, 8); 


int led1=A0,led2=A1,led3=A2,led4=A3,led5=A4,buz=A5;
int rs=3,e=2,d4=5,d5=6,d6=9,d7=10;
unsigned long buzTime;
bool buzMode=false;
int prevData=0;

//not work with nano
//change channel,power,speed
//no errors on rf24 lib

long tmp2=0;
long command;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

byte addresses[][6] = {"0","1"}; 

struct package
{
  int level=0;
  char  msg[100] = "empty";
};
typedef struct package Package;
Package data;

void setup() 
{
  Serial.begin(115200);
  myRadio.begin(); 
  myRadio.setChannel(105); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe(addresses[0]);
  myRadio.openReadingPipe(1, addresses[1]);
  myRadio.startListening();
  lcd.begin(16, 2);
  analogWrite(led1,0);
  analogWrite(led2,0);
  analogWrite(led3,0);
  analogWrite(led4,0);
  analogWrite(led5,0);
  analogWrite(buz,0);
  buzMode=false;
  prevData = 0;
}


void loop()  
{
  receive();
  alarm();
  delay(500);

}


void receive(){
  if ( myRadio.available()) 
  {
    myRadio.read( &data, sizeof(data) );
    Serial.print("level: ");
    Serial.print(data.level);
    Serial.print("  ");
    Serial.print("msg: ");
    Serial.print(data.msg);
    Serial.println("::");
    
    if((data.level==5||data.level==4||data.level==3||data.level==2||data.level==1)&&(prevData!=data.level)){
      //Serial.println(data.msg);
      prevData=data.level;
      myRadio.stopListening();
      dataS(data);
      delay(100);
      dataS(data);
      delay(100);
      dataS(data);
      delay(100);
      myRadio.startListening();
      setLED(data.level);
      
    }
    switch(data.level){
    case 1:lcd.setCursor(0, 0);lcd.print("WARNING");break;
    case 2:lcd.setCursor(0, 0);lcd.print("BE AWARE");break;
    case 3:lcd.setCursor(0, 0);lcd.print("CAUTION");break;
    case 4:lcd.setCursor(0, 0);lcd.print("DANGER");break;
    case 5:lcd.setCursor(0, 0);lcd.print("LIFE THREAT");break;
    
    }

  }
  }
  


void dataS(struct package cmd){
    myRadio.write(&cmd, sizeof(cmd)); 
    Serial.println("data packet sent");
  }

void setLED(long d){
//  lcd.begin(16, 2);
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
    if(data.level==1){
      if(millis()-buzTime>2500){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
      
    }else if(data.level==2){
      if(millis()-buzTime>2000){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
    }
    else if(data.level==3){
      if(millis()-buzTime>1500){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
    }
    else if(data.level==4){
      if(millis()-buzTime>1000){
        buzTime=millis();
        analogWrite(buz,255);
        delay(500);
        analogWrite(buz,0);
        }
      
    }
    else if(data.level==5){
      analogWrite(buz,255);
      delay(100);
      analogWrite(buz,0);
    }
    
    
    }
  
  }


