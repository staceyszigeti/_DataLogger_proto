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

*/

#include <Timer.h>
#include <TinyGPS++.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

//----Változók deklarálása----------------------------------------------------
Timer t;

String gpsString;
String sensorString;

//----Control panel-----------------------------------------------------------
int inBtn1 = 5;
int inBtn2 = 6;
int inBtn3 = 7;
int DisplayControl = 1;

//----LCD---------------------------------------------------------------------
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x3F, 20, 4);

float battery_voltage = 0;
//----Magnetometer------------------------------------------------------------ 
#define address 0x1E //0011110b, I2C 7bit address of HMC5883
int x,y,z,i;
float headingDegrees;

//----Gyro--------------------------------------------------------------------
const int MPU=0x68;  // I2C address of the MPU-6050
int16_t Tmp;
float fAccX,fAccY,fAccZ,fGyX,fGyY,fGyZ;

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
}

void loop() 
{
  GPS_feed();
  t.update();
  
  file_transfer(enableFileTransfer, filenameFileTransfer);
}









