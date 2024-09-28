/// The common headers that are included in most of the files
#ifndef _COMMON_H
#define _COMMON_H

#include <Arduino.h>

#if !__has_include("config.h")
#error "config.h not found. Please run generate_config.bat"
#else
#include "config.h"
#endif
#include "serial.h"

#define HALT_PROGRAM() do{while(1) delay(30);} while(0)
#define HALT_PROGRAM_MSG(msg) FATALLLN(msg); HALT_PROGRAM()

#endif // _COMMON_H
