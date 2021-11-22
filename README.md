# MVS Plus

A collection of projects for the NEO-GEO MVS arcade machine.

## MVLED

Arduino replacement for both MV-LED (credits) and MV-ELA (lamp driver) Neo Geo MVS arcade hardware

These specialist pieces of hardware are becoming increaingly hard to find and more expensive with their increasing rarity.
MVSPlus addresses this issue by utilising a relatively cheap Arduino Mega 2560

By default the sketch utilisies a SainSmart LCD to display the credits and current game position but, with some un-commenting, it supports 7-Segment display output (via MVLED.h) and LEDs / bulbs for game position.
Use of a relay to drive lamps is recommended.

### Arduino Setup

Input

- Game position - connect EL pins 2 -> 6 (asc) from MVS to odd pins 53 -> 45 (desc) on arduino
- Credits - connect 1P pins 3 -> 12 (asc) from MVS to odd pins 23 -> 39 (asc) on arduino

Output

- For Sainsmart LCD, plug in shield and initialise with the following pins:
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);
- For seven-segment displays include MVLED.h, uncomment respective code and use a 74HC595 shift register per 7-seg. For the first digit shift register, connect the latch, data and clock pins to arduino pins 2,3 and 4 respectively. The second digit's register should connect with 5,6 and 7.
- For LED / bulb output uncomment respective code and connect bulb pins to arduino pins 42, 44, 46 and 48.

## ATTiny85 Pic'n'Mix Mod

Use an ATTiny85 to always boot the MVS into Pic'n'Mix mode.
ATTiny85 holds down P1 Start for 10 secs at boot-time. 

Requires

- Universe Bios 4.0
- 161-1 cart
- ATTiny85

Setup

- Attach physical pin 4 to GND terminal on Player 1 Start microswitch
- Attach physical pin 5 to NO terminal on Player 1 Start microswitch
- Attach physical pin 8 to VCC 5V (from PSU)


