/* Encoder library
//
// Author: Dave Harris (c)  18-Jun-2020  v1.3   Contact:wortingukATgmailDOTcom
//
//
// Arduino library for 2 phase rotary encoder, with push switch.
//
// This is interrupt driven and does not need polling.
// 
// Uses 3 pins, phaseA (PinA), phaseB (PinB) and switch (PinSw).
// Requires Interrupt pins for pinA and pinSw.
//  Available are INT0, INT1 on UNO/NANO or INT0..5 on MEGA.
// 
// The cheap encoders have lots of contact bounce and require external
// Resistor/Capacitor filters on PinA, PinB and PinSw between encoder and MCU. 
// Suggest 10k and 10nF low pass. Also each needs a 10k pull up before filter.
//  
//
// Version:
// 0.0 DH May2020 started
// 1.0 DH 3-Jun-2020. 
// 1.1 DH 6-Jun-2020 added wrapMode method and wpu option
// 1.2 DH 10-Jun-2020 removed wpu option, added switchPressed method
// 1.3 DH 16-Jun-2020 change switchPress to duration measure method

// using the library...

#include <Encoder.h>

Encoder enc;  // class 'Encoder' instantiation as 'enc'

//Methods:
// begin, position, limits, switchState & switchPressed.


    enc.begin(PinA, PinB, PinSw, wrap, pinB_ACW);

//    Sets up the three pins and starts the interrupts.
//    The 5 args are pin numbers being used
//      PinA, Encoder phaseA, it must be an Interrupt (INT) pin.
//      PinB, Encoder phaseB, a general digital input pin.
//      PinSw, Encoder push switch, it must be an Interrupt (INT) pin.
//      wrap: true = wrap position@limit. false = no wrap
//      pinB_ACW: 1 = ACW high level, 0 = ACW low level


pos = enc.position();   // 0-255

// returns the current encoder position. Between minPos to maxPos


  enc.position(pos);   // 0-255 ... from minPos to maxPos.

// Resets the current encoder postion value.


  enc.limits(minPos, maxPos);  // 0-254, 1-255
  
// sets the position limits. If min > max, does nothing
// If pos is outside of min/max, then pos is reset inside limits


state = enc.switchState();  // 0 = pressed, 1 = not pressed

// returns the push switch state


state = enc.switchPressed();  // duration measure method
 
// returns enum PRESS_NONE, PRESS_SHORT (<700ms), PRESS_LONG (>700ms),
//                  PRESS_TIMEOUT (pressed > 5 seconds)


// See examples/test_Encoder.INO

//--------------------------------------EoF---------------------------------
*/