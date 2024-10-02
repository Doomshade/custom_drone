#ifndef _CMD_H
#define _CMD_H

#include <Arduino.h>
#include "serial.h"

typedef enum {
  NONE = 0,
  CMD_ESC_ARM,
  CMD_MPU_DEBUG,
  CMD_MPU_CALIBRATE,
  CMD_RECVR_DEBUG,
  CMD_MOTOR_ENABLE_ALL,
  CMD_MOTOR_DISABLE_ALL,
  CMD_MOTOR_ENABLE,
  CMD_MOTOR_DISABLE,
  CMD_MOTOR_TEST,
  CMD_MOTOR_SPEED
} cmd_type_t;

typedef struct {
  cmd_type_t cmd = NONE;

  uint8_t motor_idx;
  uint8_t motor_speed;
} cmd_t;

// Processes input from Serial
void cmd_parse(cmd_t *cmd);

void cmd_parse(cmd_t *cmd) {
  if (Serial.available() <= 0) {
    cmd->cmd = NONE;
    return;
  }
  
  String input = Serial.readString();
  input.toLowerCase();
  input.trim();

  if (input.equalsIgnoreCase("esc arm")) {
    cmd->cmd = CMD_ESC_ARM;
    return;
  }

  if (input.equalsIgnoreCase("mpu dbg")) {
    cmd->cmd = CMD_MPU_DEBUG;
    return;
  }

  if (input.equalsIgnoreCase("mpu cal")) {
    cmd->cmd = CMD_MPU_CALIBRATE;
    return;
  }

    if (input.equalsIgnoreCase("recvr dbg")) {
    cmd->cmd = CMD_RECVR_DEBUG;
    return;
  }

  if (input.equals("motor e")) {
    cmd->cmd = CMD_MOTOR_ENABLE_ALL;
    return;
  }

  if (input.equals("motor t")) {
    cmd->cmd = CMD_MOTOR_TEST;
    return;
  }

  if (input.equals("motor d")) {
    cmd->cmd = CMD_MOTOR_DISABLE_ALL;
    return;
  }

  if (input.startsWith("motor e ")) {
    unsigned long motor_idx = strtoul(input.substring(strlen("motor e ")).c_str(), nullptr, 10);
    if (motor_idx <= 0 || motor_idx >= 5) {
      Serial.println("ERROR: Invalid motor index. Must be <1, 4>");
      return;
    }
    cmd->cmd = CMD_MOTOR_ENABLE;
    cmd->motor_idx = motor_idx - 1;
    return 0;
  }

  if (input.startsWith("motor d ")) {
    unsigned long motor_idx = strtoul(input.substring(strlen("motor d ")).c_str(), nullptr, 10);
    if (motor_idx <= 0 || motor_idx >= 5) {
      Serial.println("ERROR: Invalid motor index. Must be <1, 4>");
      return;
    }
    cmd->cmd = CMD_MOTOR_DISABLE;
    cmd->motor_idx = motor_idx - 1;
    return;
  }

  if (input.startsWith("motor s ")) {
    unsigned long speed = strtoul(input.substring(strlen("speed ")).c_str(), nullptr, 10);
    speed = max(0, min(speed, 100));
    cmd->cmd = CMD_MOTOR_SPEED;
    cmd->motor_speed = speed;
    return;
  }

  ERRORL("Invalid command: ");
  ERROR(input.c_str());
  ERRORLN("");
}


#endif  // _CMD_H