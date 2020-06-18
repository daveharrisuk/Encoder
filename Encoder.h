#ifndef ENCODER_H_
#define ENCODER_H_
/*
 *  Arduino library for 2 phase rotary encoder with push switch
 *
 * Author: Dave Harris  (c) 18-June-2020   v1.3
 *
 * Encoder features:
 * - interrupt driven
 * - configurable position limits, minPos and maxPos
 * - configurable 'wrap at limits'
 * - position increments x10 when fast rotation detected.
 *
 * Push Switch features:
 * - Interrupt driven
 * - Uses switch pressed duration method - threshold = 700 ms
 *    . shorter than threshold is a short press
 *    . longer than threshold is a long press
 * - switch press stuck time-out of 5 seconds
 *
 * Cheap encoders have lots of contact bounce and require external RC
 * filters on Pin A, Pin B and Pin SW. Suggest 10k and 10nF low pass filter.
 * The encoder pins require pull-up resistors(not weak pull-up), suggest 10k.
 *
 * Library needs a general input pin for pinB and Interrupt pins for
 *  pinA and pinSw. (available are INT0/1 on UNO/NANO or INT0..5 on MEGA).
 *
 *  Uses 2686 byte of program space
*/
#include <Arduino.h>

enum press_t : byte // Used in class Encoder & outside of library
{
    PRESS_NONE = 0
  , PRESS_LONG  = 1
  , PRESS_SHORT  = 2
  , PRESS_TIMEOUT = 3
};

class Encoder
{
 public:

  void begin(byte, byte, byte, bool = true, bool = 0);
    						// Required: pinA, pinB, pinSw as pin numbers
  	  	  	  	  	  	  	// Optional: wrap, pinB_ACW
							//  wrap: true=wrap position@limit. false=none
  	  	  	  	  	  	  	//  pinB_ACW: 1=high level, 0=low level

  press_t switchPressed();  // PRESS_NONE, PRESS_LONG, PRESS_SHORT (< 700 ms)
							// and PRESS_TIMEOUT (pressed > 5 seconds)

  void position(byte);		// set position (_minPos to _maxPos)

  byte position();			// read current position (_pos)

  void limits(byte, byte);	// minPos, maxPos (_minPos & _maxPos)

  bool switchState();		// Read switch, 1 = not pushed, 0 = pushed
};


// switchISR(), encoderISR() & class Encoder use the below static const.

static const byte VERYFAST_ms  = 6;
// encoder pulses quicker than this are ignored

static const byte FAST_ms      = 35;
// encoder pulses quicker than this cause fast step

static const byte FAST_STEP    = 10;
// when rotated fast, step by this amount

static const uint16_t LONG_PRESS_ms = 700;
// long press threshold in ms

static const uint16_t STUCKDOWN_ms = 5000;
// timeout for stuck down switch in ms

static const uint16_t HISTORIC_ms = 500;
// enc.switchPressed() after this period marks an unprocessed UP as PRESS_NONE

#endif // ENCODER_H_ --------------------------------------------------EoF----
