#include "Arduino.h"
#include "Pages.h"
#include "Comms.h"
//#include <Adafruit_SSD1306.h>
#include "PDQ_ST7735_config.h"
#include <PDQ_FastPin.h>
#include <PDQ_ST7735.h>
#include <PDQ_GFX.h>
#include "Numbers24pt7b.h"
#include "splash.h"

PDQ_ST7735 tft;			// PDQ: create LCD object (using pins in "PDQ_ST7735_config.h")

static bool setupFlagsNeeded = true;
static bool setupRpmNeeded = true;
static bool setupTopThirdNeeded = true;
static bool setupMiddleThirdNeeded = true;
static bool setupBottomThirdNeeded = true;

void initDisplay()
{
  #if defined(ST7735_RST_PIN)	// reset like Adafruit does
    Serial.println(F("Using hardware reset pin"));
    FastPin<ST7735_RST_PIN>::setOutput();
    FastPin<ST7735_RST_PIN>::hi();
    FastPin<ST7735_RST_PIN>::lo();
    delay(1);
    FastPin<ST7735_RST_PIN>::hi();
  #endif

  tft.begin();  

  tft.fillScreen(ST7735_BLACK);

  showSplash(F("ARDUGAUGE"));
  delay(1000);
}

void showSplash(Fstring message)
{
  tft.fillScreen(ST7735_BLACK);
  tft.drawBitmap((128 - splash1_width) / 2, (64 - splash1_height) / 2,
                  splash1_data, splash1_width, splash1_height, ST7735_BLUE);
  uint8_t max_char = strlen_PM(message);
  uint8_t offset = ((128 - 6 * max_char) / 2);
  tft.setCursor(offset, 56);
  tft.print(message);
}


void showRPM(int16_t rpm, int16_t min_rpm, int16_t max_rpm)
{
  // need flag to determine setup or just update
  // setup: draw overall gauge frame; draw "RPM"
  // update: clear canvas; draw bar, draw bar vlines on canvas; paste canvas; draw "1234"
  // allocate character space

  if (setupRpmNeeded) {
    // setup
    // draw overall gauge frame rectangle from (0,84) to (127,118)
    tft.drawRect(0,84, 127,34,0xFFFF);
    // draw "RPM" at 3x font origin at (73,86) (86 is from 127 - 3*18)
    tft.setCursor(74,86);
    tft.setTextSize(3);
    tft.setTextColor(0xFFFF);
    tft.print(F("RPM"));
    setupRpmNeeded = false;
  }
  // update
  // clear canvas
  // draw RPM bar with drawHbar
  int width = map(rpm,min_rpm,max_rpm,0,127);
  drawHBar(0, 110, 128, 8, 6, width);
  // draw RPM numbers:
  String s_rpm = String(rpm);
  // start drawing at 54-((rpm.Length()-1)*16)
  // draw thousands at (0,86)
  tft.setTextSize(3);
  tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
  tft.setCursor(55 - ((s_rpm.length()-1)*18),86);
  tft.print(s_rpm);
}

void show3Bar(Fstring label1, int16_t value1, int16_t min_val1, int16_t max_val1, uint8_t decimal1,
              Fstring label2, int16_t value2, int16_t min_val2, int16_t max_val2, uint8_t decimal2,
              Fstring label3, int16_t value3, int16_t min_val3, int16_t max_val3, uint8_t decimal3)
{
  drawThirdBar(label1, value1, min_val1, max_val1, decimal1, 0);
  drawThirdBar(label2, value2, min_val2, max_val2, decimal2, 1);
  drawThirdBar(label3, value3, min_val3, max_val3, decimal3, 2);
}

void drawThirdBar(Fstring label, int16_t value, int16_t min_val, int16_t max_val, uint8_t decimal, uint8_t third)
{
  // static char valString[22];
  // if (strlen_PM(label) != 0)
  // {
  //   uint8_t offsetY = third*28; //top gauge starts at (0,0) and goes to (127,27), middle starts at (0,28) and goes to (127,55), bottom starts at (0,56) and goes to (127,83)
  //   formatValue(valString, value, decimal);
  //   uint8_t offset = centering(valString, 12, 128, maxChar(min_val, max_val, decimal));
  //   uint8_t width = constrain(map(value, min_val, max_val, 0, 128), 0, 128);

  //   OLED.setCursor(0, offsetY);
  //   OLED.print(label);
  //   OLED.setTextSize(2);
  //   OLED.setCursor(offset, 9 + offsetY);
  //   OLED.print(valString);
  //   OLED.setTextSize(1);
  //   drawHBar(0, 23 + offsetY, 128, 8, 4, width);
  // }
}

