//  PIC'N'MIX MODE MOD
// 
//  When used with Universe BIOS 4.0, will force boot into Pic'N'Mix mode.
//  Requires: Universe Bios 4, 161-1 cart, ATTiny85
//
//  Attach physical pin 4 to GND terminal on Player 1 Start microswitch
//  Attach physical pin 5 to NO terminal on Player 1 Start microswitch
//  Attach physical pin 8 to VCC 5V (from PSU)
//
//  Created by Phillip Riscombe-Burton on 22/11/2021.
//  Copyright (c) 2021 Phillip Riscombe-Burton. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// I used an Arduino Nano as the programmer:
// SETUP NANO AS PROGRAMMER
// File >> Examples >> ArduinoISP 
// Upload sketch
//
// I programmed the ATTiny85 via the ISP header on the Nano:
// ATTiny85
// Sketch >> UPLOAD USING PROGRAMMER!

const int ACTION_PIN = 0; // 0 = physical pin 5
const int HOLD_1P_MS = 10000; // 10 secs

// Neo Geo Pic&Mix mode requires 1P button to be held at boot

void setup() {

   // Hold the 1P signal at startup
    pinMode(ACTION_PIN, OUTPUT);
    digitalWrite(ACTION_PIN, LOW);
    delay(HOLD_1P_MS);

    // Stop controlling 1P button
    pinMode(ACTION_PIN, INPUT);
}

void loop() {
}
