#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte recvaddress[6] = "10011";
const byte sendaddress[6] = "01100";
  
// Motor A

int enA = 10;
int in1 = 4;
int in2 = 3;

// Motor B

int enB = 11;
int in3 = 5;
int in4 = 6;
 
// Motor Speed Values - Start at zero

int MotorSpeed1 = 0;
int MotorSpeed2 = 0;

// Joystick Values - Start at 512 (middle position)

int joyposVert = 0;
int joyposHorz = 0;  


void setup()

{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(sendaddress);
  radio.openReadingPipe(0, recvaddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  // Set all the motor control pins to outputs

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
   
  // Start with motors disabled and direction forward
  
  // Motor A
  
  digitalWrite(enA, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  // Motor B
  
  digitalWrite(enB, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
} 

void loop() {

  // Read the Joystick X and Y positions

  if (radio.available()) {
    int data[2];
    radio.read(&data, sizeof(data));
    joyposVert = data[0]; 
    joyposHorz = data[1];
    Serial.print("data: ");
    Serial.print(data[0]);
    Serial.print(" ");
    Serial.println(data[1]);
    // Determine if this is a forward or backward motion
    // Do this by reading the Verticle Value
    // Apply results to MotorSpeed and to Direction
  
    if (joyposVert < 0)
    {
      // This is Backward
  
      // Set Motor A backward
  
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
  
      // Set Motor B backward
  
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
  
      //Determine Motor Speeds
  
      // As we are going backwards we need to reverse readings 
  
      MotorSpeed1 = joyposVert * -1;
      MotorSpeed2 = joyposVert * -1;
  
    }
    else if (joyposVert > 0)
    {
      // This is Forward
  
      // Set Motor A forward
  
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
  
      // Set Motor B forward
  
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
  
      //Determine Motor Speeds
  
      MotorSpeed1 = joyposVert;
      MotorSpeed2 = joyposVert;
  
    }
    else
    {
      // This is Stopped
  
      MotorSpeed1 = 0;
      MotorSpeed2 = 0; 
  
    }
    
    // Now do the steering
    // The Horizontal position will "weigh" the motor speed
    // Values for each motor
  
    if (joyposHorz < 0)
    {
      // Move Left
  
      // As we are going left we need to reverse readings
  
      joyposHorz = joyposHorz * 3;  // Make the number positive
  
      // Map the number to a value of 255 maximum
  
      MotorSpeed1 = MotorSpeed1 - joyposHorz* -1;;
      MotorSpeed2 = MotorSpeed2 + joyposHorz* -1;;
  
    }
    else if (joyposHorz > 0)
    {
      // Move Right
  
      // Map the number to a value of 255 maximum
          
      joyposHorz = joyposHorz * 3;
      MotorSpeed1 = MotorSpeed1 + joyposHorz;
      MotorSpeed2 = MotorSpeed2 - joyposHorz;    
  
    }

    if(joyposHorz>=200){
      MotorSpeed1=200;
      MotorSpeed2=200;
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

    }
    else if(joyposHorz>=100){
      MotorSpeed1=100;
      MotorSpeed2=100;
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

    }
    else if(joyposHorz<=-200){
      MotorSpeed1=200;
      MotorSpeed2=200;
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
     else if(joyposHorz<=-100){
      MotorSpeed1=100;
      MotorSpeed2=100;
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    if(joyposVert>=100){
      MotorSpeed1=150;
      MotorSpeed2=150;
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
    else if(joyposVert>=50){
      MotorSpeed1=75;
      MotorSpeed2=75;
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
    else if(joyposVert<=-100){
      MotorSpeed1=150;
      MotorSpeed2=150;
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    else if(joyposVert<=-50){
      MotorSpeed1=100;
      MotorSpeed2=100;
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    
    if(MotorSpeed1>255)
    {
      MotorSpeed1 = 255;
    }
    if(MotorSpeed2>255)
    {
      MotorSpeed2 = 255;
    }

    if(MotorSpeed1<0)
    {
      MotorSpeed1 = 0;
    }
    if(MotorSpeed2<0)
    {
      MotorSpeed2 = 0;
    }
    
    
    // Set the motor speeds
    //Serial.print("Speeds: ");
    //Serial.print(MotorSpeed1);
    //Serial.print(" ");
    //Serial.println(MotorSpeed2);
    analogWrite(enA, MotorSpeed1);
    analogWrite(enB, MotorSpeed2);
  }

}
