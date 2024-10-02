#ifndef _RECVR_H
#define _RECVR_H
#include <Arduino.h>
#include "common.h"

typedef struct {
  int throttle;
  int yaw;
  int pitch;
  int roll;
  int arm;

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
  recvr->throttle = analogRead(RECVR_CHANNEL_PIN_THROTTLE);
  recvr->yaw = analogRead(RECVR_CHANNEL_PIN_YAW);
  recvr->pitch = analogRead(RECVR_CHANNEL_PIN_PITCH);
  recvr->roll = analogRead(RECVR_CHANNEL_PIN_ROLL);
  recvr->arm = analogRead(RECVR_CHANNEL_PIN_ARM);
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
  DEBUGLN("");

  recvr->last_debug_msg_ms = millis();
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
