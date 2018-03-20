/*
 * This is the test routine for the Goldilox uLCD-144 G1.
 */

#include <Goldilox_uLCD.h>

//Pins
const uint8_t reset_pin = 2;

//The LCD library
Goldilox_uLCD uLCD;

void setup() {
  // put your setup code here, to run once:
  /*
   * It often helps to give a big delay while the screen boots.
   * The datasheet just says that the screen must be initialised
   * in less than 5 seconds so adjust this delay as needed.
   */
  delay(1000);

  //Initialisation routine. This will also wait for a valid response.
  uLCD.initLCD(&Serial1, 9600, reset_pin);

  /*
   * Now we should be good to go. Here are some test routines.
   */
  /*
   * A string is of the form;
   * putString(int font, int row, int col, uint16_t colour, char string[]);
   */
  uLCD.putString(2, 2, 2, 0xFAAF, "Hello  World");

  /*
   * Put a line under the string
   * Line(int x1, int y1, int x2, int y2, uint16_t colour);
   */
  uLCD.Line(16, 50, 112, 50, 0xFF00);
}

void loop() {
  // put your main code here, to run repeatedly:

}
