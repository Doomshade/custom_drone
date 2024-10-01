#include "Arduino.h"
#ifndef _MPU_H
#define _MPU_H

#include "common.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

typedef struct {
  Adafruit_MPU6050 mpu;

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

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
    FATALLN("Failed to initialize MPU");
    HALT_PROGRAM();
  }

  if (!mpu->mpu.begin()) {
    FATALLN("Failed to find MPU6050 chip. Make sure the MPU is connected to the STL and STA pins!");
    HALT_PROGRAM();
  }
}

static void mpu_init_accel(mpu_t *mpu) {  
  mpu->mpu.setAccelerometerRange(MPU_ACCELEROMETER_RANGE);
  DEBUGL("Accelerometer range set to: ");
  switch (mpu->mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      DEBUG("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      DEBUG("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      DEBUG("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      DEBUG("+-16G");
      break;
  }
  DEBUGLN("");
}

static void mpu_init_gyro(mpu_t *mpu) {
  mpu->mpu.setGyroRange(MPU_GYRO_RANGE);
  DEBUGL("Gyro range set to: ");
  switch (mpu->mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      DEBUG("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      DEBUG("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      DEBUG("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      DEBUG("+- 2000 deg/s");
      break;
  }
  DEBUGLN("");
}

static void mpu_init_filter_bw(mpu_t *mpu) {
  mpu->mpu.setFilterBandwidth(MPU_FILTER_BANDWIDTH);
  DEBUGL("Filter bandwidth set to: ");
  switch (mpu->mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      DEBUG("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      DEBUG("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      DEBUG("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      DEBUG("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      DEBUG("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      DEBUG("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      DEBUG("5 Hz");
      break;
  }
  DEBUGLN("");
}

void mpu_setup(mpu_t *mpu) {
  mpu_init(mpu);
  mpu_init_accel(mpu);
  mpu_init_gyro(mpu);
  mpu_init_filter_bw(mpu);

  mpu->last_debug_msg_ms = 0;
  mpu->ready = true;
  INFOLLN("MPU6050 set up");
}

void mpu_update(mpu_t *mpu) {
  mpu->mpu.getEvent(&mpu->accel, &mpu->gyro, &mpu->temp);
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
  DEBUGB(mpu->accel.acceleration.x, 10);
  DEBUG(", Y: ");
  DEBUGB(mpu->accel.acceleration.y, 10);
  DEBUG(", Z: ");
  DEBUGB(mpu->accel.acceleration.z, 10);
  DEBUGLN(" (m/s^2)");

  DEBUG("Rotation     X: ");
  DEBUGB(mpu->gyro.gyro.x, 10);
  DEBUG(", Y: ");
  DEBUGB(mpu->gyro.gyro.y, 10);
  DEBUG(", Z: ");
  DEBUGB(mpu->gyro.gyro.z, 10);
  DEBUGLN(" (rad/s)");

  DEBUG("Temperature: ");
  DEBUGB(mpu->temp.temperature, 10);
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

    calibrate(&accel_x, mpu->accel.acceleration.x, i);
    calibrate(&accel_y, mpu->accel.acceleration.y, i);
    calibrate(&accel_z, mpu->accel.acceleration.z, i);
    calibrate(&gyro_x, mpu->gyro.gyro.x, i);
    calibrate(&gyro_y, mpu->gyro.gyro.y, i);
    calibrate(&gyro_z, mpu->gyro.gyro.z, i);
    
    delay(200);
  }

  mpu->accel_err[0] = accel_x;
  mpu->accel_err[1] = accel_y;
  mpu->accel_err[2] = accel_z - MPU_GRAVITATIONAL_CONSTANT;

  mpu->gyro_err[0] = gyro_x;
  mpu->gyro_err[1] = gyro_y;
  mpu->gyro_err[2] = gyro_z;

  INFOLLN("Calculated errors:");
  INFOLN("== Acceleration ==");
  INFO("x: ");
  INFO(mpu->accel_err[0]);
  INFOLN(" m/s^2");
  INFO("y: ");
  INFO(mpu->accel_err[1]);
  INFOLN(" m/s^2");
  INFO("z: ");
  INFO(mpu->accel_err[2]);
  INFOLN(" m/s^2");

  INFOLN("");
  INFOLN("== Gyro ==");
  INFO("x: ");
  INFO(mpu->gyro_err[0]);
  INFOLN(" rad/s");
  INFO("y: ");
  INFO(mpu->gyro_err[1]);
  INFOLN(" rad/s");
  INFO("z: ");
  INFO(mpu->gyro_err[2]);
  INFOLN(" rad/s");
}

#endif  // _MPU_H
