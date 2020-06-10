//file: test_Encoder_lite.ino   library Encoder_lite test

#include <Arduino.h>

#include <Encoder_lite.h>

// Pin A must be Interrupt-on-Change pin. INT0/1 on UNO/NANO, INT0..5 on MEGA
// The other pins are general purpose digital input pins

const byte PIN_ENCA	= 2;	// Interrupt-on-Change pin
const byte PIN_ENCB	= 3;	// general purpose pin
const byte PIN_ENCSW = 4;	// general purpose pin

Encoder_lite  enc;  // library Object Instantiation

// the enc object exposes:-
//
// enc.begin(pinA, pinB, pinSw); // set inputs and starts interrupts
//
// bit = enc.switchState();   	// 1=Not pushed. 0=Pushed
//
// val = enc.switchPressed();	// pressed duration in ms
//								// 0 = not pressed
//								// -1 = timeout pressed > 10 seconds
//
// enc.position(pos);			// 0-255 to set position
// val = enc.position();   		// read current position
//
// enc.maxPosition(max);		// 0-255 to set maxPos limit
// val = enc.maxPosition(); 	// read back max
//
// enc.minPosition(min);		// 0-255 to set minPos limit
// val = enc.minPosition();		// read back min
//
// enc.wrapMode(bit);	// True(default)= Wrap position@ min/max False= NoWrap
//

void setup(void) //-------------------------------------------------setup-----
{
	Serial.begin(115200);

	Serial.println(F("Encoder_lite test."));

	enc.begin(PIN_ENCA, PIN_ENCB, PIN_ENCSW);

	Serial.print(F("Defaults; position="));
	Serial.print(enc.position(), DEC);		// Default=32
	Serial.print(F(" minPosition="));
	Serial.print(enc.minPosition(), DEC);	// Default=0
	Serial.print(F(" maxPosition="));
	Serial.println(enc.maxPosition(), DEC);	// Default=999

	Serial.println(F("Set range=0:300, start=52 & wrap=true"));

	enc.position(52);   	// valid between minPos and maxPos
	enc.minPosition(0);  	// valid between 0 and maxPos-1
	enc.maxPosition(300);	// valid between minPos+1 and 999
	enc.wrapMode(true);  	// True or False

	Serial.print(F("Read backs; min="));
	Serial.print(enc.minPosition(), DEC);
	Serial.print(F(" max="));
	Serial.println(enc.maxPosition(), DEC);
}

void loop()  //------------------------------------------------------loop-----
{
	static unsigned int lastPos;
	static int duration;
	if (enc.position() != lastPos)
	{
		lastPos = enc.position(); // save the encoder position
		Serial.print(F("pos="));
		Serial.println(lastPos, DEC);
	}
	duration = enc.switchPressed();
	if (duration > 0)
	{
		Serial.print("pushed (ms)=");
		Serial.println(duration, DEC);
	}
	else if (duration < 0)
	{
		Serial.println("Pressed 10s timeout");
	}
}
//--------------------------------------------------------------------EoF-----
