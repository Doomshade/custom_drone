#ifndef _ESC_H
#define _ESC_H

#include <Arduino.h>
#include "common.h"

#define ESC_ALL_MOTORS_DISABLED (0b0000)
#define ESC_ALL_MOTORS_ENABLED (0b1111)
#define ESC_MOTOR_COUNT (4)
#define ESC_IDLE_SPEED_PC (0.0)
#define ESC_MAX_SPEED_PC (1.0)

#define CLAMP(var, min_val, max_val) \
  do { var = min(max_val, max(min_val, (var))); } while (0)
#define CLAMP_SPEED_US(var) CLAMP(var, ESC_IDLE_SPEED_US, ESC_MAX_SPEED_US)
#define CLAMP_SPEED_PC(var) CLAMP(var, ESC_IDLE_SPEED_PC, ESC_MAX_SPEED_PC)

typedef enum {
  ESC_STATE_NOT_ARMED = 0,
  ESC_STATE_ARMING = 1,
  ESC_STATE_ARMED = 2
} esc_state_t;

typedef struct {
  esc_state_t state = ESC_STATE_NOT_ARMED;
  uint8_t motor_enable_mask = ESC_ALL_MOTORS_DISABLED;
} esc_t;

// Sets up the ESC. ESC uses Timer 1 and 2 and associated pins that produce PWM output.
void esc_setup(esc_t* esc);
// Enables all motors for arming and usage
void esc_enable_motors(esc_t* esc);
// Disables all motors for arming and usage
void esc_disable_motors(esc_t* esc);
// Enables a certain motor, index ranges <0, 3>
void esc_enable_motor(esc_t* esc, uint8_t motor_idx);
// Disables a certain motor, index ranges <0, 3>
void esc_disable_motor(esc_t* esc, uint8_t motor_idx);
// Enables or disables a motor under a certain index <0, 3> for arming and usage
void esc_flip_usage_motor(esc_t* esc, uint8_t motor_idx);
// Returns the motor state, where false = disabled, true = enabled
bool esc_motor_state(esc_t* esc, uint8_t motor_idx);
// Arms the ESC
void esc_arm(esc_t* esc);
// Disarms the ESC
void esc_disarm(esc_t* esc);
// Sets the motor acceleration under a certain index <0, 3>. The speed must be a value <ESC_IDLE_SPEED_US, ESC_MAX_SPEED_US> (microseconds).
// For further information please read the BlHeli_S manual
void esc_set_motor_speed_us(esc_t* esc, uint8_t motor_idx, uint16_t speed_us);
// Sets the acceleration for all motors. For further information see esc_set_motor_speed_us
void esc_set_all_motor_speed_us(esc_t* esc, uint16_t speed_us);
// Sets the motor acceleration % under a certain index <0, 3>. The speed must be a value 0...1.
// For further information please read the BlHeli_S manual
void esc_set_motor_speed_pc(esc_t* esc, uint8_t motor_idx, float speed_pc);
// Sets the acceleration % for all motors. For further information see esc_set_motor_speed_pc
void esc_set_all_motor_speed_pc(esc_t* esc, float speed_pc);
// Tests whether the motors work.
// WARNING: Make sure you don't have propellers on!
void esc_test_motors(esc_t* esc);

static inline float speed_us_to_speed_pc(uint16_t speed_us) {
  // 1. 700...2300
  CLAMP_SPEED_US(speed_us);
  // 2. 0...1600
  speed_us -= ESC_IDLE_SPEED_US;
  // 3. 0...1
  return ((float)speed_us) / (ESC_MAX_SPEED_US - ESC_IDLE_SPEED_US);
}

static inline uint16_t speed_pc_to_speed_us(float speed_pc) {
  // 1. 0...1
  CLAMP_SPEED_PC(speed_pc);
  // 2. 0...1600
  speed_pc *= (ESC_MAX_SPEED_US - ESC_IDLE_SPEED_US);
  // 3. 700...2300
  return (uint16_t)(speed_pc + ESC_IDLE_SPEED_US);
}


#define INVALID_MOTOR_IDX UINT8_MAX
static inline uint8_t motor_idx_to_pin(uint8_t motor_idx) {
  uint8_t pin;
  switch (motor_idx) {
    case 0: return ESC_MOTOR_PIN1;
    case 1: return ESC_MOTOR_PIN2;
    case 2: return ESC_MOTOR_PIN3;
    case 3: return ESC_MOTOR_PIN4;
    default:
      ERRORL("Invalid motor index ");
      ERRORB(motor_idx, DEC);
      ERRORLN("");
      return INVALID_MOTOR_IDX;
  }
}

static void set_motor_speed_us(uint8_t motor_idx, uint16_t speed_us) {
  const uint8_t pin = motor_idx_to_pin(motor_idx);
  if (pin == INVALID_MOTOR_IDX) return;

  CLAMP_SPEED_US(speed_us);

  DEBUGL("Setting ESC speed to ");
  DEBUG(speed_us_to_speed_pc(speed_us) * 100.0);
  DEBUG("%% (");
  DEBUG(speed_us);
  DEBUG(" us) for motor ");
  DEBUGB(motor_idx, DEC);
  DEBUG(" on pin ");
  DEBUGBLN(pin, DEC);

  // The PWM pin accepts values 0..255 which maps to the % of duty cycle
  // For example the value 200 maps to (200/255)*100 =~ 78% duty cycle =~ 78% max speed
  const long pwm_value = map(speed_us, ESC_IDLE_SPEED_US, ESC_MAX_SPEED_US, 0, 255);
  analogWrite(pin, pwm_value);
}

