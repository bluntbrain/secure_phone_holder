#include <Keypad.h>
#include<EEPROM.h>

char password[4];
char initial_password[4],new_password[4];
int i=0;
//int relay_pin = 10;
char key_pressed=0;
int greenled = 12;
int redled = 11;
int lockopenled = 10;
int yellowled = 13;

const byte ROWS = 4; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad keypad_key = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  pinMode(lockopenled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(yellowled, OUTPUT);
  Serial.print("Secure phone lock V1.0!");
  delay(1000);
  Serial.print("Enter Password");
  initialpassword();
}
  
void loop(){
//  digitalWrite(relay_pin, HIGH);
  key_pressed = keypad_key.getKey();
  if(key_pressed=='#')
    change();
  if (key_pressed)
  {
    
    password[i++]=key_pressed;
    Serial.print(key_pressed);
      }
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
      initial_password[j]=EEPROM.read(j);
    if(!(strncmp(password, initial_password,4)))
    {
      Serial.print("Pass Accepted");
      digitalWrite(lockopenled, HIGH);
      digitalWrite(greenled, HIGH);
      delay(12000);
      digitalWrite(greenled, LOW);
//      digitalWrite(relay_pin, LOW);
      delay(500);
      digitalWrite(lockopenled, LOW);
      Serial.print("Pres # to change");
      delay(1000);
      Serial.print("Enter Password:");
      i=0;
    }
    else{
//      digitalWrite(relay_pin, HIGH);
      Serial.print("Wrong Password");
      digitalWrite(redled, HIGH);
      delay(2000);
      digitalWrite(redled, LOW);
      Serial.print("Pres # to Change");
      delay(1000);
      Serial.print("Enter Password");
      i=0;
    }
  }
}
void change(){
  int j=0;
  Serial.print("Current Password");
      digitalWrite(yellowled, HIGH);
      
  while(j<4)
  {
    char key=keypad_key.getKey();
    if(key)
    {
      new_password[j++]=key;
      Serial.print(key);
    }
    key=0;
  }
  delay(500);
  if((strncmp(new_password, initial_password, 4)))
  {
    Serial.print("Wrong Password");
    Serial.print("Try Again");
    delay(1000);
  }
  else
  {
    j=0;
    Serial.print("New Password:");
    while(j<4)
    {
      char key=keypad_key.getKey();
      if(key)
      {
        initial_password[j]=key;
        Serial.print(key);
        EEPROM.write(j,key);
        j++;
      }
    }
    Serial.print("Pass Changed");
    digitalWrite(yellowled, LOW);
    delay(1000);
  }
  Serial.print("Enter Password");
  key_pressed=0;
}
void initialpassword(){
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    initial_password[j]=EEPROM.read(j);
}

//void beep(){
//  digitalWrite(buzzer, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(200);                       // wait for a second
//  digitalWrite(buzzer, LOW);
//  delay(200);
//}
