#ifndef PLOT_HPP
#define PLOT_HPP
#include <stdint.h>
#include "ssd1306.hpp"

void update_plot(SSD1306 &oled, S_I2C_Dev &dev);
#endif
