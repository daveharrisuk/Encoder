/* Encoder_lite Arduino library

// Lightweight Arduino library for 2 phase rotary encoder, with push switch.
//
// This is interrupt driven and does not need polling.
// 
// The cheap encoders are very poor and you need external RC filters.
// This has simple debounce coded. The cheap encoders have lots of bounce
// and you definitely need external RC filters on Pin A, Pin B and Pin SW.
// Suggest 10k and 10nF low pass filter.
//  
// This uses one External Interrupt pin.
//  One of INT0, INT1 on UNO/NANO or INT0..5 on MEGA.
// 
//
// Author: Dave Harris  (c) 06-Jun-2020   contact: wortingukATgmailDOTcom
//
// Version:
// 0.0 DH May2020 started
// 1.0 DH 3-Jun-2020. 
// 1.1 DH 6-Jun-2020 added wrapMode method and wpu option
// 1.2 DH 10-Jun-2020 removed wpu option, added switchPressed method


// using the library...

#include <Encoder_lite.h>

Encoder_lite enc;  // Object Instantiation

//Methods:
// begin, position, minPosition, maxPosition, switchState & switchPressed.


    enc.begin(PinA, PinB, PinSW);

//    Sets up the three pins and starts the encoder interrupt on pinA.
//    The 4 args are the 3 pin numbers being used
//      PinA, Encoder phaseA, it must be an Interrupt (INT) pin.
//      PinB, Encoder phaseB, a general digital input pin.
//      PinSW, Encoder push switch contact, a general digital input pin.
//    Returns true.


ret = enc.position();

//    Returns the current encoder position.  minPos to maxPos


ret = enc.position(value);   // minPos to maxPos, default to 32.

//    Resets the current encoder postion value.
//    If the value is outside of minPos and maxPos, then no change is made.
//    Returns position change success or fail.

  
ret = enc.minPosition();

//    Returns the current minimum position limit. 0-998


ret = enc.minPosition(value);  // 0-998, default=0.

//    Sets the minimum position limit.
//    If value is above maxPos minus 1, then no change is made. 
//    If current position is below new minPos, then is set to minPos.
//    Returns change success or fail.


ret = enc.maxPosition();

//    Returns the current maximum position limit. 1-999


ret = enc.maxPosition(value);  // 1-999, default=999.

//    Sets the maximum position limit, maxPos.
//    If value is below minPos plus 1, then no change is made.
//    If current position is above maxPos, then is set to maxPos.
//    Returns change success or fail.


   enc.wrapMode(bit);  // True= wrapMode on. False= wrapMode off.


ret = enc.switchState();  // 0 = pressed, 1 = not pressed

// returns the push switch state


duration = enc.switchPressed();  // duration ms Pressed
 
// Not pressed returns 0
// Pressed, after release, returns duration ms
// Still pressed after 10 seconds returns -1 (stuck?)

// See examples/test_Encoder_lite.INO

//--------------------------------------EoF---------------------------------
*/