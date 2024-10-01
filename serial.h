/// Helper serial header
/// The logging levels are the following:
/// - DEBUG
/// - INFO
/// - WARNING
/// - FATAL
///
/// The naming convention is as follows:
/// DEBUG - prints the message to serial if the log level is >= DEBUG
/// DEBUGL - prints the message including the *L*evel prefix if the log level is >= DEBUG
/// DEBUGLN - prints the message to serial with new *L*i*N*e if the log level is >= DEBUG
/// DEBUGB - prints the message to serial with the given base if the log level is >= DEBUG
/// DEBUGBLN - prints the message to serial with new *L*i*N*e with the given *B*ase if the log level is >= DEBUG
/// DEBUGLLN - prints the message including the *L*evel prefix to serial with new *L*i*N*e if the log level is >= DEBUG
/// DEBUGLBLN - prints the message including the *L*evel prefix with the given *B*ase to serial with new *L*i*N*e if the log level is >= DEBUG
///
/// Swap out DEBUG for INFO, WARN, ERROR, or FATAL
#ifndef _SERIAL_H
#define _SERIAL_H

#include <Arduino.h>

#define LEVEL_DEBUG "DBG: "
#define LEVEL_INFO "INF: "
#define LEVEL_WARN "WAR: "
#define LEVEL_ERROR "ERR: "
#define LEVEL_FATAL "FTL: "

#define STMT(stmt) \
  do { stmt; } while (0)
#define EMPTY_STMT \
  do { ; } while (0)
#define LN STMT(Serial.println())

#if LOG_LEVEL != LOG_LEVEL_NONE
#define LOG(msg) STMT(Serial.print(msg))
#define LOGL(level, msg) \
  LOG(level); \
  LOG(msg)
#define LOGLN(msg) \
  LOG(msg); \
  LN
#define LOGB(msg, base) STMT(Serial.print(msg, base))
#define LOGBLN(msg, base) \
  LOGB(msg, base); \
  LN
#define LOGLB(level, msg, base) \
  LOG(level); \
  LOGB(msg, base)
#define LOGLLN(level, msg) \
  LOGL(level, msg); \
  LN
#define LOGLBLN(msg, base) \
  LOGLB(msg, base); \
  LN
#else
#define LOG(msg) EMPTY_STMT
#define LOGL(level, msg) EMPTY_STMT
#define LOGLN(msg) EMPTY_STMT
#define LOGB(msg, base) EMPTY_STMT
#define LOGLB(level, msg, base) EMPTY_STMT
#define LOGBLN(msg, base) EMPTY_STMT
#define LOGLLN(msg) EMPTY_STMT
#define LOGLBLN(msg, base) EMPTY_STMT
#endif


#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define DEBUG(msg) LOG(msg)
#define DEBUGB(msg, base) LOGB(msg, base)
#define DEBUGL(msg) LOGL(LEVEL_DEBUG, msg)
#define DEBUGBLN(msg, base) LOGBLN(msg, base)
#define DEBUGLN(msg) LOGLN(msg)
#define DEBUGLLN(msg) LOGLLN(LEVEL_DEBUG, msg)
#else
#define DEBUG(msg) EMPTY_STMT
#define DEBUGL(msg) EMPTY_STMT
#define DEBUGBLN(msg, base) EMPTY_STMT
#define DEBUGLN(msg) EMPTY_STMT
#define DEBUGLLN(msg) EMPTY_STMT
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define INFO(msg) LOG(msg)
#define INFOB(msg, base) LOGB(msg, base)
#define INFOL(msg) LOGL(LEVEL_INFO, msg)
#define INFOBLN(msg, base) LOGBLN(LEVEL_INFO, msg, base)
#define INFOLN(msg) LOGLN(msg)
#define INFOLLN(msg) LOGLLN(LEVEL_INFO, msg)
#else
#define INFO(msg) EMPTY_STMT
#define INFOB(msg, base) EMPTY_STMT
#define INFOL(msg) EMPTY_STMT
#define INFOLN(msg) EMPTY_STMT
#define INFOLLN(msg) EMPTY_STMT
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define WARN(msg) LOG(msg)
#define WARNL(msg) LOGL(LEVEL_WARN, msg)
#define WARNLN(msg) LOGLN(msg)
#define WARNLLN(msg) LOGLLN(LEVEL_WARN, msg)
#else
#define WARN(msg) EMPTY_STMT
#define WARNL(msg) EMPTY_STMT
#define WARNLN(msg) EMPTY_STMT
#define WARNLLN(msg) EMPTY_STMT
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define ERROR(msg) LOG(msg)
#define ERRORB(msg, base) LOGB(msg, base)
#define ERRORL(msg) LOGL(LEVEL_ERROR, msg)
#define ERRORLN(msg) LOGLN(msg)
#define ERRORLLN(msg) LOGLLN(LEVEL_ERROR, msg)
#else
#define ERROR(msg) EMPTY_STMT
#define ERRORB(msg, base) EMPTY_STMT
#define ERRORL(msg) EMPTY_STMT
#define ERRORLN(msg) EMPTY_STMT
#define ERRORLLN(msg) EMPTY_STMT
#endif

#if LOG_LEVEL >= LOG_LEVEL_FATAL
#define FATAL(msg) LOG(msg)
#define FATALL(msg) LOGL(LEVEL_FATAL, msg)
#define FATALLN(msg) LOGLN(msg)
#define FATALLLN(msg) LOGLLN(LEVEL_FATAL, msg)
#else
#define FATAL(msg) EMPTY_STMT
#define FATALL(msg) EMPTY_STMT
#define FATALLN(msg) EMPTY_STMT
#define FATALLLN(msg) EMPTY_STMT
#endif


void serial_setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  INFOLLN("Serial set up");
}


#endif  // _SERIAL_H
