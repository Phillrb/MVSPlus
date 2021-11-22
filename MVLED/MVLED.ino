//  MVLED
//  Arduino replacement for both MV-LED (credits) and MV-ELA (lamp driver) MVS arcade hardware.
//  These specialist pieces of hardware are becoming increaingly hard to find and more expensive with their increasing rarity.
//  MVSPlus addresses this issue by utilising a relatively cheap Arduino Mega 2560
//
//  Created by Phillip Riscombe-Burton on 08/06/2014.
//  Copyright (c) 2014 Phillip Riscombe-Burton. All rights reserved.
//
//	Permission is hereby granted, free of charge, to any person obtaining a
//	copy of this software and associated documentation files (the
//	"Software"), to deal in the Software without restriction, including
//	without limitation the rights to use, copy, modify, merge, publish,
//	distribute, sublicense, and/or sell copies of the Software, and to
//	permit persons to whom the Software is furnished to do so, subject to
//	the following conditions:
//
//	The above copyright notice and this permission notice shall be included
//	in all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*
* For MV-LED: 
* - up to 2 pairs of seven-segment displays can be used (commented out by default)
* - an LCD display can be used (default option)
* For MV-ELA:
* - 4 LEDs or bulbs can be used (commented out by default)
* - an LCD display can be used (default option)
*/



//If using 7-segment display pairs then:
//#include "MVLED.h" // for 7-segment display replacement

//LCD display
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);

//
// Settings constants
//
const int pinsPerCreditReading = 8;
const int pinsPerGamePosReading = 4; //may need to change for 6 slot
const int maxNumberOfSlots = 4; //adjust for number of slots

//Game position pin states
const int gamePos1[pinsPerGamePosReading] = {0,0,0,0}; //1
const int gamePos2[pinsPerGamePosReading] = {1,0,0,0}; //2
const int gamePos3[pinsPerGamePosReading] = {0,0,1,0}; //3
const int gamePos4[pinsPerGamePosReading] = {1,0,1,0}; //4
//const int gamePos5[pinsPerGamePosReading] = {????}; //5
//const int gamePos6[pinsPerGamePosReading] = {????}; //6


//
//Input Pins
//

//Credits
//P1 credit clock pin, P1 digit 1  (odd pins 37 to 31),  P1 digit 2 (odd pins 29 to 23)
const int pinP1Array[pinsPerCreditReading + 1] = {39,37,35,33,31,29,27,25,23}; 

//Game number data; clk pin is 53 and data is odd pins 51 to 45 
const int pinGameReadArray[pinsPerGamePosReading + 1] = {53,51,49,47,45};

//
//Output pins
//

//Game number pins //For LEDS representing each game position
//const int pinGameWriteArray[maxNumberOfSlots] = {42,44,46,48}; //adjust for number of slots - for LEDs

//
//Variables
//

//Credits
int lastCreditReading = -1;

//Game number
int lastGameReading = -1;





void setup() 
{ 
  //Allow serial logs
  Serial.begin(9600);
  
  //INPUT PINS
  
  //Setup Player 1 credit pins
  for (int i = 0; i <= pinsPerCreditReading; i++)
  {
    pinMode(pinP1Array[i], INPUT_PULLUP );
  }
  
  //Setup Read Game pins for input
  for (int i = 0; i <= pinsPerGamePosReading; i++)
  {
    pinMode(pinGameReadArray[i], INPUT_PULLUP );
  }
  
 
  //OUTPUT PINS
  
//  //Setup pins for game number illumination //FOR LEDs
//  for (int i = 0; i < maxNumberOfSlots; i++)
//  {
//    pinMode(pinGameWriteArray[i], OUTPUT);
//  }
  
  //Setup 7-segment displays
//  setupMVLEDPins(); //for 7-segment display replacement
 
   //LCD output for both Credits and Game position
   // set up the LCD's number of columns and rows: 
    lcd.begin(16,2);
    updateDisplay();
}

void loop()
{
    //Check player 1 credit
    if(didCreditChange())
    {
      //ALERT CREDIT CHANGE
      Serial.print("Credits: ");
      Serial.println(lastCreditReading);
      
      //LCD
      updateDisplay();
    }
    
    //*** MV-LED+ *** //for 7-segment displays - needs to update on every loop!
//      MVLEDS(lastCreditReading, 0);

    //Check game number
    if(didGameChange())
    {
      //ALERT GAME CHANGE
      Serial.print("GAME SELECTED: POSITION ");
      Serial.println(lastGameReading);
      
      //Illuminate new game number
//      updateGameDisplay(lastGameReading); //FOR LEDs

      //LCD
      updateDisplay();
    }
}





