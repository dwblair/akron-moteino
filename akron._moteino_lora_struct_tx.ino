

#include <SPI.h> // (built into Arduino IDE, nothing to install)
#include <RH_RF95.h> // find latest 'zip' file at top of page here: http://www.airspayce.com/mikem/arduino/RadioHead/ 
#include "LowPower.h" // https://github.com/rocketscream/Low-Power

  
#define NODEID 2
#define GATEWAYID 1

// Change to 434.0 or other frequency, must match RX's freq!
//#define RF95_FREQ 915.0
#define RF95_FREQ 868.0
 
// Singleton instance of the radio driver
//RH_RF95 rf95(RFM95_CS, RFM95_INT);
RH_RF95 rf95;

// Blinky on receipt
#define LED 9

// Voltage PIN
#define VBATPIN A7

// the following struct must match that on the receiving side 
typedef struct {
  int           nodeId; //store this nodeId
  int           gatewayId; 
  unsigned long packetCount; 
  float         temp;
  float         humid;
  float         lux;
  float         batt;
} Payload;
Payload theData;


unsigned long packetCount=0;


void setup() 
{

  
  pinMode(LED, OUTPUT);

 
//  while (!Serial);
  Serial.begin(9600);
  delay(100);
  
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  Serial.println();
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}
 
int16_t packetnum = 0;  // packet counter, we increment per xmission
 
void loop()
{ 
  digitalWrite(LED, HIGH);
  Serial.println("Sending to rf95_server");


// battery measurement example -- assuming a voltage divider on pin VBATPIN

  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  
  measuredvbat *= 1000; // convert to voltage in thousandths


// this is where we would add sensor code, to generate the below values
   float therm = 3.;
 float temp = analogRead(A0); // example, just to generate new values for testing
 float humid = 1.;
 float lux = analogRead(A1);

 
  //fill in the struct with new values
    theData.nodeId = NODEID;
    theData.gatewayId = GATEWAYID;
    theData.packetCount = packetnum;
    theData.temp = temp;
    theData.humid = humid;
    theData.batt = measuredvbat;
    theData.lux = lux;


  Serial.print("packetCount="); Serial.println(packetnum);
  Serial.print("temp="); Serial.println(temp);


  rf95.send((const void*)(&theData), sizeof(theData));
 
  //Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();

  Serial.println("Sent."); 
  Serial.println();
  delay(10);


    digitalWrite(LED, LOW);

packetnum++;

  delay(200);
  //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  

  
/*
 // other sleep options:
 SLEEP_15MS,
 SLEEP_30MS, 
  SLEEP_60MS,
  SLEEP_120MS,
  SLEEP_250MS,
  SLEEP_500MS,
  SLEEP_1S,
  SLEEP_2S,
  SLEEP_4S,
  SLEEP_8S,
  SLEEP_FOREVER
*/

  
}
