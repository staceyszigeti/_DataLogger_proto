static void LCD()
{
  int s =millis();
  
  if (digitalRead(inBtn1) == LOW)
    {
      if (DisplayControl == 12)
        {
          DisplayControl = 0;
        };
      DisplayControl = DisplayControl + 1;
      lcd.clear();
    };
  if (digitalRead(inBtn2) == LOW)
    {
      if (DisplayControl == 1)
        {
          DisplayControl = 13;
        };
      DisplayControl = DisplayControl - 1;
      lcd.clear();
    };  
  
  switch (DisplayControl) {
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("FASTRRR 2015"); 
      lcd.setCursor(0, 1);
      lcd.print("DATE: "); lcd.print(gpsDATE);
      lcd.setCursor(0, 2);
      lcd.print("TIME: "); lcd.print(gpsTIME); lcd.print(" (GMT)");
      lcd.setCursor(0, 3);
      lcd.print("SAT: "); lcd.print(gpsSAT); lcd.setCursor(10, 3); lcd.print("TEMP: "); lcd.print(Tmp); lcd.print(char(223));;  
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("GPS debugging "); 
      lcd.setCursor(0, 1);
      lcd.print("S.F: "); lcd.print(gps.sentencesWithFix());
      lcd.setCursor(11, 1);
      lcd.print("HOPD: "); lcd.print(gps.hdop.value());
      lcd.setCursor(0, 2);
      lcd.print("P.Checksum: "); lcd.print(gps.passedChecksum());
      lcd.setCursor(0, 3);
      lcd.print("F.Checksum: "); lcd.print(gps.failedChecksum());
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("LAT: "); lcd.print(gps.location.rawLat().negative); lcd.print(" "); lcd.print(gpsLAT,5); lcd.print(char(223));lcd.print(" "); lcd.setCursor(19, 0); lcd.print(gps.location.isValid());      
      lcd.setCursor(0, 1); 
      lcd.print("LNG: "); lcd.print(gps.location.rawLat().negative); lcd.print(" "); lcd.print(gpsLNG,5); lcd.print(char(223));lcd.print(" "); lcd.setCursor(19, 1); lcd.print(gps.location.isValid());
      lcd.setCursor(0, 2);
      lcd.print("SPEED: "); lcd.print(gps.speed.knots(),3); lcd.print(" kts"); lcd.setCursor(19, 2); lcd.print(gps.speed.isValid());     
      lcd.setCursor(0, 3); 
      lcd.print("COURSE: "); lcd.print(gps.course.deg(), 3); lcd.print(char(223)); lcd.setCursor(19, 3); lcd.print(gps.course.isValid());
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Heading: ");
      lcd.print(headingDegrees);
      lcd.print((char)223);
      lcd.setCursor(0, 1);
      lcd.print(x); lcd.print(" "); lcd.print(y); lcd.print(" "); lcd.print(z);
      lcd.setCursor(0, 0);
      break;
    case 5:
      count_refresh++;
      if ( count_refresh > 5) {
        lcd.clear();
        count_refresh=0;
      };
      lcd.setCursor(0, 0);
      lcd.print("Yaw Pitch Roll [deg]"); 
      lcd.setCursor(0, 1);
      lcd.print(ypr[0] * 180/M_PI,1); lcd.print(" "); lcd.print(ypr[1] * 180/M_PI,1); lcd.print(" "); lcd.print(ypr[2] * 180/M_PI,1);
      lcd.setCursor(0, 2);
      lcd.print("FIFO status: ");
      lcd.setCursor(0, 3);
      lcd.print(mpu_FIFO_status); lcd.setCursor(10, 3); lcd.print(mpu_FIFO_count);
      //lcd.print(aaReal.x,1); lcd.print(" "); lcd.print(aaReal.y,1); lcd.print(" "); lcd.print(aaReal.z,1);
      break;
    case 6:
      lcd.setCursor(0, 0);
      lcd.println("Battery voltage: ");
      lcd.setCursor(0, 1);
      lcd.print(battery_voltage); lcd.print("V");
      lcd.setCursor(0, 0);
      break;
    case 7:
      if (digitalRead(inBtn3) == LOW) {
        BtnPressCount++;
      };
      
      if (SdCardOk == HIGH && BtnPressCount == 15 && SdCardLog ==0) { //15*200ms = 3s
      SdCardLog = 1;
      BtnPressCount = 0;
      FileName = honap+nap+hr+minut+ ".LOG";
      };
      
      if (BtnPressCount == 15 && SdCardLog ==1) { //15*200ms = 3s
      SdCardLog = 0;
      BtnPressCount = 0;
      };
      
      lcd.setCursor(0, 0);
      if (SdCardOk) { 
      lcd.println("SD card: "); lcd.print("OK");
      } else {
      lcd.println("SD card: "); lcd.print("-");
      };
      lcd.setCursor(0, 1);
      lcd.print("Free space:"); lcd.print(SdCardFreeMem); lcd.print(" MB");
      lcd.setCursor(0, 2);
      lcd.print("Start: "); lcd.print(SdCardLog);
      lcd.setCursor(0, 3);
      lcd.print(FileName);
      break;
    case 8:
      if (digitalRead(inBtn3) == LOW) {
        BtnPressCount++;
      };
      
      if (BtnPressCount == 15) { //15*200ms = 3s
      enableEthernet =  !enableEthernet;
      BtnPressCount = 0;
      lcd.clear();
      };
      
      lcd.setCursor(0, 0);
      if (enableEthernet) { 
        lcd.setCursor(0, 2);
        lcd.print(Ethernet.localIP());
        lcd.setCursor(0, 3);
        lcd.print("RUNNING");
      } else {
        lcd.setCursor(0, 2);
        lcd.print("0.0.0.0");
        lcd.setCursor(0, 3);
        lcd.print("");        
      };
      lcd.setCursor(0, 0);
      lcd.print("Ethernet: ");
      lcd.setCursor(0, 1);
      lcd.print("IP addr.: ");
      break;
    case 9:
      lcd.setCursor(0, 0);
      lcd.print("Cycle time (ms): 1"); 
      lcd.setCursor(0, 1);
      lcd.print("GPS feed: "); lcd.print(cycGPS_feed);
      lcd.setCursor(0, 2);
      lcd.print("GPS main: "); lcd.print(cycGPS_main);
      lcd.setCursor(0, 3);
      lcd.print("SD write: "); lcd.print(cycSDwrite);
      break;
    case 10:  
      lcd.setCursor(0, 0);
      lcd.print("Cycle time (ms): 2"); 
      lcd.setCursor(0, 1);
      lcd.print("SD info: "); lcd.print(cycSDinfo);
      lcd.setCursor(0, 2);
      lcd.print("LCD: "); lcd.print(cycLCD);
      lcd.setCursor(0, 3);
      lcd.print("Bluetooth: "); lcd.print(cycBluetooth);
      break;
    case 11:
      lcd.setCursor(0, 0);
      lcd.print("Cycle time (ms): 3"); 
      lcd.setCursor(0, 1);
      lcd.print("Compass: "); lcd.print(cycCompass);
      lcd.setCursor(0, 2);
      lcd.print("Gyro: "); lcd.print(cycGyro);
      lcd.setCursor(0, 3);
      lcd.print("Ethernet: "); lcd.print(cycEthernet);
      break;
    case 12:
      if (digitalRead(inBtn3) == LOW) {
        BtnPressCount++;
      };
      
      if (BtnPressCount == 15) { //15*200ms = 3s
      enableFileTransfer =  !enableFileTransfer;
      BtnPressCount = 0;
      lcd.clear();
      };
      
      lcd.setCursor(0, 0);
      if (enableFileTransfer && SdCardOk && !SdCardLog) { 
        enableSerial = LOW;
        lcd.setCursor(0, 3);
        lcd.print("SENDING");
      } else {
        enableSerial = HIGH;
        enableFileTransfer = LOW;
        lcd.setCursor(0, 3);
        lcd.print("");        
      };
      lcd.setCursor(0, 0);
      lcd.print("File transfer ");
      lcd.setCursor(0, 1);
      lcd.print("Filename: "); 
      lcd.setCursor(0, 2);
      lcd.print(filenameFileTransfer);
      break;
  }; 
  int sto = millis();
  cycLCD=sto-s;   
}
