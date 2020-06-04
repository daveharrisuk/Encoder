// test_Encoder_lite library
#include <Arduino.h>

#include <Encoder_lite.h>

// Pin A must be Interrupt-on-Change pin. INT0/1 on UNO/NANO, INT0..5 on MEGA.
#define PIN_ENCA	2
#define PIN_ENCB	3
#define PIN_ENCSW	4

Encoder_lite enc;  // Object Instantiation

void setup()
{
	enc.begin(PIN_ENCA, PIN_ENCB, PIN_ENCSW); // start enc

	Serial.begin(115200);
	Serial.println("Encoder_lite library test");

	Serial.print("Defaults. position=");
	Serial.print(enc.position());
	Serial.print(" minPosition=");
	Serial.print(enc.minPosition());
	Serial.print(" maxPosition=");
	Serial.println(enc.maxPosition());

	Serial.println("Set encoder start at 80, range 4 to 160");
	enc.position(80);
	enc.minPosition(4);
	enc.maxPosition(160);

	Serial.print("Read back min=");
	Serial.print(enc.minPosition());
	Serial.print(" max=");
	Serial.println(enc.maxPosition());

}
void loop()
{
	static uint8_t pos;
	static bool sw;
	if (enc.position() != pos)
	{
		pos = enc.position(); // read the encoder rotary position
		Serial.print("pos=");
		Serial.println(pos, DEC);
	}
	if (enc.pushSwitch() != sw)
	{
		sw = enc.pushSwitch(); // read the encoder push switch
		Serial.print("push");
		Serial.println(sw, DEC);
	}
}
