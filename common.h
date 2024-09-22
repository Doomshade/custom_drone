/// The common headers that are included in most of the files
#ifndef _COMMON_H
#define _COMMON_H

#if !__has_include("config.h")
#error "config.h not found. Please run generate_config.bat"
#else
#include "config.h"
#endif

#include "serial.h"

#endif // _COMMON_H
