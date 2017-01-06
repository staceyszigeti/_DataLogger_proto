static void gyro_i2c_setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

static void gyro_i2c()
{
  int s = millis();
  int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;
  int LSB_Acc, LSB_Gyro;
  LSB_Acc=16384;
  LSB_Gyro=131;
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Tmp=Tmp/340.00+36.53;

  fAccX=AcX/LSB_Acc;
  fAccY=AcY/LSB_Acc;
  fAccZ=AcZ/LSB_Acc;
  fGyX=GyX/LSB_Gyro;
  fGyY=GyY/LSB_Gyro;
  fGyZ=GyZ/LSB_Gyro;

  sensorString = "<sensor AccX=" + String(fAccX) + " AccY=" + String(fAccY) + " AccZ=" + String(fAccZ) + " GyX=" + String(fGyX) + " GyY=" + String(fGyY) + " GyZ=" + String(fGyZ) + " TEMP=" + String(Tmp) + " HEADING=" + String(headingDegrees) + ">";

  if (enableSerial) {
  Serial.println(sensorString);
  };
  
  int sto = millis();
  cycGyro=sto-s;

}
