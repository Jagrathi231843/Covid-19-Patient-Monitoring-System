#include <WiFi.h>
#include <secrets.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#include <DFRobot_MAX30102.h>

DFRobot_MAX30102 particleSensor;
int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
float vout;

char ssid[] = "realme 6";   // your network SSID (name) 
char pass[] = "gautham212001";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber =  1581561;
const char * myWriteAPIKey = "4WKAUDUJOMH7VFHK";

int number = 0;

void setup() {
   Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    }
    delay(1000);
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  particleSensor.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4, \
   /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100, \
                        /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);
}
int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid 

void loop() {
  
 sensorValue = analogRead(sensorPin);
  vout = sensorValue / 8;
 
  Serial.println(vout);

  particleSensor.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);
 
 if(heartRate<50)
 heartRate=50;
  Serial.print(heartRate, DEC);
  if(SPO2<50)
 SPO2=50;
 
  Serial.print(F("; SPO2="));
  Serial.print(SPO2, DEC);
  Serial.print(F(", SPO2Valid="));
 
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  ThingSpeak.setField(1,vout);
  ThingSpeak.setField(2,heartRate);
  ThingSpeak.setField(3,SPO2);
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x= ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
   if(x==200){
    Serial.println("Channel update succesful");
   }
   else{
     Serial.println("error");
    }
   delay(10000); // Wait 20 seconds to update the channel again  
  
  }
  
  
