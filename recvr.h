#ifndef _RECVR_H
#define _RECVR_H
#include <Arduino.h>
#include "common.h"

typedef struct {
  uint16_t throttle;
  uint16_t yaw;
  uint16_t pitch;
  uint16_t roll;
  uint16_t arm;

  bool debug = false;
  unsigned long last_debug_msg_ms;
} recvr_t;

// Sets up the receiver
void recvr_setup(recvr_t *recvr);
void recvr_read(recvr_t *recvr);
void recvr_debug(recvr_t *recvr);
bool recvr_debug_enabled(recvr_t *recvr);
void recvr_debug_enable(recvr_t *recvr);
void recvr_debug_disable(recvr_t *recvr);

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

void recvr_debug(recvr_t *recvr) {
  if (!recvr_debug_enabled(recvr)) return;

  unsigned long dt = millis() - recvr->last_debug_msg_ms;
  if (dt < MPU_DEBUG_MILLIS) return;

  DEBUGL("Throttle: ");
  DEBUGLN(recvr->throttle);

  DEBUGL("Yaw: ");
  DEBUGLN(recvr->yaw);

  DEBUGL("Pitch: ");
  DEBUGLN(recvr->pitch);

  DEBUGL("Roll: ");
  DEBUGLN(recvr->roll);

  DEBUGL("Arm: ");
  DEBUGLN(recvr->arm);
}


bool recvr_debug_enabled(recvr_t *recvr) {
  return recvr->debug;
}
void recvr_debug_enable(recvr_t *recvr) {
  recvr->debug = true;
}
void recvr_debug_disable(recvr_t *recvr) {
  recvr->debug = false;
}


#endif  // RECVR_H
