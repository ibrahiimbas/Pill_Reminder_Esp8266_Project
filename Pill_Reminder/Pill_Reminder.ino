// Connections:
// DS1302 CLK/SCLK --> D5
// DS1302 DAT/IO --> D4
// DS1302 RST/CE --> D2
// DS1302 VCC --> 3.3v - 5v
// DS1302 GND --> GND
// Buzzer (+) --> D8
// Servo Brown --> GND
// Servo Red --> 5V
// Servo Yellow --> D3

#include <Servo.h> 
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include<ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define ssid "Internet_Name" //Your mobile hotspot name
#define password "Internet_Password" //Your mobile hotspot password

#define AIO_SERVER      "io.adafruit.com"
// Using port 1883 for MQTTS
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "Username" //AdafruitIO username
#define AIO_KEY       "AdafruitIOKey" //AdafruitIO Key

// WiFiFlientSecure for SSL/TLS support
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Adafruit IO Feed
Adafruit_MQTT_Publish remainingTFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/YourFeedName");

//RTC Pin Connections
ThreeWire RtcWire(02,14,04); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(RtcWire);  

//Buzzer Pin Connections
const int buzzerPin = 15;

RtcDateTime lastTriggerTime; // Keep the last trigger time

Servo s1;  

int minutes = 1; // Set countdown minute

uint8_t remainingSeconds = 0; // Remaining second

void setup () 
{
    // Connect Wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.begin(57600);

   while(WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);

    activateServo();

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();
    Serial.print("Current date and time ");
    Serial.println(printDateTime(Rtc.GetDateTime()));

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

     lastTriggerTime = Rtc.GetDateTime();

    // Buzzer and led pins are set as output
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
    //Attach Servo to D3
    s1.attach(0);  
}

void loop () 
{
   // Check MQTT connection
    if (mqtt.connected() == false) 
    {
        reconnect();
    }
    mqtt.processPackets(1000);

    RtcDateTime now = Rtc.GetDateTime();

    if (!now.IsValid())
    {
        // Common Causes:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }

    // Calculate and send the remaining time to AdafruitIO
    uint32_t elapsedSeconds = now.TotalSeconds() - lastTriggerTime.TotalSeconds();
    uint32_t remainingSeconds = (minutes * 60) - (elapsedSeconds % (minutes * 60)); // Remaining seconds

    if (remainingSeconds >= 0) 
    {
        int countdown =remainingSeconds;

        // Send countown information to AdafruitIO
        if (!remainingTFeed.publish(countdown)) 
        {
            Serial.println("Error! Countdown couldn't be sent");
        } 
        if(countdown<=10)
        {
          Serial.print(countdown);
          Serial.println(" seconds to give pill.");
        }
    }

    if (isTimeElapsed(lastTriggerTime, now, minutes))  // Has ... minutes pass?
    {
        activateServo();
        Serial.println("Pill time has come...");
        Serial.print("Date and time of taking pill: ");
        Serial.println(printDateTime(now));
        activateBuzzer();

        // Update last trigger time
        lastTriggerTime = now;
    }

    delay(1000);
}

String printDateTime(const RtcDateTime& dt)
{
    char datestring[26];
    snprintf_P(datestring, 
            sizeof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second());
    return String(datestring);
}

bool isTimeElapsed(const RtcDateTime& startTime, const RtcDateTime& currentTime, int minutes)
{
    // Calculate elapsed minute
    uint32_t elapsedSeconds = currentTime.TotalSeconds() - startTime.TotalSeconds();
    uint32_t elapsedMinutes = elapsedSeconds / 60;

    return elapsedMinutes >= minutes;
}

void activateBuzzer()
{   
    digitalWrite(buzzerPin, HIGH);
    delay(1000);                 
    digitalWrite(buzzerPin, LOW);
    delay(250); 
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(125);
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
}

// Reconnect MQTT
void reconnect() 
{
    Serial.println("Adafruit IO'ya bağlanılıyor...");
    while (mqtt.connect() != 0) 
    {
        Serial.print(".");
        delay(5000);
    }
    Serial.println("Adafruit IO'ya bağlantı sağlandı!");
}

//Servo
void activateServo()
{
  Serial.println("Servo calisti...");
   s1.write(0);  
   delay(100);  
   s1.write(90);  
   delay(500);  
   s1.write(0); 
}

