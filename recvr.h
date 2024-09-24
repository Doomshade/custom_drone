#ifndef _RECVR_H
#define _RECVR_H
#include <Arduino.h>
#include "common.h"

typedef struct {
  uint16_t throttle;
  uint16_t yaw;
  uint16_t pitch;
  uint16_t roll;
  bool arm;
} recvr_t;

// Sets up the receiver
void recvr_setup(recvr_t *recvr);
void recvr_read(recvr_t *recvr);

void recvr_setup(recvr_t *recvr) {
  pinMode(RECVR_CHANNEL_PIN_THROTTLE, INPUT);
  pinMode(RECVR_CHANNEL_PIN_YAW, INPUT);
  pinMode(RECVR_CHANNEL_PIN_PITCH, INPUT);
  pinMode(RECVR_CHANNEL_PIN_ROLL, INPUT);
  pinMode(RECVR_CHANNEL_PIN_ARM, INPUT);
}

void recvr_read(recvr_t *recvr) {
  recvr->throttle = pulseIn(RECVR_CHANNEL_PIN_THROTTLE, HIGH, 30000);
  recvr->yaw = pulseIn(RECVR_CHANNEL_PIN_YAW, HIGH, 30000);
  recvr->pitch = pulseIn(RECVR_CHANNEL_PIN_PITCH, HIGH, 30000);
  recvr->roll = pulseIn(RECVR_CHANNEL_PIN_ROLL, HIGH, 30000);
  recvr->arm = pulseIn(RECVR_CHANNEL_PIN_ARM, HIGH, 30000);
}


#endif  // RECVR_H
