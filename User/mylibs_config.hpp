#ifndef MYLIBS_CONFIG
#define MYLIBS_CONFIG
#include "myints.h"
#include "debug.h"
#include "x_base.hpp"
#include <cstdlib>
#define SSD1306_I2C_Dev S_I2C_Dev

#define XMalloc(x) malloc(x)
#define XFree(x)   free(x)
#endif
