#include <Arduino.h>
#include "common.h"
#include "mpu.h"
#include "esc.h"
#include "cmd.h"
#include "recvr.h"
#include "fcu.h"

static mpu_t mpu;
static esc_t esc;
static recvr_t recvr;
static fcu_t fcu;

static inline void handle_command(cmd_t cmd) {
  switch (cmd.cmd) {
    case CMD_ESC_ARM:
      esc_arm(&esc);
      break;
    case CMD_MPU_DEBUG:
      if (mpu_debug_enabled(&mpu)) {
        mpu_debug_disable(&mpu);
      } else {
        mpu_debug_enable(&mpu);
      }
      break;
    case CMD_MPU_CALIBRATE:
      mpu_gyro_calibrate(&mpu);
      break;
    case CMD_MOTOR_FLIP_ALL:
      ERRORLLN("Command not yet supported");
      break;
    case CMD_MOTOR_ENABLE_ALL:
      esc_enable_motors(&esc);
      break;
    case CMD_MOTOR_DISABLE_ALL:
      esc_disable_motors(&esc);
      break;
    case CMD_MOTOR_ENABLE:
      esc_enable_motor(&esc, cmd.motor_idx);
      break;
    case CMD_MOTOR_DISABLE:
      esc_disable_motor(&esc, cmd.motor_idx);
      break;
    case CMD_MOTOR_SPEED:
      esc_set_all_motor_speed_pc(&esc, cmd.motor_speed / 100.0);
      break;
  }
}

void setup() {
  serial_setup();
  mpu_setup(&mpu);
  esc_setup(&esc);
  recvr_setup(&recvr);
  fcu_setup(&fcu, &mpu, &esc, &recvr);
}

void loop() {
  // Handle user input first
  cmd_t cmd;
  cmd_parse(&cmd);
  handle_command(cmd);

  // Have the FCU work its ass
  fcu_work_your_ass(&fcu);
}
