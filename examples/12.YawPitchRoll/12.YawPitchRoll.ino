/*!
 * @file 01.SixAxisData.ino
 * @brief Read 6-axis data (accelerometer + gyroscope) from BMI323
 * @details This example demonstrates how to read raw accelerometer and gyroscope data from BMI323 sensor.
 * @copyright Copyright (c) 2025 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [Martin](Martin@dfrobot.com)
 * @version V1.0.0
 * @date 2025-12-08
 * @url https://github.com/DFRobot/DFRobot_BMI323
 */

#include "DFRobot_BMI323.h"
#include <MadgwickAHRS.h>

#define BMI323_CS_PIN D9    // Set to the pin connected to the sensor's CS pin

DFRobot_BMI323 bmi323(&SPI, BMI323_CS_PIN);
Madgwick filter;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("BMI323 Yaw, Pitch and Roll Data");
  Serial.println("====================");

  while (!bmi323.begin()) {
    Serial.println("Sensor init failed, please check wiring. Retry in 1s.");
    delay(1000);
  }
  Serial.print("Chip ID: ");
  Serial.print("0x");
  Serial.println(bmi323.getChipID(), HEX);

  if (!bmi323.configAccel(bmi323.eAccelODR50Hz, bmi323.eAccelRange8G, bmi323.eAccelModeNormal)) {
    Serial.println("Accel config failed!");
    while (1) {
      delay(1000);
    }
  }

  if (!bmi323.configGyro(bmi323.eGyroODR800Hz, bmi323.eGyroRange2000DPS, bmi323.eGyroModeNormal)) {
    Serial.println("Gyro config failed!");
    while (1) {
      delay(1000);
    }
  }

  delay(1000);
  Serial.println("Setup complete!\n");
}

void loop()
{
  DFRobot_BMI323::sSensorData accel;
  DFRobot_BMI323::sSensorData gyro;

  if (bmi323.getAccelGyroData(&accel, &gyro)) {
    filter.updateIMU(gyro.x, gyro.y, gyro.z, accel.x, accel.y, accel.z);
    float roll  = filter.getRoll();
    float pitch = filter.getPitch();
    float yaw   = filter.getYaw();
    Serial.print("Pitch: ");   Serial.print(pitch, 1);
    Serial.print(" \tRoll: "); Serial.print(roll, 1);
    Serial.print(" \tYaw: ");  Serial.println(yaw, 1);
  } else {
    Serial.println("Failed to read sensor data.");
  }
  delay(10);
}
