//   MVLED.h
//   MVLED.h is the complete replacement of up to 2 MV-LEDs
//   Requires up to 12 output pins and x input pins
//   Output pins (in groups of 3) connect to 74HC595 shift registers - these each drive a 7-seg display
//   Multiplexing is used to illuminate a single segment at a time.
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
   OUTPUT PINS: 
   
   MV-LED1
   - pins 2,3,4 -> shift register sr1a -> seven-segment display seg1a
   - pins 5,6,7 -> shift register sr1b -> seven-segment display seg1b
   
   MV-LED2
   - pins 8,9,10 -> shift register sr2a -> seven-segment display seg2a
   - pins 11,12,13 -> shift register sr2b -> seven-segment display seg2b
   
*/

/* File includes code taken from:
    7-Segment LED counter, multiplexing using 74HC595 8-bit shift
    register. Full details of this project: http://bit.ly/sOft87

    Created 6 Nov 2011
    by Mark Sweeting - www.sweeting.org/mark
 */

#include <Arduino.h>

//MV-LED1
  //sr1a (for 1st digit)
  const int sr1aLatchPin = 2;  //Pin connected to Pin 12 of seg1a (Latch)
  const int sr1aDataPin  = 3;  //Pin connected to Pin 14 of seg1a (Data)
  const int sr1aClockPin = 4;  //Pin connected to Pin 11 of seg1a (Clock)
  //sr1b (for 2nd digit)
  const int sr1bLatchPin = 5;  //Pin connected to Pin 12 of seg1b (Latch)
  const int sr1bDataPin  = 6;  //Pin connected to Pin 14 of seg1b (Data)
  const int sr1bClockPin = 7;  //Pin connected to Pin 11 of seg1b (Clock)
    
  //MV-LED2
  //sr2a (for 1st digit)
  const int sr2aLatchPin = 8;  //Pin connected to Pin 12 of seg2a (Latch)
  const int sr2aDataPin  = 9;  //Pin connected to Pin 14 of seg2a (Data)
  const int sr2aClockPin = 10;  //Pin connected to Pin 11 of seg2a (Clock)
  //sr2b (for 2nd digit)
  const int sr2bLatchPin = 11;  //Pin connected to Pin 12 of seg2b (Latch)
  const int sr2bDataPin  = 12;  //Pin connected to Pin 14 of seg2b (Data)
  const int sr2bClockPin = 13;  //Pin connected to Pin 11 of seg2b (Clock)

  //7-segs can display HEX - so limit to numeric digits only
  const int topNumber = 9; //Increase to 15 for letters A - F in Hex

  //common Annode
  const bool isCommonAnnodeSeg1 = true;
  const bool isCommonAnnodeSeg2 = false;
  
  //Variables
  int numberOfCreditsMVLED1 = 0;
  int numberOfCreditsMVLED2 = 0;
  
// Describe each digit in terms of display segments
const byte numbers[10] = {
                    0b11111100, //0
                    0b01100000, //1
                    0b11011010, //2
                    0b11110010, //3
                    0b01100110, //4
                    0b10110110, //5
                    0b10111110, //6
                    0b11100000, //7
                    0b11111110, //8
                    0b11100110  //9
};

void setupMVLEDPins()
{
  //*** MV-LED+ ***
  //MV-LED1
  pinMode(sr1aLatchPin, OUTPUT);
  pinMode(sr1aClockPin, OUTPUT);
  pinMode(sr1aDataPin,  OUTPUT);
  pinMode(sr1bLatchPin, OUTPUT);
  pinMode(sr1bClockPin, OUTPUT);
  pinMode(sr1bDataPin,  OUTPUT);
  
  //MV-LED2
  pinMode(sr2aLatchPin, OUTPUT);
  pinMode(sr2aClockPin, OUTPUT);
  pinMode(sr2aDataPin,  OUTPUT);
  pinMode(sr2bLatchPin, OUTPUT);
  pinMode(sr2bClockPin, OUTPUT);
  pinMode(sr2bDataPin,  OUTPUT);
}


