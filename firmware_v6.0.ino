/*------Fastrrr---------------------------
Stacey Szigeti
Data logger box

Button #1 = 5 pin
Button #2 = 6 pin
Button #3 = 7 pin

Arduino communicates with the shield using the SPI bus. This is on digital 
pins 11, 12, and 13 on the Uno and pins 50, 51, and 52 on the Mega. On both boards,
pin 10 is used as SS. On the Mega, the hardware SS pin, 53, is not used to select the
W5100, but it must be kept as an output or the SPI interface won't work.
MISO - 50 pin
MOSI - 51 pin
SCK  - 52 pin
Hardware SS - 53 pin  => must be output!
Ethernet SS - 10 pin  => must be output!
SD card  SS - 4 pin   => must be output!

SPI SS  => HIGH (off)

Serial3.begin(57600); Hardware Serial RX/TX buffer increased to 60>>256byte
__#define SERIAL_RX_BUFFER_SIZE 128
__#define SERIAL_TX_BUFFER_SIZE 128
<base Arduino folder>\hardware\arduino\avr\cores\arduino\HardwareSerial.h

MPU I2C module added - old code removed
-pitch/yaw/roll 

*/

#include <Timer.h>
#include <TinyGPS++.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

//----Gyro--------------------------------------------------------------------

// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added seamless Fastwire support
//                 - added note about gyro calibration
//      2012-06-21 - added note about Arduino 1.0.1 + Leonardo compatibility error
//      2012-06-20 - improved FIFO overflow handling and simplified read process
//      2012-06-19 - completely rearranged DMP initialization code and simplification
//      2012-06-13 - pull gyro and accel data from FIFO packet instead of reading directly
//      2012-06-09 - fix broken FIFO read sequence and change interrupt detection to RISING
//      2012-06-05 - add gravity-compensated initial reference frame acceleration output
//                 - add 3D math helper file to DMP6 example sketch
//                 - add Euler output and Yaw/Pitch/Roll output formats
//      2012-06-04 - remove accel offset clearing for better results (thanks Sungon Lee)
//      2012-06-01 - fixed gyro sensitivity to be 2000 deg/sec instead of 250
//      2012-05-30 - basic DMP initialization working

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */



// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
//#define OUTPUT_TEAPOT



//#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
//bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
boolean mpu_FIFO_status;// FIFO overflow
unsigned long mpu_FIFO_count = 0;   // FIFO change count

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

//----Változók deklarálása----------------------------------------------------
Timer t;

String gpsString;
String sensorString;

//----Control panel-----------------------------------------------------------
int inBtn1 = 5;
int inBtn2 = 6;
int inBtn3 = 7;
int DisplayControl = 1;
int count_refresh = 0;

//----Gyro_temp--------------------------------------------------------------------
const int MPU=0x68;  // I2C address of the MPU-6050
int16_t Tmp;
float fAccX,fAccY,fAccZ,fGyX,fGyY,fGyZ;

//----LCD---------------------------------------------------------------------
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 20, 4);

float battery_voltage = 0;
//----Magnetometer------------------------------------------------------------ 
#define address 0x1E //0011110b, I2C 7bit address of HMC5883
int x,y,z,i;
float headingDegrees;

//----SD/Ethernet Shield----------------------------------------------------------
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

const int SdChipSelect = 4;
Sd2Card card;
SdVolume volume;
SdFile root;
uint32_t volumesize;

boolean SdCardOk = LOW;
boolean SdCardLog = LOW;
int SdCardFreeMem = 0;
int BtnPressCount = 0;
String FileName = "";

bool enableFileTransfer = LOW;
bool enableSerial = HIGH;
String filenameFileTransfer = "12262046.LOG";

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 10);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

bool enableEthernet = LOW;

//----The TinyGPS++----------------------------------------------------------- 
TinyGPSPlus gps;

String  gpsDATE, gpsTIME;
double gpsLAT;
double gpsLNG;
unsigned int gpsSAT;
double gpsSOG;
double gpsCOURSE;
String honap,nap,hr,minut,sec;

//-----Ciklusidő mérés--------------------------------------------------------
int cycGPS_feed, cycGPS_main, cycSDwrite, cycSDinfo, cycLCD, cycBluetooth, cycGyro, cycCompass, cycBatteryLevel, cycEthernet, cycOverall;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

void setup() 
{
  pinMode(inBtn1, INPUT_PULLUP);
  pinMode(inBtn2, INPUT_PULLUP);
  pinMode(inBtn3, INPUT_PULLUP);
  
  Serial.begin(115200); //PC
  Serial2.begin(9600);  //Bluetooth
  Serial3.begin(57600); //GPS
  
  //Set baud rate 57600; just RMC; update rate 500ms
  Serial3.println("$PMTK251,57600*2C");
  Serial3.println("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28");
  Serial3.println("$PMTK220,500*2B");
  
  // turn off all spi modules
  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
    
  lcd.begin();
  
  webserver_setup();
  compass_i2c_setup();
  gyro_i2c_setup();
  SD_card_setup();

  t.every(100, GPS_main);
  t.every(100, compass_i2c);
  t.every(100, gyro_i2c);
  t.every(200, LCD);
  t.every(500, bluetooth);
  t.every(5000, battery_monitor);
  t.every(500, SD_card_info);   
  //t.every(500, SD_card);   =>>  GPS rutin futása után közvetlenül
  t.every(500, webserver);
  t.every(500, temperature);
}

void loop() 
{
  GPS_feed();
  t.update();
  gyro_i2c();
  
  file_transfer(enableFileTransfer, filenameFileTransfer);
}









