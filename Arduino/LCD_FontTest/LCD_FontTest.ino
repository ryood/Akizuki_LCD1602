/*
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

   RotaryEncoder A 9
   RotaryEncoder B 8
   RotaryEncoder C GND

   TactSW 10
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int clearBtn = 10;
const int RE_A = 9;
const int RE_B = 8;

uint8_t charCode = 0x00;

// Rotary Encoderの読み取り akizuki/Alps
int readRE()
{
  static uint8_t index;
  int retVal = 0;
  index = (index << 2) | (digitalRead(RE_B) << 1) | (digitalRead(RE_A));
  index &= 0b1111;
  switch (index) {
  // 時計回り
  case 0b0111:  // 01 -> 11
    retVal = 1;
    break;
  // 反時計回り
  case 0b1101:  // 11 -> 01
    retVal = -1;
    break;
  }
  delay(1);  // (とりあえず)チャタリング防止
  return retVal;
}

void setup() {
  pinMode(clearBtn, INPUT_PULLUP);
  pinMode(RE_A, INPUT_PULLUP);
  pinMode(RE_B, INPUT_PULLUP);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("LCD Char Font");

  delay(1000);
  lcd.clear();
}

void loop() {
  if (digitalRead(clearBtn) == 0) {
    charCode = 0;
  }
  charCode += readRE() * 16;

  lcd.home();
  lcd.print(charCode, HEX);
  lcd.print(":");
  lcd.print(charCode + 15, HEX);
  lcd.print("      ");
  
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.write(charCode + i);
  }
}

