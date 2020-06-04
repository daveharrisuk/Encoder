/*File: Encoder_lite.h
 *
 * Lightweight Arduino library for 2 phase rotary encoder, with push switch.
 *
 * Author: Dave Harris  (c) 03-June-2020
 *
 * Although debounce coded, the cheap encoders are poor and you still
 *  need external RC filters on PIN A and PIN B
 *
 * This uses one External Interrupt pin, INT0/1 on UNO/NANO, INT0..5 on MEGA.
*/
#ifndef ENCODER_LITE_H_
#define ENCODER_LITE_H_

#include <Arduino.h>

#define ENC_DEBOUNCE_mS 30
#define ENC_QUICK_THRESHOLD_mS 50
#define ENC_STEP_SLOW 1
#define ENC_STEP_FAST 10


class Encoder_lite
{
public:

	bool begin(uint8_t pinInA = 2, uint8_t pinInB = 3, uint8_t pinInSw = 4);

	bool pushSwitch(); 	// false = no push, true = push

	uint8_t position(uint8_t);	// 0-255 to set
	uint8_t position(void);   	// for read back

	uint8_t maxPosition(uint8_t);	// 0-255 to set max
	uint8_t maxPosition(void);  	// void for read back

	uint8_t minPosition(uint8_t);	// 0-255 to set min
	uint8_t minPosition(void);		// void for read back
};

void encoderISR(void);

#endif // ENCODER_LITE_H_
//----------------------------------------------------------------------EoF----
