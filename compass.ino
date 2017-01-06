static void compass_i2c_setup()
{
  Wire.begin();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

static void compass_i2c()
{
   int s = millis();
   //Tell the HMC5883 where to begin reading data
   Wire.beginTransmission(address);
   Wire.write(0x03); //select register 3, X MSB register
   Wire.endTransmission();
        
   //Read data from each axis, 2 registers per axis
   Wire.requestFrom(address, 6);
   if(6<=Wire.available())
     {
       x = Wire.read()<<8; //X msb
       x |= Wire.read(); //X lsb
       z = Wire.read()<<8; //Z msb
       z |= Wire.read(); //Z lsb
       y = Wire.read()<<8; //Y msb
       y |= Wire.read(); //Y lsb
     };
   float heading = atan2(y, z);
       
   // Your mrad result / 1000.00 (to turn it into radians).
   float declinationAngle = 72.14  / 1000.0;
   // If you have an EAST declination, use += declinationAngle, if you have a WEST declination, use -= declinationAngle
   heading += declinationAngle;
   headingDegrees = heading * 180/M_PI;
        
   if(z < 0)
   {    
     headingDegrees = 180 - headingDegrees;
   }
   else if (z > 0 && y < 0)
   {
     headingDegrees = headingDegrees * -1;
   }
   else if (z > 0 && y > 0)
   {
     headingDegrees = 360 - headingDegrees;
   }
   else if (z == 0 && y < 0)
   {
     headingDegrees = 90;
   }
   else if (z == 0 && y > 0)
   {
     headingDegrees = 270;
   };
   
   int sto = millis();
   cycCompass=sto-s;

   
}
