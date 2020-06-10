//#include "Arduino.h"
// simple encoder sketch. No pos limits or fast steps.
#define PINA 2
#define PINB 3
#define DEBOUNCE_ms 4
volatile int pos = 0; // position global variable

// encoder Interrupt Service Routine, on PINA falling
void encoderISR() {    // so, PINA just went LOW...
  static unsigned long last_ms = 0;
  if ((millis() - last_ms) < DEBOUNCE_ms)
  { return; } // within debounce time so return early
  last_ms = millis(); // update the time
  if (digitalRead(PINB) == HIGH)
  { pos--; } // rotated ACW decrease pos
  else
  { pos++; } // rotated CW increase pos
}
void setup() {
 Serial.begin(9600);
 attachInterrupt(digitalPinToInterrupt(PINA)
     , encoderISR, FALLING);
}
void loop() {
  static int lastPos = -1; // to see if pos has changed
  if (pos != lastPos) { // has pos changed?
    lastPos = pos; // save the encoder position
    Serial.print(" pos="); Serial.println(pos);
  }
}
