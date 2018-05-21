// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// KXTF9-4100
// This code is designed to work with the KXTF9-4100_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include<Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// KXTF9-4100 I2C address is 0x0F(15)
#define Addr 0x0F
#define GYRO 0x68         // gyro I2C address

RF24 radio(7, 8); // CE, CSN

const byte sendAddress[6] = "10011";
const byte receiveAddress[6] = "01100";

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(sendAddress);
  radio.openReadingPipe(0, receiveAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  writeTo(GYRO, 0x3D, 0x08);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select Control register1
  Wire.write(0x1B);
  // Operating mode, 12-bit valid
  Wire.write(0xD0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); // start transmission to device 
  Wire.write(address);             // write register address
  Wire.write(val);                 // write value to write
  Wire.endTransmission();         // end transmission
}

void loop()
{
  unsigned int data[6];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x06);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 bytes of data
  // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  // Convert the data to 12-bits
  int xAccl = (data[1] * 256 + (data[0] & 0xF0)) / 16;
  if (xAccl > 2047)
  {
    xAccl -= 4096;
  }
  int yAccl = (data[3] * 256 + (data[2] & 0xF0)) / 16;
  if (yAccl > 2047)
  {
    yAccl -= 4096;
  }
  int zAccl = (data[5] * 256 + (data[4] & 0xF0)) / 16;
  if (zAccl > 2047)
  {
    zAccl -= 4096;
  }

  int mag = xAccl*0.91;
  int dir = yAccl*0.91;

  if(mag > 255)
    mag = 255;
  else if(mag < -255)
    mag = -255;

  if(dir > 255)
    dir = 255;
  else if(dir < -255)
    dir = -255;

  // Output data to serial monitor
  Serial.print("Magnitude:");
  Serial.print(xAccl);
  Serial.print(", Rotaton:");
  Serial.println(yAccl);

  const int text[2] = {mag, dir};
  radio.write(&text, sizeof(text));
  
  delay(1000);
}
