#ifndef _FCU_H
#define _FCU_H

#include "common.h"

typedef struct {
  mpu_t *mpu;
  esc_t *esc;
  recvr_t *recvr;
} fcu_t;

void fcu_setup(fcu_t *fcu, mpu_t *mpu, esc_t *esc, recvr_t *recvr);
void fcu_work_your_ass(fcu_t *fcu);

void fcu_setup(fcu_t *fcu, mpu_t *mpu, esc_t *esc, recvr_t *recvr) {
  INFOLLN("Initializing FCU");
  if (!fcu) HALT_PROGRAM_MSG("Failed to initialize FCU.");
  if (!mpu) HALT_PROGRAM_MSG("Failed to initialize FCU - MPU not found");
  if (!esc) HALT_PROGRAM_MSG("Failed to initialize FCU - ESC not found");
  if (!recvr) HALT_PROGRAM_MSG("Failed to initialize FCU - RECVR not found");

  fcu->mpu = mpu;
  fcu->esc = esc;
  fcu->recvr = recvr;
  esc_enable_motors(esc);
}

void fcu_work_your_ass(fcu_t *fcu) {
  // Do a bunch of readings
  recvr_read(fcu->recvr);
  mpu_update(fcu->mpu);
}


#endif  // _FCU_H