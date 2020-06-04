/*File: encoder_lite.cpp
 *
 *
 *
*/
#include "Encoder_lite.h"

#include <Arduino.h>


static uint8_t _pinB;

static uint8_t _pinSW;

static uint16_t _maxPos = 255;

static uint16_t _minPos = 0;

static volatile uint16_t _curPos = 31;


bool Encoder_lite::begin(byte pinInA, byte pinInB, byte pinInSw) //---begin----
/*
 * Set the encoder pins and start the encoder.
 * pinInA has to be an interrupt on change pin.
*/
{
	_pinB = pinInB;   // for latter use in ISR
	_pinSW = pinInSw; // used latter in pushSwitch

	pinMode(pinInA, INPUT_PULLUP);
	pinMode(pinInB, INPUT_PULLUP);
	pinMode(pinInSw, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(pinInA), encoderISR, FALLING);

	return true; // attachInterrupt() does not return success or failure :-(
};


bool Encoder_lite::pushSwitch()  //------------------------------pushSwitch----
/* return push switch state */
{
	return ! (bool)digitalRead(_pinSW);
}


uint8_t Encoder_lite::position(void)  //--------------------------position----
/*
 * read back encoder current position
*/
{ return _curPos; }

uint8_t Encoder_lite::position(uint8_t pos)  //-------------------position----
/*
 * if pos is between minPos and maxPos, then presets the position
 */
{
	if (pos >= _minPos && pos <= _maxPos)
	{
		_curPos = (uint16_t) pos;
	}
	return _curPos;
}


uint8_t Encoder_lite::maxPosition(void)  //---------------------maxPosition----
/*
 * read back encoder maximum position
*/
{ return _maxPos; }

uint8_t Encoder_lite::maxPosition(uint8_t max)  //--------------maxPosition----
/*
 * set encoder maximum position
 */
{
	if (max > _minPos + 10)
	{
		_maxPos = max;
		if (_curPos > max)
		{
			_curPos = (uint16_t) max;
		}
	}
	return _maxPos;
}


uint8_t Encoder_lite::minPosition(void)   //--------------------minPosition----
/*
 * read back encoder minimum position
*/
{ return _minPos; }

uint8_t Encoder_lite::minPosition(uint8_t min)  //--------------minPosition----
/*
 * set encoder minimum position
*/
{
	if (min < _maxPos - 10)
	{
		_minPos = min;
		if (_curPos < min)
		{
			_curPos = (uint16_t) min;
		}
	}
	return _minPos;
}


void encoderISR()  //--------------------------------------------encoderISR----
/*
 * Encoder ISR attached to Pin  Falling. See enc.begin()
*/
{
	static uint32_t	lastTime_mS = 0;
	static uint32_t	duration_mS = 0;
	static uint8_t	step = 1;

  	 duration_mS = millis() - lastTime_mS;

  	 if (duration_mS < ENC_DEBOUNCE_mS)
  	 {
  		 return; // called within the debounce period = no action
  	 }

  	 lastTime_mS = millis();

  	 if (duration_mS < ENC_QUICK_THRESHOLD_mS)
  	 {
	  	 step = ENC_STEP_FAST;
  	 }
 	 else
  	 {
	  	 step = ENC_STEP_SLOW;
  	 }
  	 //entering ISR, pinA is Low, so if pinB is high then pinA != pinB

  	 if (digitalRead(_pinB) == HIGH)
  	 {
		if (_curPos >= (_minPos + step))
		{
			_curPos = _curPos - step; // anti clockwise
		}
  	 }
  	 else
	 if (_curPos <= (_maxPos - step))
	 {
		 _curPos = _curPos + step; // clockwise
	 }
  	 return; // from interrupt
}

//----------------------------------------------------------------------EoF----
