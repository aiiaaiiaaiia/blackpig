//Boards Manager URLs
//http://arduino.esp8266.com/stable/package_esp8266com_index.json
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example runs directly on ESP8266 chip.
  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino
  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!
  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int button = 0; //button in app
String newPassword;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "49c680f5a8424d37aa686e849dd68524";    // for linked to my Blynk app

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Aiiphone";       // I connected wifi with my personal hotspot
char pass[] = "****************";   // This part is my password , i'd like not to publish

int lastConnectionAttempt = millis();
int connectionDelay = 5000; // try to reconnect every 5 seconds
bool state_button = 0; 

WidgetLED led1(V1);
WidgetLED led2(V2);
BlynkTimer timer;
bool state = 0;

void notifyOnButtonPress()// function when you press button and it will send notic to app 
{
  // Invert state, since button is "Active LOW"
  int state_button = !state_button; 
  if (state_button) { //สั่งกดปุ่มแล้ว จะส่งแจ้งเตือนไปที่แอป เปลี่ยนข้อความกะได้นะ อิอิ
    { state = !state;
    if(state){
    Blynk.notify("Button is pressed! <UNLOCK> ");
    led1.on();
    }
    else {
    Blynk.notify("Button is pressed! <LOCK> ");
    led1.off();
    }
  }
 } 
}


// set value button widget
BLYNK_WRITE(V0) {  // This function gets called every state change of the Button Widget
  button = param.asInt();  // This assigns the Button state to a variable
 }  


// Attach virtual serial terminal to Virtual Pin V1
//WidgetTerminal terminal(V1);

// set terminal to change password
// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
//BLYNK_WRITE(V1)
//{
//
//  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
//  if (String("Change password") == param.asStr()) {
//    terminal.println("New password: ") ;
//    newPassword = param.asStr(); //paramiter for new password
//  } else {
//
//    // Send it back
//    terminal.write(param.getBuffer(), param.getLength());
//    terminal.print(" is unknown command");
//    terminal.println();
//  }
//
//  // Ensure everything is sent
//  terminal.flush();
//}


void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(2, OUTPUT);   
  pinMode(12, INPUT);
  digitalWrite(2, HIGH);
  Blynk.begin(auth, ssid, pass);
//  attachInterrupt(digitalPinToInterrupt(pinButton), notifyOnButtonPress, CHANGE);
   // Clear the terminal content
//  terminal.clear();
//  terminal.println(F("connected wifi"));  // This will print when your hardware gets connected to Blynk Server
//  terminal.flush();
}
bool buttonstate = 0;
void loop()
{
  // check WiFi connection:
  if (WiFi.status() != WL_CONNECTED)
  {
    // (optional) "offline" part of code

    // check delay:
    if (millis() - lastConnectionAttempt >= connectionDelay)
    {
      lastConnectionAttempt = millis();

      // attempt to connect to Wifi network:
      if (pass && strlen(pass))
      {
        WiFi.begin((char*)ssid, (char*)pass);
      }
      else
      {
        WiFi.begin((char*)ssid);
      }
    }
  }
  else
  {
    Blynk.run();
    if(digitalRead(12) == LOW)
    {
    notifyOnButtonPress();
    }   // มาจาก การกดswitch และจะทำการ noti ไปที่มือถือ
  
    else if ( (button == 1) && (buttonstate == 0)){
     buttonstate =! buttonstate;  
    digitalWrite(2, LOW);  // ส่งไปให้ servo ทำงานที่ตัวบอร์ด
   // Blynk.notify("Yaaay... Door is unlocked!");
    led2.off();
    digitalWrite(2, HIGH);
    }  
    else if ( (button == 0 ) && (buttonstate == 1) )
    {
      buttonstate =! buttonstate;
      digitalWrite(2, LOW);  // ส่งไปให้ servo ทำงานที่ตัวบอร์ด
      led2.on();
      digitalWrite(2, HIGH);
    } 
  //   Blynk.notify("Yaaay... Door is locked!");
  }
}
