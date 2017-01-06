void file_transfer(boolean enable, String filename)
{
 if (enable && SdCardOk && !SdCardLog) {
   
   if (SD.begin(SdChipSelect)) 
   {
     Serial.println("SD card error!");
   } else
   {
     Serial.println("SD card initialize!");
     Serial.println("$START");
     Serial.println(filename); 
     File dataFile = SD.open(filename.c_str());
      // if the file is available, write to it:
      if (dataFile) {
        while (dataFile.available()) {
          Serial.write(dataFile.read());
        }
        dataFile.close();
        enableFileTransfer = !enableFileTransfer;
        Serial.println("$END"); 
      } else {
        Serial.println("error opening datalog.txt");
        enableFileTransfer = !enableFileTransfer;
      };
   };  
  };
  
}
