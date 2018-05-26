#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
byte addresses[][6] = {"0","1"};

long command;
String st_command;
long last_command;
long s=0;
String st_s="";


struct package
{
  int level=0;
  char  msg[100] = "Text to be transmitted";
};
typedef struct package Package;
Package data;


void setup()
{
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(100); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe(addresses[1]);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.stopListening();
  
  delay(1000);
}

void loop()
{ 
//  s=Serial.parseInt();
//  if(s!=0){
//  dataS(s);
//  }

//  st_s=Serial.readString();
//  if(s!=""){
//  dataString(st_s);
//  }
  
  data.level=2;
  strcpy(data.msg,"run for ur life");
  dataPack(data);
  delay(500);
  

}

void dataS(long cmd){
    command=cmd;
    //delay(500);
    myRadio.write(&command, sizeof(command)); 
    Serial.println("data packet sent");
  }

void dataString(String cmd){
    st_command=cmd;
    //delay(500);
    myRadio.write(&st_command, sizeof(st_command)); 
    Serial.println("data packet sent");
  }

void dataPack(struct package cmd){
    myRadio.write(&cmd, sizeof(cmd)); 
    Serial.println("data packet sent");
  }

void stormSend(){
  long storm_cmd;
  for(int i=1;i<=5;i++){
    storm_cmd =command;
    delay(500);
    myRadio.write(&storm_cmd, sizeof(storm_cmd)); 
    
  }
  Serial.println("storm data packet sent");
  delay(1000);

  
  }
