/* Encoder_lite Arduino library

//Lightweight Arduino library for 2 phase rotary encoder, with push switch.
//It is interrupt driven and does not need polling.
// 
// Although debounce coded, the cheap encoders are very poor and you still
//  need external RC filters on PIN A and PIN B
//  
// This uses one External Interrupt pin.
// It must be INT0/1 on UNO/NANO, INT0..5 on MEGA.
// 
//
// Author: Dave Harris  (c) 03-Jun-2020   contact: wortingukATgmailDOTcom
//
// Version:
// 0.0 DH May2020 started
// 1.0 DH 3-Jun-2020. 


// using the library...
// in the file manager, unzip the library into the libraies folder in your
// personal Arduino folder.
//
// On my PC its C:\Users\wortinguk\Documents\Arduino\libraries\Encoder_lite
// But yours will be different.

// Open the examples test_Encoder_lite.ino file ...
// You will need to edit the pin number definitions to suit your Arduino board.
// Verify and load to the board.
// Check out how it works and enjoy.



#include <Encoder_lite.h>  // this gets the library from the libaries folder

Encoder_lite enc;  // This Instantiates the object 'enc'

//Methods exposed by the object are...
//         begin, position, minPosition, maxPosition and pushSwitch.

// notes on the methods...

status = enc.begin(PinInA, PinInB, PinInSW);

//    Sets up the three pins and starts the encoder interrupt.
//    The 3 args are the 3 pin numbers being used...
//      PinInA, Encoder phaseA, it must be an Interrupt (INT) pin.
//      PinInB, Encoder phaseB, a general digital input pin.
//      PinInSW, Encoder push switch contact, a general digital input pin.
//    Returns true.


ret = enc.position();

//    Returns the current encoder position. 0-255


ret = enc.position(value);   // 0-255, default to 31.

//    Resets the current encoder postion value.
//    If the value is outside of minPos and maxPos, then
//    resets then no change is made.
//    Returns the value set.

  
ret = enc.minPosition();

//    Returns the current minimum position limit. 0-245


ret = enc.minPosition(value);  // 0-245, default=0.

//    Sets the minimum position limit.
//    If value is above maxPos minus 10, then no change is made. 
//    If current Position is below new minPos, then curPos is set to minPos.
//    Returns the value set.


ret = enc.maxPosition();

//    Returns the current maximum position limit. 10-255


ret = enc.maxPosition(value);  // 10-255, default=255.

//    Sets the maximum position limit, maxPos.
//    If value is below minPos +10, then no change is made.
//    If current Posistion is above maxPos, then curPos is set to maxPos.
//    Returns the value set.


ret = enc.pushSwitch();  // 0 or 1

// returns the push switch state



// See examples/test_Encoder_lite.INO

//--------------------------------------EoF---------------------------------*/