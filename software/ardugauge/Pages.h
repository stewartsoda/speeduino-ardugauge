#ifndef PAGES_H
#define PAGES_H

#define STRING_LENGTH 22

#define strlen_PM(x) strlen_P(reinterpret_cast<const char *>(x))
#define Fstring const __FlashStringHelper *

#include "Comms.h"

void initDisplay();
void showSplash(Fstring message);

void showRPM(int16_t rpm, int16_t min_rpm, int16_t max_rpm);
void show3Bar(Fstring label1, int16_t value1, int16_t min_val1, int16_t max_val1, uint8_t decimal1,
              Fstring label2, int16_t value2, int16_t min_val2, int16_t max_val2, uint8_t decimal2,
              Fstring label3, int16_t value3, int16_t min_val3, int16_t max_val3, uint8_t decimal3);
void drawThirdBar(Fstring label, int16_t value, int16_t min_val, int16_t max_val, uint8_t decimal, uint8_t third);
// void show4Numeric(Fstring label1, int16_t value1, int16_t min_val1, int16_t max_val1, uint8_t decimal1,
//                   Fstring label2, int16_t value2, int16_t min_val2, int16_t max_val2, uint8_t decimal2,
//                   Fstring label3, int16_t value3, int16_t min_val3, int16_t max_val3, uint8_t decimal3,
//                   Fstring label4, int16_t value4, int16_t min_val4, int16_t max_val4, uint8_t decimal4);
// void drawQuarter(Fstring label, int16_t value, int16_t min_val, int16_t max_val, uint8_t decimal, uint8_t quad);
void showFlags(Fstring label1, bool value1,
               Fstring label2, bool value2,
               Fstring label3, bool value3,
               Fstring label4, bool value4,
               Fstring label5, bool value5,
               Fstring label6, bool value6);

void drawFlag(Fstring label, bool value, uint8_t x, uint8_t y, uint8_t w = 62, uint8_t h = 12);

// uint8_t maxChar(int32_t min_val, int32_t max_val, uint8_t decimal);
// uint8_t formatValue(char *buf, int32_t value, uint8_t decimal);
// uint8_t centering(char *buf, uint8_t c_width, uint8_t field_width = 128, uint8_t max_char = 0);
void drawHBar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bar_h, uint8_t bar_w);
// void clearBuffer(char *buf, uint8_t bufLen = STRING_LENGTH);
void printFPS(uint32_t fps);

#endif //PAGES_H