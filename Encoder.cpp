/*File: Encoder.cpp
 *
 * Arduino library for 2 phase rotary encoder, with push switch.
 *
 * Author: Dave Harris  (c) 18-June-2020   v1.3
*/

#include <Arduino.h>
#include <Encoder.h>


enum swStatus_t : byte // switchISR(), encoderISR() & class Encoder use this
{
    _DOWN = 0		// ISR flags DOWN
  , _UP = 1			// ISR flags UP
  , _PROCESSED = 2	// Processed and has been notified to user
  , _TIMEOUT = 3	// flag DOWN for too long
};


// switchISR(), encoderISR() & class Encoder use the below variable

byte _pinB;  // set by .begin    no default

byte _pinSw; // set by .begin    no default

bool _wrap;  // set by .begin    default wrap mode true

bool _pinB_ACW; // set by .begin   default 0

byte _maxPos = 255;

byte _minPos = 0;

volatile byte _pos = 32;

volatile unsigned long _swFirstDown_ms;

volatile unsigned long _swLastUp_ms;

volatile swStatus_t  _swStatus;



press_t Encoder::switchPressed(void)  //-------------------switchPressed()----
/*
 * returns enum
 *  PRESS_NONE not pressed
 *  PRESS_LONG after a Long press
 *  PRESS_SHORT after a Short press
 *  PRESS_TIMEOUT still pressed down after 5 seconds
 *
 * globals variables: _swStatus, _swFirstDown_ms, _swLastUp_ms
 * calls : millis(), delay()
*/
{
	unsigned int duration_ms = 0;

	if (_swStatus >= _PROCESSED) // PROCESSED or TIMEOUT
	{
		return PRESS_NONE; // no pressed
	}
	// so is switch DOWN or UP?

	while (_swStatus == _DOWN) // wait while switch is DOWN
	{
		duration_ms = (unsigned int) (millis() - _swFirstDown_ms);
		if (duration_ms < 5)
		{
			delay(duration_ms); // simple debounce, if needed
		}
		if (duration_ms > STUCKDOWN_ms)
		{
			//_swFirstDown_ms = millis();
			_swStatus = _TIMEOUT;
			return PRESS_TIMEOUT; // its down way too long
		}
  	} // switch is UP now

	_swStatus = _PROCESSED; // replace UP status with PROCESSED

	if ((millis() - _swLastUp_ms) > HISTORIC_ms) // too historic?
	{
		return PRESS_NONE;
	}
	if (duration_ms > LONG_PRESS_ms)
	{
		return PRESS_LONG;
	}
	return PRESS_SHORT;
}


static void switchISR()  //------------------------------------switchISR()----
/*
 * switch Interrupt Service Routine attached to PinSw on Change.
 *
 * @16 MHz it typically takes 8.3 us
 *
 * global variables: _swStatus, _swFirstDown_ms, _swLastUp_ms
 * calls : millis(), digitalRead()
*/
{
	if (digitalRead(_pinSw) == LOW) // switch went High to Low = DOWN
	{
		if (_swStatus >= _PROCESSED ) // PROCESSED or TIMEOUT
		{
			_swFirstDown_ms = millis(); // note the time
		}
		_swStatus = _DOWN;
	}
	else // switch went Low to High = UP
	{
		if (_swStatus == _TIMEOUT)
		{
			_swStatus = _PROCESSED;
		}
		else
		{
			_swStatus = _UP;
			_swLastUp_ms = millis(); // note the time
		}
	}
}    //return from interrupt


static void encoderISR()  //----------------------------------encoderISR()----
/*
 * Encoder Interrupt Service Routine attached to PinA on Falling.
 *
 * @16 MHz it typically takes 11.5 us
 *
 * global variables: _pos, _minPos, _maxPos, _wrap
 * calls : millis(), digitalRead()
*/
{
	static unsigned long last_ms = 0; // carried over to next interrupt

	unsigned int duration_ms = (millis() - last_ms);

  	if (duration_ms < VERYFAST_ms)
  	{
  		return; // pulses can be corrupt = no action
  	}
  	last_ms = millis();

	byte step = 1;

  	if (duration_ms < FAST_ms)
  	{
	  	step = FAST_STEP;
  	}

  	// entering ISR, pinA just went Low, so test pinB to determine direction

  	if (digitalRead(_pinB) == _pinB_ACW) // ACW, so decrease position
  	{
		if (_pos <= _minPos) // at minimum (or below), do we need to wrap?
  		{
			if (_wrap)
			{
				_pos = _maxPos; // wrap to maximum
			}
			else // stay at minimum  (if was below then correct)
			{
				_pos = _minPos; // (defensive code)
			}
  		}
  		else // we are above minimum
  		{
  			if (_pos >= (_minPos + step)) // enough to do a fast step?
			{
				_pos -= step; // step down position (slow or fast)
			}
  			else // we are close to minPos
  			{
  				_pos--; // step down slow
  			}
  		}
	}
  	else // so clockwise, so increase position
  	{
	  	if (_pos >= _maxPos)  // at max (or above), do we need to wrap?
	  	{
	  		if  (_wrap)
	  		{
	  			_pos = _minPos; // wrap to minimum
	  		}
	  		else // stay at maximum  (if was above then correct)
	  		{
	  			_pos = _maxPos; // (defensive code)
	  		}
	  	}
	  	else // we are below maximum
	  	{
			if (_pos <= (_maxPos - step)) // enough to do a fast step?
			{
				_pos += step; 	// step up position (slow or fast)
			}
			else // we are close to maxPos
			{
				_pos++; // step up slow
			}
	  	 }
  	}
}    //return from interrupt


void Encoder::begin(  //-------------------------------------------begin()----
		byte pinA, byte pinB, byte pinSw, bool wrap, bool pinB_ACW)
/*
 * Set the encoder pins and start the encoder.
 * pinA has to be an interrupt on change pin.
*/
{
	_pinB = pinB;   		// for later use in ISR
	_pinSw = pinSw; 		// for use in switch code
	_wrap = wrap;   		// wrap mode
	_pinB_ACW = pinB_ACW; 	// pinB_ACW: true=high level, false=low level

	pinMode(pinA, INPUT);
	pinMode(pinB, INPUT);
	pinMode(pinSw, INPUT);

	_swStatus = ( digitalRead(pinA) == 0 ? _DOWN : _PROCESSED );

	attachInterrupt(digitalPinToInterrupt(pinA), encoderISR, FALLING);

	attachInterrupt(digitalPinToInterrupt(pinSw), switchISR, CHANGE);
};


bool Encoder::switchState(void)  //--------------------------switchState()----
/*
 *  return push switch state  0 = pressed, 1 = not pressed
*/
{
	return digitalRead(_pinSw);
}


byte Encoder::position(void)  //--------------------------------position()----
/*
 * read back encoder current position
*/
{
	return _pos;
}


void Encoder::position(byte newPos)  //-------------------position(newPos)----
/*
 * set the position. If outside of minPos and maxPos then adjust.
*/
{
	if (newPos < _minPos)
	{
		_pos = _minPos;
	}
	else
	{
		if (newPos > _maxPos)
		{
			_pos = _maxPos;
		}
		else
		{
			_pos = newPos;
		}
	}
}


void Encoder::limits(byte min, byte max)  //---------------limits(min,max)----
/*
 * set encoder position limits
*/
{
	if (max > min) // defensive check
	{
		_minPos = min;
		_maxPos = max;

		position(_pos); // does _pos need adjusting?
    }
}

//---------------------------------------------------------------------EoF----
