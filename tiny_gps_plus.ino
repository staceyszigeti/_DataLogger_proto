static void GPS_feed()
{   
    int s =millis(); 
    while (Serial3.available() > 0)
    gps.encode(Serial3.read());
    
    if (gps.date.month()<10)
    {honap = "0"+String(gps.date.month());}
    else {honap = String(gps.date.month());};
    
    if (gps.date.day()<10)
    {nap = "0"+String(gps.date.day());}
    else {nap = String(gps.date.day());};

    if (gps.time.hour()<10)
    {hr = "0"+String(gps.time.hour());}
    else {hr = String(gps.time.hour());};
    
    if (gps.time.minute()<10)
    {minut = "0"+String(gps.time.minute());}
    else {minut = String(gps.time.minute());};
    
    if (gps.time.second()<10)
    {sec = "0"+String(gps.time.second());}
    else {sec = String(gps.time.second());};
    
    gpsTIME = hr+":"+minut+":"+sec;
    gpsDATE = String(gps.date.year())+"-"+honap+"-"+nap;
    gpsLAT = gps.location.lat();
    gpsLNG = gps.location.lng();
    gpsSOG = gps.speed.knots();        
    gpsCOURSE = gps.course.deg();
    gpsSAT = gps.satellites.value();
    
    int sto = millis();
    cycGPS_feed=sto-s;  
}

static void GPS_main()
{
    int s =millis(); 
    
    gpsString = "<gps DATE=" + gpsDATE + " TIME=" + gpsTIME + " LAT=" + String(gpsLAT,5) + " LNG=" + String(gpsLNG,5) + " ALT=" + String(000) + " SOG=" + String(gpsSOG,2) + " COURSE=" + String(gpsCOURSE,2) + " SAT=" + String(gpsSAT) + ">";
    if (enableSerial) {
    Serial.println(gpsString);
    };
    
    SD_card();
    
    int sto = millis();
    cycGPS_main=sto-s; 
}



