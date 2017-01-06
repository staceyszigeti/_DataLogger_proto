void SD_card() {
  int s =millis();
  // Enable SD card
  digitalWrite(4, LOW);
  
  if (SdCardLog) {
  String dataString = gpsDATE + ";" + gpsTIME + ";" + String(gpsLAT,5) + ";" + String(gpsLNG,5) + ";" + String(gpsSOG,2) + ";" + String(gpsCOURSE,2) + ";" + String(gpsLNG,5) + ";" + String(gpsSOG,2) + ";" + String(gpsCOURSE,2) + sensorString;
  File dataFile = SD.open(FileName.c_str(), FILE_WRITE);
  
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  } else {
    SdCardOk = LOW;
  };
    
  };
    
  // Disable SD card
  digitalWrite(4, HIGH);

  int sto = millis();
  cycSDwrite=sto-s;

}


