#ifndef _MPU_H
#define _MPU_H

#include "common.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

struct mpu_t {
  Adafruit_MPU6050 mpu;
  bool ready = false;
  bool debug = false;
};

void mpu_setup(struct mpu_t *mpu);
void mpu_debug_enable(struct mpu_t *mpu);
void mpu_debug_disable(struct mpu_t *mpu);
void mpu_debug(struct mpu_t *mpu);
bool mpu_debug_enabled(struct mpu_t *mpu);
void mpu_gyro_calibrate(struct mpu_t *mpu);

void mpu_setup(struct mpu_t *mpu) {
  if (!mpu->mpu.begin()) {
    FATALLN("Failed to find MPU6050 chip! Make sure the gyro is connected to the STL and STA pins!");
    while (1) delay(50);
    return;
  }

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

  mpu->ready = true;
  INFOLLN("MPU6050 set up");
}


void mpu_debug_enable(struct mpu_t *mpu) {
  mpu->debug = true;
  DEBUGLLN("MPU debug enabled");
}


void mpu_debug_disable(struct mpu_t *mpu) {
  mpu->debug = false;
  DEBUGLLN("MPU debug disabled");
}

void mpu_debug(struct mpu_t *mpu) {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu->mpu.getEvent(&a, &g, &temp);
  /* Print out the values */
  DEBUG("Acceleration X: ");
  DEBUGB(a.acceleration.x, 10);
  DEBUG(", Y: ");
  DEBUGB(a.acceleration.y, 10);
  DEBUG(", Z: ");
  DEBUGB(a.acceleration.z, 10);
  DEBUGLN(" m/s^2");

  DEBUG("Rotation     X: ");
  DEBUGB(g.gyro.x, 10);
  DEBUG(", Y: ");
  DEBUGB(g.gyro.y, 10);
  DEBUG(", Z: ");
  DEBUGB(g.gyro.z, 10);
  DEBUGLN(" rad/s");

  DEBUG("Temperature: ");
  DEBUGB(temp.temperature, 10);
  DEBUGLN(" degC");

  DEBUGLN("");
  delay(500);
}

bool mpu_debug_enabled(struct mpu_t *mpu) {
  return mpu->debug;
}

void mpu_gyro_calibrate(struct mpu_t *mpu) {
  ERRORLLN("Gyro calibration not yet implemented");
}

#endif  // _MPU_H