#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeIn =  0xE8E8F0F0E1LL;

RF24 radio(7, 8);

// The sizeof this struct should not exceed 32 bytes
struct PacketData {
  unsigned long hours;
  unsigned long minutes;
  unsigned long seconds;
};

PacketData data;

/**************************************************/

int packetCounts[10];
int packetCountIndex = 0;
int packetCountTotal = 0;

#define AVG_SECONDS 10

int avgs[AVG_SECONDS];
int avgIndex = 0;
unsigned long avgTotal = 0;

void setup()
{
  Serial.begin(115200);
  // Set up radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
  
  memset(&data, 0, sizeof(PacketData));  
  memset( packetCounts, 0, sizeof(packetCounts) );
  memset( avgs, 0, sizeof(avgs) );
}

unsigned long packetsRead = 0;
unsigned long lastScreenUpdate = 0;
unsigned long lastAvgUpdate = 0;
unsigned long lastRecvTime = 0;
unsigned long drops = 0;

/**************************************************/

void recvData()
{  
  while ( radio.available() ) {        
    radio.read(&data, sizeof(PacketData));
    packetsRead++;
    lastRecvTime = millis();
  }
}

/**************************************************/

char ppsBuf[16];
char avgBuf[16];
char hmsBuf[16];

void updateScreen()
{  
  unsigned long now = millis();
  if ( now - lastScreenUpdate < 100 )
    return;
    
  // moving average over 1 second
  packetCountTotal -= packetCounts[packetCountIndex];
  packetCounts[packetCountIndex] = packetsRead;
  packetCountTotal += packetsRead;

  packetCountIndex = (packetCountIndex + 1) % 10;
  packetsRead = 0;
  Serial.print("packet count:");
  Serial.print(packetCountTotal);
  Serial.print(" ");
  Serial.print("avg count:");
  Serial.println(avgTotal / AVG_SECONDS);
  //Serial.print(hmsBuf, "%02ld:%02ld:%02ld", data.hours, data.minutes, data.seconds);

  lastScreenUpdate = millis();

  if ( now - lastAvgUpdate >= 1000 ) {    
    // moving average of 1 second moving average
    avgTotal -= avgs[avgIndex];
    avgs[avgIndex] = packetCountTotal;
    avgTotal += packetCountTotal;
  
    avgIndex = (avgIndex + 1) % AVG_SECONDS;
    lastAvgUpdate = millis();
  }
}

/**************************************************/

void loop()
{
  recvData();
  updateScreen();
}







