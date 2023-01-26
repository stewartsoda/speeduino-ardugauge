
//#include <gfxfont.h>

#include <EEPROM.h>
#include "Arduino.h"
#include "Pages.h"
#include "Comms.h"
#define N_PAGES 10
#define BUTTON_HOLD_TIME 500

uint8_t pageNum = EEPROM.read(0);

void setup()
{
  Serial.begin(115200);
  initDisplay();
  // if (pageNum >= N_PAGES)
  // {
  //   pageNum = 0;
  //   EEPROM.update(0, pageNum);
  // }
}

void loop()
{
  // button and page number operations
  // static bool buttonLast = false;
  // static bool buttonHeld = false;
  // static uint32_t timePressed = 0;

  // bool buttonNow = !digitalRead(2);
  // digitalWrite(LED_BUILTIN, buttonNow);

  // // button pressed
  // if (!buttonLast & buttonNow)
  // {
  //   timePressed = millis();
  // }
  // // button held
  // if (buttonNow & !buttonHeld & ((millis() - timePressed) > BUTTON_HOLD_TIME))
  // {
  //   pageNum = EEPROM.read(0);
  //   pageNum--;
  //   if (pageNum >= N_PAGES)
  //     pageNum = N_PAGES - 1;
  //   EEPROM.update(0, pageNum);
  //   buttonHeld = true; // prevent rerunning button held action.
  // }
  // // button released
  // if (buttonLast & !buttonNow)
  // {
  //   if (!buttonHeld) // do not perform if button hold action executed.
  //   {
  //   pageNum = EEPROM.read(0);
  //   pageNum++;
  //   if (pageNum >= N_PAGES)
  //     pageNum = 0;
  //   EEPROM.update(0, pageNum);
  //   }
  //   buttonHeld = false;
  // }
  // buttonLast = buttonNow;

  // // serial operation, frequency based request
  // static uint32_t lastUpdate = millis();
  // if (millis() - lastUpdate > 10)
  // {
  //   requestData(50);
  //   lastUpdate = millis();
  // }
  uint16_t curr_rpm = 0;
  while (curr_rpm++ < 6000) {
    // get refresh rate
    static uint32_t lastRefresh = millis();

    // display pages
    showRPM(curr_rpm,0,6000);
    showFlags(F("crank"), random(0,2),
            F("RUN"), random(0,2),
            F("sync"), random(0,2),
            F("warm"), random(0,2),
            F("NOS"), random(0,2),
            F("Awesome!"), random(0,2));

    static uint16_t refreshRate = 1000 / (millis() - lastRefresh);
    printFPS(refreshRate);
    lastRefresh = millis();

  }
}