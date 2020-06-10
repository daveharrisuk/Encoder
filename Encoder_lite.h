/*File: Encoder_lite.h
 *
 * Arduino library for two phase rotary encoder, with push switch.
 *
 * Author: Dave Harris  (c) 10-June-2020   v1.2
 *
 * This has very simple debounce code. The cheap encoders have lots of bounce
 * and you definitely need external RC filters on Pin A, Pin B and Pin SW.
 * Suggest 10k and 10nF low pass filter.
 *
 * Coded to need one External Interrupt, INT0/1 on UNO/NANO, INT0..5 on MEGA.
*/
#ifndef ENCODER_LITE_H_
#define ENCODER_LITE_H_

#include <Arduino.h>

#define DEBOUNCE_ms 5
#define FAST_THRESHOLD_ms 35
#define MAX_POSITION 999U
#define STEP_FAST 10


class Encoder_lite
{
public:

  bool begin(byte, byte, byte);	// pinA, pinB, pinSw

  bool position(uint16_t);		// between minPos and maxPos, to set position

  uint16_t position(void);   	// read current position

  bool maxPosition(uint16_t);	// minPos to MAX_POSITION to set maxPos limit

  uint16_t maxPosition(void);	// read back maximum position

  bool minPosition(uint16_t);	// MIN_POSITION to maxPos to set minPos limit

  uint16_t minPosition(void);	// Read back minimum position

  void wrapMode(bool = true);	// true:wrap position=limit. false:no wrap

  bool switchState(void); 		// 1 = not pushed, 0 = pushed

  int switchPressed(void); 		// Pressed for duration = 5 to 10000 ms
								// Not pressed = 0
								// Still pressed after 10 seconds = -1
};

//static void encoderISR(void);

#endif // ENCODER_LITE_H_
//----------------------------------------------------------------------EoF----
