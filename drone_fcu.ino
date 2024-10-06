#include <Arduino.h>
#include "common.h"
#include "mpu.h"
#include "esc.h"
#include "cmd.h"
#include "recvr.h"

mpu_t mpu;
esc_t esc;
recvr_t recvr;
unsigned long last_time = 0;

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
    case CMD_RECVR_DEBUG:
      if (recvr_debug_enabled(&recvr)) {
        recvr_debug_disable(&recvr);
      } else {
        recvr_debug_enable(&recvr);
      }
      break;
    case CMD_MPU_CALIBRATE:
      mpu_gyro_calibrate(&mpu);
      break;
    case CMD_MOTOR_ENABLE_ALL:
      esc_enable_motors(&esc);
      INFOLLN("Enabled all motors");
      break;
    case CMD_MOTOR_DISABLE_ALL:
      esc_disable_motors(&esc);
      INFOLLN("Disabled all motors");
      break;
    case CMD_MOTOR_ENABLE:
      esc_enable_motor(&esc, cmd.motor_idx);
      INFOL("Enabled motor ");
      INFOLN(cmd.motor_idx);
      break;
    case CMD_MOTOR_DISABLE:
      esc_disable_motor(&esc, cmd.motor_idx);
      INFOL("Disabled motor ");
      INFOLN(cmd.motor_idx);
      break;
    case CMD_MOTOR_SPEED:
      esc_set_all_motor_speed_pc(&esc, cmd.motor_speed / 100.0);
      INFOL("Set motor speed to ");
      INFO(cmd.motor_speed);
      INFOLN("%");
      break;
    case CMD_MOTOR_TEST:
      INFOLLN("Testing motors");
      esc_test_motors(&esc);
      INFOLLN("Finished testing motors");
      break;
  }
}

static inline void update_components() {
  mpu_update(&mpu);
  recvr_read(&recvr);
}

static inline void debug_components() {
  mpu_debug(&mpu);
  recvr_debug(&recvr);
}

void setup() {
  serial_setup();
  mpu_setup(&mpu);
  esc_setup(&esc);
  recvr_setup(&recvr);
  esc_enable_motors(&esc);
}

void loop() {
  // Handle user input first
  cmd_t cmd;
  cmd_parse(&cmd);
  handle_command(cmd);

  // Read values from components
  update_components();

  // Debug the information of components to serial
  debug_components();
  // Measure and print the actual interrupt frequency every second
  unsigned long current_time = millis();

  if (recvr_debug_enabled(&recvr)) {
    if (current_time - last_time >= 1000) {
      noInterrupts();
      unsigned long count = interrupt_count;
      interrupt_count = 0;
      interrupts();

      float frequency = count / ((current_time - last_time) / 1000.0);
      DEBUGL("Interrupt Frequency: ");
      DEBUG(frequency);
      DEBUGLN(" Hz");

      last_time = current_time;
    }
  } else {
    interrupt_count = 0;
  }
}
