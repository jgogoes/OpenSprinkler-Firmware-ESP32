#pragma once

#include <stdint.h>

#if defined(ESP8266)
typedef unsigned long time_os_t;
#else
#include <time.h>
typedef time_t time_os_t;
#endif