//Game reading
boolean didGameChange(void)
{
 int gameReading = -1;
 
   //Read game pos from pins
    gameReading = readGamePos(lastGameReading); 
    
    //Changed?
    if(gameReading != lastGameReading)
    {
       lastGameReading = gameReading;
       return true;
    }
  
  return false;
}

int readGamePos(int lastReading)
{
  //Check clock pin
  if(digitalRead(pinGameReadArray[0]) == HIGH)
  {
    int pos = lastReading;
    int tempPos = 0;
    
    //Read all data pins only once
    int pinReading[pinsPerGamePosReading] = {};
    for (int i = 1; i <= pinsPerGamePosReading; i++)
    {
      pinReading[i-1] = digitalRead(pinGameReadArray[i]);
    }
    
    //Compare the reading to existing pos data
    if(memcmp(pinReading, gamePos1, sizeof(pinReading)) == 0) tempPos = 1;
    else if(memcmp(pinReading, gamePos2, sizeof(pinReading)) == 0) tempPos = 2;
    else if(memcmp(pinReading, gamePos3, sizeof(pinReading)) == 0) tempPos = 3;
    else if(memcmp(pinReading, gamePos4, sizeof(pinReading)) == 0) tempPos = 4;
//    else if(memcmp(pinReading, gamePos5, sizeof(pinReading)) == 0) tempPos = 5;
//    else if(memcmp(pinReading, gamePos6, sizeof(pinReading)) == 0) tempPos = 6;
     
     if( tempPos > 0 && lastReading != tempPos)
     {
        lastReading = tempPos; 
     }
  } 
  
  return lastReading;
}



//Credit reading
boolean didCreditChange(void)
{
  int creditReading = -1;
  
 //Read P1 credit as decimal
  creditReading = readCreditFromPins(lastCreditReading); 
  
  //Changed?
  if(creditReading != lastCreditReading)
  {
     lastCreditReading = creditReading;
     return true;
  }
  
  return false;
}

int readCreditFromPins(int lastCreditReading)
{
   //Check clock pin
  if(digitalRead(pinP1Array[0]) == HIGH)
  {
      //Create values to hold digit data
      int r1=0, r2=0, decimalReading = 0;
  
      //Convert the binary data to individual decimal digits
      //Simple binary conversion method - keep a running subtotal
      //the '2' represents base2 - regardless, this method always produces
      //a base10 (decimal) result
      //reads digit 1 into r1 and digit2 into r2
     
      for(int i = pinsPerCreditReading / 2; i > 0; i--)
      { 
          r2 = r2 * 2 + digitalRead(pinP1Array[i]);
          r1 = r1 * 2 + digitalRead(pinP1Array[i + (pinsPerCreditReading / 2)]);
      }
     
     //Convert both digits to single decimal reading
     decimalReading = (r1 * 10) + r2;
     
    //Update new reading if it is actually new!
    if(lastCreditReading != decimalReading && decimalReading <= 99 && decimalReading >= 0)
    {
      lastCreditReading = decimalReading;
    }
  } 
  
  return lastCreditReading;
}

//Utility method to refresh the LCD with latest readings
void updateDisplay(){
  
    lcd.clear();
    
    //Credits
    lcd.setCursor(0, 0);
    lcd.print("Credits: "); 
    if(lastCreditReading >= 0 && lastCreditReading < 10) lcd.print("0");    
    lcd.print(lastCreditReading);
    
    //Game position
    lcd.setCursor(0, 1);
    lcd.print("Game: "); 
    if(lastGameReading >=0) lcd.print(lastGameReading);
  
}

////Game display - for LEDs
//void updateGameDisplay(int lastGameReading)
//{
//  int activeSlot = lastGameReading - 1; //zero based
//  
//  //Refresh illumination pins
//  for(int i = 0; i < maxNumberOfSlots; i++)
//  {
//      digitalWrite(pinGameWriteArray[i], i == activeSlot);
//  }
//}
