# Pill Reminder Esp8266 Project
Basic Esp8266 project for my embedded systems lecture. <br/>

# Project Description: <br/>
  In this project, a system has been developed to regularly remind users of their pill intake times. The system tracks pill schedules using a real-time clock and, when the predetermined time is reached, it alerts the user through a buzzer and activates a servo motor to dispense a single pill from the tube to the patient. Once the process is completed, the system resets the timer and starts counting again. <br/> <br/>

# Components That Are Used in This Project: <br/>
-Esp8266<br/>
![Image](https://github.com/user-attachments/assets/655c6882-adad-4390-ba99-2e01efbfc054) <br/><br/>
-DS1302 RTC Module <br/>
![Image](https://github.com/user-attachments/assets/ff0bf392-ad63-4d66-b65e-61ba0631912f) <br/><br/>
-SG90 9G Servo Motor <br/>
![Image](https://github.com/user-attachments/assets/2ce513bc-fcf5-4886-85a8-fb1f39b2cc1d) <br/><br/>
-Buzzer <br/>
![Image](https://github.com/user-attachments/assets/01b82aac-ad9e-4146-bb6f-f77eda9de40c) <br/><br/>
-Jumper cables and breadboard <br/>
-Small tube for keep the pills in it <br/>

# Overview of Project: <br/>
![Image](https://github.com/user-attachments/assets/95d13623-51a3-49ea-b75d-bcead237fec8) <br/><br/>

# Connections Between Components: <br/>
RTC Module:<br/>
DS1302 CLK/SCLK --> D5<br/>
DS1302 DAT/IO --> D4<br/>
DS1302 RST/CE --> D2<br/>
DS1302 VCC --> 3.3v - 5v<br/>
DS1302 GND --> GND<br/><br/>
Buzzer:<br/>
Buzzer (+) --> D8<br/>
Buzzer (-) --> GND <br/><br/>
Servo Motor:<br/>
Servo Brown --> GND<br/>
Servo Red --> 5V<br/>
Servo Yellow --> D3<br/><br/>

# Libraries That Used in This Project:<br/>
```
#include <Servo.h> 
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
``` 
https://github.com/Makuna/Rtc <br/>
<br/>
# Wifi Settings:<br/>
Activate Hotspot Wifi then change the Wifi name (ssid) and Wifi password password (password) in the codes.<br/>
```
#define ssid "Internet_Name" //Your mobile hotspot name
#define password "Internet_Password" //Your mobile hotspot password
```
!!You must use 2.4GHz Band to Hotspot Wifi. Otherwise Esp8266 doesn't connect the Wifi!! <br/><br/>

# Adafruit IO Settings: <br/>
You should create an Adafruit IO account, copy your username and activate key. Then paste them in code.<br/>
```
#define AIO_USERNAME  "Username" //AdafruitIO username
#define AIO_KEY       "AdafruitIOKey" //AdafruitIO Key
```
You should create a feed in Adafruit IO. Paste the feed key in code. <br/>
```
Adafruit_MQTT_Publish remainingTFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/YourFeedName");
```
Then create a dashboard in Adafruit IO and connect the dashboard to your feed. Set the dashboard as you wish. <br/>
![Image](https://github.com/user-attachments/assets/418bfbee-cdaa-4a26-9f5d-3356aa525933) <br/><br/>

# Setting the Countdown Timer: <br/>
You can change the countownd timer in code below.<br/>
```
int minutes = 1; // Set countdown minute
```
<br/>

# Uploading and Running the Project <br/>
After all this settings. You can upload the codes to your ESP and start the project. Firstly, your ESP connects to your hotspot Wi-Fi. Then the system connects to Adafruit IO with your username and key in the codes. After that, the countdown starts, and the system works. While the system works, it especially sends the information to the AdafruitIO dashboard. You can see the live information in your Adafruit IO dashboard. You can change the dashboard as you like.
