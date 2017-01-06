static void battery_monitor()
{
   int s = millis();
   int analogPin = 1;
   int analog_battery = analogRead(A0);
   battery_voltage = analog_battery * 0.0172 +1.6324; 
   
   int sto = millis();
   cycBatteryLevel=sto-s; 
}
