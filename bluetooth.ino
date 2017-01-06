static void bluetooth()
{
  int s =millis();
  Serial2.println(gpsString);
  Serial2.println(sensorString);
  int sto = millis();
  cycBluetooth=sto-s;
}
