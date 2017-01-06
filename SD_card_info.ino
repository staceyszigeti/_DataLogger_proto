void SD_card_info() {
  int s =millis();
  // Enable SD card
  digitalWrite(4, LOW);

  if (!volume.init(card)) {
    SdCardOk = LOW;
  } 
  // print the type and size of the first FAT-type volume  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                         // SD card blocks are always 512 bytes
  volumesize /= 1048576;
  
  SdCardFreeMem = volumesize;
  
  // Disable SD card
  digitalWrite(4, HIGH);
  
  int sto = millis();
  cycSDinfo=sto-s;

}
