static void SD_card_setup()
{  
// see if the card is present and can be initialized:
  if (!SD.begin(SdChipSelect)) 
  {
    SdCardOk = LOW;
  } else
  {
    SdCardOk = HIGH;
  };
  
}