void showFlags(Fstring label1, bool value1,
               Fstring label2, bool value2,
               Fstring label3, bool value3,
               Fstring label4, bool value4,
               Fstring label5, bool value5,
               Fstring label6, bool value6)
{
  //OLED.clearDisplay();
  // flag boxes are
  // (0,122) to (62,133), (64,122) to (127,133)
  // (0,135) to (62,146), (64,135) to (127,146)
  // (0,148) to (62,159), (64,148) to (127,159)
  drawFlag(label1, value1, 1, 122);
  drawFlag(label2, value2, 65, 122);
  drawFlag(label3, value3, 1, 135);
  drawFlag(label4, value4, 65, 135);
  drawFlag(label5, value5, 1, 148);
  drawFlag(label6, value6, 65, 148);

  //OLED.display();
}

void drawFlag(Fstring label, bool value, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  if (strlen_PM(label) != 0)
  {
    uint8_t max_char = strlen_PM(label);
    uint8_t x_offset = ((w - 6 * max_char) / 2);
    uint8_t y_offset = (h - 8) / 2;

    if (value) //flag is tripped
    {
      //tft.fillRect(x, y, w, h, ST7735_WHITE);
      tft.setTextColor(ST7735_BLACK,ST7735_WHITE);
    }
    else
    {
      tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
   }
   tft.setTextSize(1);
    tft.setCursor(x + x_offset, y + y_offset);
    tft.print(label);
    tft.drawRect(x, y, w, h, ST7735_WHITE);
  }
}

void printFPS(uint32_t fps) {
  tft.setCursor(0,0);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.print(fps);
}

// uint8_t maxChar(int32_t min_val, int32_t max_val, uint8_t decimal)
// {
//   static char buf[STRING_LENGTH];
//   uint8_t max_char = formatValue(buf, min_val, decimal);
//   max_char = max(formatValue(buf, max_val, decimal), max_char);
//   return max_char;
// }

// uint8_t formatValue(char *buf, int32_t value, uint8_t decimal)
// {
//   // static char temp[STRING_LENGTH];
//   // clearBuffer(temp);

//   clearBuffer(buf);
//   snprintf(buf, 22, "%d", value);
//   uint8_t len = strlen(buf);

//   if (decimal != 0)
//   {
//     uint8_t target = decimal + 1;
//     uint8_t numLen = len - ((value < 0) ? 1 : 0);
//     while (numLen < target)
//     {
//       for (uint8_t i = 0; i < numLen + 1; i++)
//       // if negative, skip negative sign
//       {
//         buf[len - i + 1] = buf[len - i];
//         buf[len - i] = '0';
//       }
//       buf[len + 1] = '\0';
//       numLen++;
//       len++;
//     }
//     // insert
//     for (uint8_t i = 0; i < decimal + 1; i++)
//     {
//       buf[len - i + 1] = buf[len - i];
//       buf[len - i] = '.';
//     }
//     // clearBuffer(buf);
//     // snprintf(buf, STRING_LENGTH, "%d", target);
//   }
//   return strlen(buf);
// }

// uint8_t centering(char *buf, uint8_t c_width, uint8_t field_width, uint8_t max_char)
// {
//   if (max_char == 0)
//   {
//     max_char = strlen(buf);
//   }
//   return ((field_width - c_width * max_char) / 2 + (max_char - strlen(buf)) * c_width);
// }

void drawHBar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t bar_h, uint8_t bar_w)
{
  uint8_t q0 = x; // start of first quarter, at x
  uint8_t q1 = x + (w - 1) >> 2; // start of second quarter, at (x+w)/4
  uint8_t q2 = x + (w - 1) >> 1; // start of third quarter, at (x+w)/2
  uint8_t q3 = x + 3 * ((w - 1) >> 2); // start of fourth quarter
  uint8_t q4 = x + w - 1; // end of fourth quarter
  uint8_t bar_y = (h - bar_h - 1) / 2 + y; // y coordinate for the bar, at bottom of gauge, half the size of bar_h

  tft.drawFastVLine(q0, y, h, ST7735_WHITE); // draw left frame
  tft.drawFastVLine(q1, y + h - 5, 4, ST7735_WHITE); // draw q1 tick
  tft.drawFastVLine(q2, y + h - 5, 4, ST7735_WHITE); // draw q2 tick
  tft.drawFastVLine(q2 + 1, y + h - 5, 4, ST7735_WHITE); // draw q2 tick
  tft.drawFastVLine(q3, y + h - 5, 4, ST7735_WHITE); // draw q3 tick
  tft.drawFastVLine(q4, y, h, ST7735_RED); // draw right frame
  tft.drawFastHLine(x, y + h - 1, w, ST7735_WHITE); // draw bar bottom
  tft.fillRect(x, bar_y, bar_w, bar_h, ST7735_WHITE); // draw bar
  tft.drawFastVLine(q0 + 1, bar_y, bar_h, ST7735_BLACK);
  tft.drawFastVLine(q4 - 1, bar_y, bar_h, ST7735_WHITE);
}

// void clearBuffer(char *buf, uint8_t bufLen)
// {
//   for (uint8_t i = 0; i < bufLen; i++)
//   {
//     buf[i] = '\0';
//   }
// }