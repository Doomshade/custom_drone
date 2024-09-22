#ifndef _RECVR_H
#define _RECVR_H
#include <Arduino.h>
#include "common.h"

// Sets up the receiver
void recvr_setup(struct recvr_t *recvr);
void recvr_read(struct recvr_t *recvr);

struct recvr_t {
  unsigned long throttle;
  unsigned long yaw;
  unsigned long pitch;
  unsigned long idk;
  unsigned long arm;
};

void recvr_setup(struct recvr_t *recvr) {
  pinMode(RECVR_CHANNEL_PIN_THROTTLE, INPUT);
  pinMode(RECVR_CHANNEL_PIN_YAW, INPUT);
  pinMode(RECVR_CHANNEL_PIN_PITCH, INPUT);
  pinMode(RECVR_CHANNEL_PIN_IDK, INPUT);
  pinMode(RECVR_CHANNEL_PIN_ARM, INPUT);
}

void recvr_read(struct recvr_t *recvr) {
  recvr->throttle = pulseIn(RECVR_CHANNEL_PIN_THROTTLE, HIGH, 30000);
  recvr->yaw = pulseIn(RECVR_CHANNEL_PIN_YAW, HIGH, 30000);
  recvr->pitch = pulseIn(RECVR_CHANNEL_PIN_PITCH, HIGH, 30000);
  recvr->idk = pulseIn(RECVR_CHANNEL_PIN_IDK, HIGH, 30000);
  recvr->arm = pulseIn(RECVR_CHANNEL_PIN_ARM, HIGH, 30000);
}


#endif  // RECVR_H
