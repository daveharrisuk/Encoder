/*file:test_Encoder.ino  test library 'Encoder'
 *
 * Author: Dave Harris  (c) 18-June-2020   v1.3
 *
 * Encoder features:
 * - interrupt driven
 * - programmable position limits, minPos and maxPos
 * - programmable 'wrap at limits'
 * - position increments x10 when fast rotation detected.
 *
 * Push Switch features:
 * - Interrupt driven
 * - switch pressed duration method - threshold = 700 ms
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
*/

#include <Arduino.h>

#include <Encoder.h>


const byte PIN_ENCSW = 2; // an Interrupt pin
const byte PIN_ENCA	= 3;  // an Interrupt pin
const byte PIN_ENCB	= 4;  // a general purpose input pin


Encoder enc;  // library Object Instantiation

// the class Encoder methods are:-
//
//   enc.begin(pinA, pinB, pinSw, wrap, pinB_ACW);
//								//arg 1, 2 and 3 mandatory
// 								// set pin numbers and starts interrupts
//								//arg 4 and 5 optional
// 								// True(default)= Wrap @ min/max False= NoWrap
//								// ACW pinB 0 or 1 for ACW. Depends on wiring
//
// bool = enc.switchState();   	// 1 = Not pushed. 0 = Pushed
//
// enum = enc.switchPressed();	// pressed enum type...
//								// PRESS_NONE is not pressed
//								// PRESS_LONG  > 700 ms
//								// PRESS_SHORT < 700 ms
//								// PRESS_TIMEOUT is pressed > 5 seconds
//
//    enc.position(pos);		// set position 0-255
//
// pos = enc.position();   		// read current position
//
//    enc.limits(min, max);		// limits, min=0-254, max=1-255


void setup(void)  //----------------------------------------------setup()-----
{
	enc.begin(PIN_ENCA, PIN_ENCB, PIN_ENCSW); // 3 of 5 arguments
											  // optional are...
											  // 4th = wrap mode default=true
											  // 5th = pinB_ACW  default=0
	Serial.begin(115200);
	Serial.println("test library 'Encoder'");
	Serial.println("default pos=32 minPos=0 maxPos=255");

	Serial.print("enc.switchState is ");
	Serial.println(enc.switchState());

	Serial.println("enc.limits(1, 199); enc.position(52);");

	enc.position(52);
	enc.limits(1, 199);
}

void loop()  //----------------------------------------------------loop()-----
{
	static byte lastPos;
	static byte pos;

	pos = enc.position();
	if (pos != lastPos) // log if pos has changed
	{
		Serial.print( (pos > lastPos) ? '+' : '-'); // direction
		Serial.print("pos=");
		Serial.println(pos, DEC);
		lastPos = pos; // save the encoder position
	}
	switch (enc.switchPressed())
	{
	case PRESS_LONG: // (the enum press_t comes from library)
		Serial.println("PRESS_LONG(>700 ms)");
		break;
	case PRESS_SHORT:
		Serial.println("PRESS_SHORT(<700 ms)");
		break;
	case PRESS_TIMEOUT:
		Serial.println("PRESS_TIMEOUT(>5000 ms) pos reset");
		enc.position(32);
		break;
	case PRESS_NONE:
		break;
	}
	delay(1);
}
//--------------------------------------------------------------------EoF-----
