#include "Arduino.h"
#ifndef _MPU_H
#define _MPU_H

#include "common.h"
#include <Wire.h>
#define MPU_ADDRESS (0x68)  // I2C address of the MPU-6050
#define X (0)
#define Y (1)
#define Z (2)

typedef struct {
  int acc_raw[3];
  int gyro_raw[3];
  int temp;

  // xyz coords
  float accel_err[3];
  float gyro_err[3];
  bool ready = false;
  bool debug = false;
  unsigned long last_debug_msg_ms;
} mpu_t;

void mpu_setup(mpu_t *mpu);
void mpu_update(mpu_t *mpu);
void mpu_debug_enable(mpu_t *mpu);
void mpu_debug_disable(mpu_t *mpu);
void mpu_debug(mpu_t *mpu);
bool mpu_debug_enabled(mpu_t *mpu);
void mpu_gyro_calibrate(mpu_t *mpu);

static void mpu_init(mpu_t *mpu) {
  if (!mpu) {
    FATALLLN("Failed to initialize MPU");
    HALT_PROGRAM();
  }

  Wire.begin();
  TWBR = 12;

  // Configure power management
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x6B);                    // Request the PWR_MGMT_1 register
  Wire.write(0x00);                    // Apply the desired configuration to the register
  Wire.endTransmission();              // End the transmission

  // Configure the gyro's sensitivity
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x1B);                    // Request the GYRO_CONFIG register
  Wire.write(0x08);                    // Apply the desired configuration to the register : ±500°/s
  Wire.endTransmission();              // End the transmission
   
  // Configure the acceleromter's sensitivity
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x1C);                    // Request the ACCEL_CONFIG register
  Wire.write(0x10);                    // Apply the desired configuration to the register : ±8g
  Wire.endTransmission();              // End the transmission

  // Configure low pass filter
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x1A);                    // Request the CONFIG register
  Wire.write(0x03);                    // Set Digital Low Pass Filter about ~43Hz
  Wire.endTransmission();              // End the transmission
}

void mpu_setup(mpu_t *mpu) {
  mpu_init(mpu);

  mpu->last_debug_msg_ms = 0;
  mpu->ready = true;
  INFOLLN("MPU6050 set up");
}

void mpu_update(mpu_t *mpu) {
  Wire.beginTransmission(MPU_ADDRESS); // Start communicating with the MPU-6050
  Wire.write(0x3B);                    // Send the requested starting register
  Wire.endTransmission();              // End the transmission
  Wire.requestFrom(MPU_ADDRESS,14);    // Request 14 bytes from the MPU-6050

  // Wait until all the bytes are received
  while(Wire.available() < 14);

  mpu->acc_raw[X]  = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[X] variable
  mpu->acc_raw[Y]  = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[Y] variable
  mpu->acc_raw[Z]  = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[Z] variable
  mpu->temp = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the temperature variable
  mpu->gyro_raw[X] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[X] variable
  mpu->gyro_raw[Y] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[Y] variable
  mpu->gyro_raw[Z] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[Z] variable
}

void mpu_debug_enable(mpu_t *mpu) {
  mpu->debug = true;
  DEBUGLLN("MPU debug enabled");
}


void mpu_debug_disable(mpu_t *mpu) {
  mpu->debug = false;
  DEBUGLLN("MPU debug disabled");
}

void mpu_debug(mpu_t *mpu) {
  if (!mpu || !mpu_debug_enabled(mpu)) return;
  
  unsigned long dt = millis() - mpu->last_debug_msg_ms;
  if (dt < MPU_DEBUG_MILLIS) return;
  
  DEBUG("Acceleration X: ");
  DEBUG(mpu->acc_raw[X] - mpu->accel_err[X]);
  DEBUG(", Y: ");
  DEBUG(mpu->acc_raw[Y] - mpu->accel_err[Y]);
  DEBUG(", Z: ");
  DEBUG(mpu->acc_raw[Z] - mpu->accel_err[Z]);
  DEBUGLN(" (m/s^2)");

  DEBUG("Rotation     X: ");
  DEBUG(mpu->gyro_raw[X] - mpu->gyro_err[X]);
  DEBUG(", Y: ");
  DEBUG(mpu->gyro_raw[Y] - mpu->gyro_err[Y]);
  DEBUG(", Z: ");
  DEBUG(mpu->gyro_raw[Z] - mpu->gyro_err[Z]);
  DEBUGLN(" (rad/s)");

  DEBUG("Temperature: ");
  DEBUG(mpu->temp);
  DEBUGLN(" (°C)");

  DEBUGLN("");

  mpu->last_debug_msg_ms = millis();
}

bool mpu_debug_enabled(mpu_t *mpu) {
  return mpu->debug;
}

static inline void calibrate(float* prev_val, float new_val, size_t num_try) {
  if (num_try == 0) {
    *prev_val = new_val;
  } else {
    *prev_val += (new_val - *prev_val) / num_try;
  }
}
void mpu_gyro_calibrate(mpu_t *mpu) {
  INFOLLN("Calibrating gyro, this will take a while...");

  float accel_x, accel_y, accel_z;
  float gyro_x, gyro_y, gyro_z;
  for(size_t i = 0; i < MPU_CALIBRATION_TRIES; ++i) {
    mpu_update(mpu);

    calibrate(&accel_x, mpu->acc_raw[X], i);
    calibrate(&accel_y, mpu->acc_raw[Y], i);
    calibrate(&accel_z, mpu->acc_raw[Z], i);
    calibrate(&gyro_x, mpu->gyro_raw[X], i);
    calibrate(&gyro_y, mpu->gyro_raw[Y], i);
    calibrate(&gyro_z, mpu->gyro_raw[Z], i);
    
    delay(200);
  }

  mpu->accel_err[0] = accel_x;
  mpu->accel_err[1] = accel_y;
  mpu->accel_err[2] = accel_z + MPU_GRAVITATIONAL_CONSTANT;

  mpu->gyro_err[0] = gyro_x;
  mpu->gyro_err[1] = gyro_y;
  mpu->gyro_err[2] = gyro_z;

  INFOLLN("Calculated errors:");
  INFOLN("== Acceleration ==");
  INFO("x: ");
  INFOB(mpu->accel_err[0], 10);
  INFOLN(" m/s^2");
  INFO("y: ");
  INFOB(mpu->accel_err[1], 10);
  INFOLN(" m/s^2");
  INFO("z: ");
  INFOB(mpu->accel_err[2], 10);
  INFOLN(" m/s^2");

  INFOLN("");
  INFOLN("== Gyro ==");
  INFO("x: ");
  INFOB(mpu->gyro_err[0], 10);
  INFOLN(" rad/s");
  INFO("y: ");
  INFOB(mpu->gyro_err[1], 10);
  INFOLN(" rad/s");
  INFO("z: ");
  INFOB(mpu->gyro_err[2], 10);
  INFOLN(" rad/s");
}

#endif  // _MPU_H
