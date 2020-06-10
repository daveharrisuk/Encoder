/*File: encoder_lite.cpp
 *
 * Lightweight Arduino library for 2 phase rotary encoder, with push switch.
 *
 * Author: Dave Harris  (c) 10-June-2020   v1.2
 *
 *
*/
#include "Encoder_lite.h"

#include <Arduino.h>


static uint8_t pin_B;

static uint8_t pin_SW;

static uint16_t maxPos = MAX_POSITION;

static uint16_t minPos = 0;

static bool wrapFlag = true; // default wrapMode true

static volatile uint16_t pos = 32;



static void encoderISR()  //------------------------------------encoderISR----
/*
 * Encoder ISR attached to Pin  Falling. See enc.begin()
*/
{
	static uint32_t	last_ms = 0;
	static uint32_t	duration_ms;

	static uint16_t	step;

  	duration_ms = millis() - last_ms;

  	if (duration_ms < DEBOUNCE_ms)
  	{
  		return; // called within the debounce period = no action
  	}
  	last_ms = millis();

  	if (duration_ms < FAST_THRESHOLD_ms)
  	{
	  	step = STEP_FAST;
  	}
  	else
  	{
  		step = 1;
  	}
  	//entering ISR, pinA is Low, so if pinB is high then pinA != pinB
  	//
  	if (digitalRead(pin_B) == HIGH) // anti clockwise, so decrease position
  	{
		if (pos <= minPos) // at minimum (or below), do we need to wrap?
  		{
			if (wrapFlag)
			{
				pos = maxPos; // wrap to maximum
			}
			else // stay at minimum  (if was below then correct)
			{
				pos = minPos; 	// (defensive code)
			}
  		}
  		else // we are above minimum
  		{
  			if (pos >= (minPos + step)) // enough to do a fast step?
			{
				pos -= step; // step down position (slow or fast)
			}
  			else // we are close to minPos
  			{
  				pos--; // step down slow
  			}
  		}
	}
  	else // so pinA == pinB, so clockwise, so increase position
  	{
	  	if (pos >= maxPos)  // at max (or above), do we need to wrap?
	  	{
	  		if  (wrapFlag)
	  		{
	  			pos = minPos; // wrap to minimum
	  		}
	  		else // stay at maximum  (if was above then correct)
	  		{
	  			pos = maxPos; 	// (defensive code)
	  		}
	  	}
	  	else // we are below maximum
	  	{
			if (pos <= (maxPos - step)) // enough to do a fast step?
			{
				pos += step; 	// step up position (slow or fast)
			}
			else // we are close to maxPos
			{
				pos++; // step up slow
			}
	  	 }
  	}
} //return from interrupt

bool Encoder_lite::begin(byte pinA, byte pinB, byte pinSw)  //-------begin----
/*
 * Set the encoder pins and start the encoder.
 * pinA has to be an interrupt on change pin.
*/
{
	pin_B = pinB;   // for later use in ISR
	pin_SW = pinSw; // for use in pushSwitch

	pinMode(pinA, INPUT);
	pinMode(pinB, INPUT);
	pinMode(pinSw, INPUT);

	attachInterrupt(digitalPinToInterrupt(pinA), encoderISR, FALLING);

	return true; // attachInterrupt() does not return success or failure :-(
};


bool Encoder_lite::switchState(void)  //-----------------------switchState----
/*
 *  return push switch state  1 = pressed, 0 = not pressed
*/
{
	return (bool) ! digitalRead(pin_SW);
}


int Encoder_lite::switchPressed(void)  //------------------switchPressed()----
/*
 * Switch...
 *  Not pressed returns 0
 *  Pressed, after release, returns duration 5 to 10000 ms (10 seconds)
 *  Still pressed after 10 seconds returns -1  (stuck?)
*/
{
	int	duration_ms = 0;

	if (digitalRead(pin_SW) == 1)
	{
		return 0;
	}
	do
	{
		delay(5);
		duration_ms = duration_ms + 5;
		if (duration_ms > 10000)
		{
			return -1;
		}
  	} while (digitalRead(pin_SW) == 0);
	delay(5);
	return duration_ms;
}


uint16_t Encoder_lite::position(void)  //-----------------------position()----
/*
 * read back encoder current position
*/
{
	return pos;
}

bool Encoder_lite::position(uint16_t newPos)  //----------position(newPos)----
/*
 * if pos is between minPos and maxPos, then set the position
 */
{
	if ((newPos >= minPos) && (newPos <= maxPos))
	{
		pos = newPos;
		return true; // success
	}
	return false; // failed
}


uint16_t Encoder_lite::maxPosition(void)  //-----------------maxPosition()----
/*
 * read back encoder maximum position
*/
{
	return maxPos;
}

bool Encoder_lite::maxPosition(uint16_t newMax) //-----maxPosition(newMax)----
/*
 * set encoder maximum position, between 0 and (maxPos -10)
*/
{
	if (newMax > minPos && newMax <= MAX_POSITION) // is new valid?
	{
		maxPos = newMax;

		if (pos > maxPos) // does current need adjusting?
		{
			pos = maxPos; // adjust current
		}
		return true; // success
	}
	return false; // failed
}


uint16_t Encoder_lite::minPosition(void)   //----------------minPosition()----
/*
 * read back encoder minimum position
*/
{
	return minPos;
}

bool Encoder_lite::minPosition(uint16_t newMin) //-----minPosition(newMin)----
/*
 * set encoder minimum position
*/
{
	if (newMin < maxPos) // is new valid?
	{
		minPos = newMin;

		if (pos < minPos) // does current need adjusting?
		{
			pos = minPos; // adjust current
		}
		return true; // success
	}
	return false; // failed
}


void Encoder_lite::wrapMode(bool wrap)  //------------------wrapMode(wrap)----
/*
 * set wrap mode On (default, Wrap position at end limits) or Off.
*/
{
	wrapFlag = wrap;
}


//---------------------------------------------------------------------EoF----