static inline void set_motor_speed_pc(uint8_t motor_idx, float speed_pc) {
  set_motor_speed_us(motor_idx, speed_pc_to_speed_us(speed_pc));
}

static inline void set_all_motor_speed_us(uint16_t speed_us) {
  for (uint8_t motor_idx = 0; motor_idx < ESC_MOTOR_COUNT; motor_idx++) {
    set_motor_speed_us(motor_idx, speed_us);
  }
}

static inline void set_all_motor_speed_pc(float speed_pc) {
  set_all_motor_speed_us(speed_pc_to_speed_us(speed_pc));
}

void old_shit() {
  // Configure Timer1 for high frequency PWM on pins 9 and 10
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // Set PWM mode to Fast PWM, TOP = ICR1
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);

  // No prescaler
  TCCR1B |= (1 << CS10);

  // Calculate and set TOP value for highest possible frequency
  ICR1 = F_CPU / 32000 - 1;

  // Enable PWM on pin 9 (OC1A) and 10 (OC1B)
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

  // Configure Timer2 for high frequency PWM on pins 11 and 3
  TCCR2A = 0;
  TCCR2B = 0;
  // Set PWM mode to Fast PWM
  TCCR2A |= (1 << WGM21);

  // No prescaler
  TCCR2B |= (1 << CS20);

  // Enable PWM on pin 11 (OC2A) and 3 (OC2B)
  TCCR2A |= (1 << COM2A1) | (1 << COM2B1);
}

void esc_setup(esc_t* esc) {
  pinMode((uint8_t)ESC_MOTOR_PIN1, OUTPUT);
  pinMode((uint8_t)ESC_MOTOR_PIN2, OUTPUT);
  pinMode((uint8_t)ESC_MOTOR_PIN3, OUTPUT);
  pinMode((uint8_t)ESC_MOTOR_PIN4, OUTPUT);

  noInterrupts();

  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
  TCCR1B = _BV(CS12);

  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS22);

  interrupts();
  INFOLLN("ESC set up");
}

void esc_set_motor_speed_us(esc_t* esc, uint8_t motor_idx, uint16_t speed_us) {
  if (!esc || esc->state == ESC_STATE_NOT_ARMED) return;

  set_motor_speed_us(motor_idx, speed_us);
}

void esc_set_all_motor_speed_us(esc_t* esc, uint16_t speed_us) {
  for (uint8_t motor_idx = 0; motor_idx < ESC_MOTOR_COUNT; motor_idx++) {
    esc_set_motor_speed_us(esc, motor_idx, speed_us);
  }
}

void esc_set_motor_speed_pc(esc_t* esc, uint8_t motor_idx, float speed_pc) {
  if (!esc || esc->state == ESC_STATE_NOT_ARMED) return;

  set_motor_speed_pc(motor_idx, speed_pc);
}

void esc_set_all_motor_speed_pc(esc_t* esc, float speed_pc) {
  for (uint8_t motor_idx = 0; motor_idx < ESC_MOTOR_COUNT; motor_idx++) {
    esc_set_motor_speed_pc(esc, motor_idx, speed_pc);
  }
}

void esc_enable_motors(esc_t* esc) {
  esc->motor_enable_mask = ESC_ALL_MOTORS_ENABLED;
}

void esc_disable_motors(esc_t* esc) {
  esc->motor_enable_mask = ESC_ALL_MOTORS_DISABLED;
}

void esc_enable_motor(esc_t* esc, uint8_t motor_idx) {
  if (motor_idx >= ESC_MOTOR_COUNT) return;

  esc->motor_enable_mask |= (1 << motor_idx);
}

void esc_disable_motor(esc_t* esc, uint8_t motor_idx) {
  if (motor_idx >= ESC_MOTOR_COUNT) return;

  esc_set_motor_speed_pc(esc, motor_idx, 0.0);
  esc->motor_enable_mask |= (1 << motor_idx);
}

void esc_flip_usage_motor(esc_t* esc, uint8_t motor_idx) {
  if (motor_idx >= ESC_MOTOR_COUNT) return;

  esc->motor_enable_mask ^= (1 << motor_idx);
}

void esc_arm(esc_t* esc) {
  INFOLLN("Arming ESC");
  esc->state = ESC_STATE_ARMING;

  esc_set_all_motor_speed_pc(esc, 0.00);
  delay(3000);
  
  esc_set_all_motor_speed_pc(esc, 0.05);
  delay(500);

  esc_set_all_motor_speed_pc(esc, 0.90);
  delay(1500);

  esc_set_all_motor_speed_pc(esc, 0.05);
  delay(1000);
  
  esc_set_all_motor_speed_pc(esc, 0.00);

  esc->state = ESC_STATE_ARMED;
  INFOLLN("ESC armed");
}

void esc_test_motors(esc_t* esc) {
  const uint8_t max_pc = 30;

  for (int i = 0; i < max_pc; i++) {
    esc_set_all_motor_speed_pc(esc, i / 100.0);
    delay(20);
  }

  delay(1000);

  for (int i = max_pc; i >= 0; i--) {
    esc_set_all_motor_speed_pc(esc, i / 100.0);
    delay(20);
  }

  delay(5000);
}

#endif  // _ESC_H
