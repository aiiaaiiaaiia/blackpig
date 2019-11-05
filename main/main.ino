//Boards Manager URLs: 
//http://ecourse.cpe.ku.ac.th/practicum/board/package_cpeku_index.json

#include <Servo.h>
#include <Keypad.h>
#include <Password.h>

#define pin_button PIN_PC1
#define pin_ledbuildin PIN_PD3
#define pin_ledgreen PIN_PC2
#define pin_ledred PIN_PC3
#define pin_servo PIN_PC5
#define pin_switchled PIN_PC4
#define pin_yellow PIN_PD5

#define pin_output_to_esp PIN_PD1
#define pin_input_from_esp PIN_PD0

Servo bpservo;
int degree = 90;  
bool state = 0;   //  1 = locked , 0 = unlock  start with locked
String newPasswordString;
char newPassword[6];     

Password password = Password( "1234" );

const byte ROWS = 4; 
const byte COLS = 3; 
byte maxPasswordLength = 4; 
byte currentPasswordLength = 0;

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = { PIN_PB0, PIN_PB1, PIN_PB2, PIN_PB3 };
byte colPins[COLS] = { PIN_PB4, PIN_PB5, PIN_PC0 }; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

  bool stateLED = false;
  int Buttonstate = LOW;
  int previous = LOW;
  
void setup() {
  pinMode(pin_button, INPUT_PULLUP);     // switch_button
  pinMode(pin_ledbuildin,OUTPUT);    // board buildin
  pinMode(pin_yellow,OUTPUT);       // led yellow switch
  pinMode(pin_ledgreen,OUTPUT);    // led green keypad
  pinMode(pin_ledred,OUTPUT);    // led red keypad
  pinMode(pin_switchled,OUTPUT);    // switch led 
  digitalWrite(pin_ledbuildin, LOW);
  digitalWrite(pin_yellow, LOW);
  bpservo.attach(pin_servo);    // servo
  bpservo.write(75);
  pinMode(pin_input_from_esp, INPUT);    // recieve pin 2 from blynk
  pinMode(pin_output_to_esp, OUTPUT);   // send to pin 12 in blynk
  digitalWrite(pin_output_to_esp, HIGH);
}

void loop()
{
  Buttonstate = digitalRead(pin_button);
  if ((Buttonstate != previous) && previous == HIGH ){
    stateLED = !stateLED; 
    delay(500);
    digitalWrite(pin_output_to_esp, LOW);
    delay(50);
    digitalWrite(pin_output_to_esp, HIGH);  
    knoblocker();
      
  }
  previous = Buttonstate;
  
  char key = kpd.getKey();
  if(key) 
  {
    switch (key)
    {
        case '#':  if(state)
        { 
          digitalWrite( pin_ledgreen, HIGH); 
          delay(2000);     
          digitalWrite( pin_ledgreen, LOW); 
          delay(3000);
          knoblocker();
          sendnoti();
          stateLED = !stateLED;
        } else {
          checkPassword();        //enter
        }
            break;
        case '*':  resetPassword();         //reset
            digitalWrite(pin_ledbuildin, HIGH);
            delay(500);
            digitalWrite(pin_ledbuildin, LOW);  
            break;  
        default:   processNumberKey(key);    //append key       
            digitalWrite(pin_yellow, HIGH);
            delay(500);
            digitalWrite(pin_yellow, LOW);
     }
  }

  if(digitalRead(pin_input_from_esp) == LOW)
  {
    knoblocker();
    delay(1500);
  }
}

void knoblocker()
{ state = !(state);
  if(state)  { 
    degree = 0;
    digitalWrite(pin_switchled,HIGH);
  } // unlocked  whem state == 1
  
  else       { 
    degree = 75;
    digitalWrite(pin_switchled,LOW);
  } //locked
  
  bpservo.write(degree);
}

void sendnoti()
{
  digitalWrite(pin_output_to_esp, LOW);
  delay(50);
  digitalWrite(pin_output_to_esp, HIGH);
}

void processNumberKey(char key) {
   currentPasswordLength++;
   password.append(key);
//   if (currentPasswordLength == maxPasswordLength) {
//      checkPassword();
//   }    
}

void checkPassword() {
   if (password.evaluate()){
      knoblocker();
      digitalWrite( pin_ledgreen, HIGH); 
      delay(2000);     
      digitalWrite( pin_ledgreen, LOW);    // PC2 greed led on 
      delay(4000);
      knoblocker();   
      sendnoti();
   } else {
      digitalWrite( pin_ledred, HIGH);     
      delay(1500);        
      digitalWrite( pin_ledred, LOW);     // PC3 red led on 
      sendnoti();
   } 

   resetPassword();
}

void resetPassword() {
   password.reset(); 
   currentPasswordLength = 0; 
}
