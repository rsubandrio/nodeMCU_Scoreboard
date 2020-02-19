#include <Arduino.h>
#include <SPI.h>
#include <DMD2.h>
#include "../include/BigNumber.h"

#define pin_A 16
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15

#define DISPLAYS_WIDE 1
#define DISPLAYS_HIGH 1
SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk); // DMD controls the entire display

byte score1 = 0;
byte score2 = 0;
byte scored_before = 0;

void setup() {
  dmd.setBrightness(255);
  dmd.selectFont(BigNumber);
  dmd.begin();
  dmd.drawString(0,-1,F("00"));
  dmd.drawString(19, -1, F("00"));
  dmd.drawBox(14,4,17,8);
  dmd.drawBox(0, 14, 12, 15, GRAPHICS_OFF);
  dmd.drawBox(19, 14, 31, 15, GRAPHICS_OFF);
}

void loop() {
}