//Send a byte of data to a shift register
void shiftIt (int segPair, bool isLeftSeg, byte data)
{
  
  int latchPin, clockPin, dataPin; 
  bool isCommonAnnode;
  
  //Set the correct pins to communicate with this shift register
  if(segPair == 1)
  {  
    //set if common annode or cathode
    isCommonAnnode = isCommonAnnodeSeg1;
    
    if(isLeftSeg) 
    { 
      //1st pair - left display
      latchPin = sr1aLatchPin;
      clockPin = sr1aClockPin;
      dataPin = sr1aDataPin;
    }
    else
    {
      //1st pair - right display
      latchPin = sr1bLatchPin;
      clockPin = sr1bClockPin;
      dataPin = sr1bDataPin;
    }
  }
  else
  {
    //set if common annode or cathode
    isCommonAnnode = isCommonAnnodeSeg2;
    
    if(isLeftSeg) 
    { 
      //2nd pair - left display
      latchPin = sr2aLatchPin;
      clockPin = sr2aClockPin;
      dataPin = sr2aDataPin;
    }
    else
    {
      //2nd pair - right display
      latchPin = sr2bLatchPin;
      clockPin = sr2bClockPin;
      dataPin = sr2bDataPin;
    }
  }
    
    // Set latchPin LOW and clock these 8 bits into the register
    // to illuminate a single segment.
    digitalWrite(latchPin, LOW);
    
    //Set each bit in the byte of data
    for (int k=0; k<=7; k++)
    {
      // clockPin LOW prior to sending bit
       digitalWrite(clockPin, LOW); 
      
      /* Do another bitwise AND against a mask to check the state of
       * each bit as we clock it in.
       */
       
      //Write data if need to light up particular segment piece
      if ( data & (1 << k) )
      {
        digitalWrite(dataPin, isCommonAnnode ? LOW : HIGH); // turn "On"
      }
      else
      {	
        digitalWrite(dataPin, isCommonAnnode ? HIGH : LOW); // turn "Off"
      }

      // and clock the bit in
      digitalWrite(clockPin, HIGH); 
    }
    
    //stop shifting out data
    digitalWrite(clockPin, LOW); 
    
    //set latchPin to high to lock and send data
    digitalWrite(latchPin, HIGH);
    
    // put delay here if you want to see the multiplexing in action!
//    delay(100);  
}

void show(int segPair, bool isLeftSeg, byte number)
{
  
  /* Loop over each segment in the "number" we're about to display,
   * and illuminate only one segment at a time.
   */
  for(int j=0; j<=7; j++)
  {
    /*
    Move over each bit that makes up the seven segment display (from
    left to right, A -> G), and check to see if it should be on or
    not.

    For example, to decide if segment "B" should be illuminated to
    display the digit "6" perform a logical AND with their two byte
    values:

    "6" is 0b10111110 and Segment B is the 2nd segment so its state
    is stored in the second bit from the left. You can see it's
    value is Off.

    The Bitwise AND operation results in the following:
    
       10111110  first operand
     & 01000000  second operand
       --------
       00000000  outcome of bitwise AND: all bits are Off.
    */

    byte toWrite = number & (0b10000000 >> j); 
    
    // Otherwise shift it into the register
    shiftIt(segPair, isLeftSeg, toWrite); 
  }
}

//Takes a number and displays it on the associated 7-seg displays
void displayNumberOnSegPair(int numberToDisplay, int segPair)
{
    //Get the first and second digits for the 7-seg displays
    int lastDigit = numberToDisplay % 10;
    int firstDigit = ((numberToDisplay % 100) - lastDigit) / 10;
    
    //Put a safety on the digit display - can only display digits 0 - 9 on each display
    if(lastDigit > topNumber) lastDigit = 0; 
    if(firstDigit > topNumber) firstDigit = 0; 
    
//    Serial.println(lastDigit);
    
    //Display the digits on the 7-segs
    show(segPair, true, numbers[firstDigit]);
    show(segPair, false, numbers[lastDigit]);
}


//MV-LED replacement function
void MVLEDS(int numberOfCreditsMVLED1, int numberOfCreditsMVLED2)
{
  //MV-LED1 and MV-LED2
 
  //Show number of credits on each 7-seg pair
  if(numberOfCreditsMVLED1 >= 0 && numberOfCreditsMVLED1 <= 99)
  {
    displayNumberOnSegPair(numberOfCreditsMVLED1, 1); 
  }
  
  if(numberOfCreditsMVLED2 >= 0 && numberOfCreditsMVLED2 <= 99)
  {
    displayNumberOnSegPair(numberOfCreditsMVLED2, 2); 
  }
}
