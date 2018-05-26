#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut =  0xE8E8F0F0E1LL;

// The sizeof this struct should not exceed 32 bytes
struct PacketData {
  unsigned long hours;
  unsigned long minutes;
  unsigned long seconds;
};

PacketData data;

RF24 radio(7, 8);

void setup() {
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);

  data.hours = 0;
  data.minutes = 0;
  data.seconds = 0;    
}

unsigned long lastTick = 0;

void loop() {
  unsigned long now = millis();
  if ( now - lastTick >= 1000 ) {
    data.seconds++;
    if ( data.seconds >= 60 ) {
      data.seconds = 0;
      data.minutes++;
    }
    if ( data.minutes >= 60 ) {
      data.minutes = 0;
      data.hours++;
    }
    lastTick = now;
  }
    
  radio.write(&data, sizeof(PacketData));    
}


