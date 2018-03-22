/*
* 4DG Goldilox functions for uLCDxxxx G1 models
*
* This has been tested for use on the uLCD-144 G1 model because I 
* couldn't find a compatible library for the old models.
*/
#include "Goldilox_uLCD.h"

Goldilox_uLCD::Goldilox_uLCD() {}

/*
* Initialise the display
* (must be called first before any other
* communication is attempted)
*/
void Goldilox_uLCD::initLCD(HardwareSerial *newSerial, int baud, uint8_t rst_pin) {
	//First set up all the pins and ports
	img_counter = 0; //Image counter to keep track of bitmap images
	displaySerial = newSerial; //Store a reference to the serial port
	reset_pin = rst_pin;//Store the reset pin
	displaySerial->begin(baud); //Start the serial port

	/*
	The reset pin is active LOW. Hold it low
	for a little while to power-on-reset.
	*/
	pinMode(reset_pin, OUTPUT);
	digitalWrite(reset_pin, LOW);
	delay(100);
	digitalWrite(reset_pin, HIGH);
	delay(100);

	//Auto-detect baud. Keep trying until the display responds.
	while (displaySerial->read() != LCD_ACK) {
		sendCommand(LCD_BAUD_DETECT);
		delay(20);
	}
}

/*
* Prints a string to the screen
*/
void Goldilox_uLCD::putString(int font, int row, int col, uint16_t colour, String string) {
	int i = 0;

	sendCommand(LCD_STRING); //Put string
	sendCommand(col); //0 to 15 for 5x7 and 8x8, or 0 to 9 for 8x12.
	sendCommand(row); //0 to 20 for 5x7 or 0 to 15 for 8x fonts.
	sendCommand(font); // 0, 1 or 2
	sendCommand(highByte(colour));
	sendCommand(lowByte(colour));

	for(char& c : string) {
		sendCommand(c);
	}

	sendCommand((byte)0x00); //Terminator

	//Wait for the device to respond
	getResponse();
}

/*
* Draw a circle centred on x, y, with a
* radius and colour.
*/
void Goldilox_uLCD::Circle(int pen, int x, int y, uint16_t colour, int radius) {
	//First set the pen type
	setPen(pen);
	//Now send the commands
	sendCommand(LCD_CIRCLE);
	sendCommand((byte)x);
	sendCommand((byte)y);
	sendCommand((byte)radius);
	sendCommand(highByte(colour));
	sendCommand(lowByte(colour));

	//Wait for the device to respond
	getResponse();
}

/*
* Clear the LCD display
*/
void Goldilox_uLCD::clearLCD() {
	sendCommand(LCD_CLEAR);

	//Wait for the device to respond
	getResponse();
}

/*
* Changes the background colour of the display.
* (this is a really slow operation)
*/
void Goldilox_uLCD::BGColour(uint16_t colour) {
	sendCommand(LCD_BG_COLOUR);
	sendCommand(highByte(colour));
	sendCommand(lowByte(colour));

	//Wait for the device to respond
	getResponse();
}

/*
* Sends a tets byte which should respond with
* 0x06 if successful.
*/
void Goldilox_uLCD::checkComms() {
	sendCommand(LCD_BAUD_DETECT);

	//Wait for the device to respond
	getResponse();
}

/*
* Queries the display firmware to return
* the version information.
*/
void Goldilox_uLCD::versionInfo() {
	sendCommand(LCD_VERSION);
	sendCommand(0x01);

	//Wait for the device to respond
	getResponse();
}

/* 
Draw a coloured line across the screen from x1, y1 to x2, y2.
 */
void Goldilox_uLCD::Line(int x1, int y1, int x2, int y2, uint16_t colour)
{
	//Check values within range
	if (x1 > 127)
		x1 = 127;
	if (x2 > 127)
		x2 = 127;
	if (y1 > 127)
		y1 = 127;
	if (y2 > 127)
		y2 = 127;


	sendCommand(LCD_LINE);
	sendCommand(x1);
	sendCommand(y1);
	sendCommand(x2);
	sendCommand(y2);
	sendCommand(highByte(colour));
	sendCommand(lowByte(colour));

	//Wait for the device to respond
	getResponse();
}

/*
Draw a coloured square with corners at x1, y1 AND x2, y2.
*/
void Goldilox_uLCD::Square(int pen, int x1, int y1, int x2, int y2, uint16_t colour)
{
	//Set the pen type
	setPen(pen);
	//Now send commands
	sendCommand(LCD_RECTANGLE);
	sendCommand(x1);
	sendCommand(y1);
	sendCommand(x2);
	sendCommand(y2);
	sendCommand(highByte(colour));
	sendCommand(lowByte(colour));

	//Wait for the device to respond
	getResponse();
}

/*
Draws a triangle with the given vertices
*/
void Goldilox_uLCD::Triangle(int pen, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t colour) {
	//First apply the requested pen type
	setPen(pen);

	//Now send the triangle commands
	sendCommand(LCD_TRIANGLE);
	sendCommand(x1);
	sendCommand(y1);
	sendCommand(x2);
	sendCommand(y2);
	sendCommand(x3);
	sendCommand(y3);
	sendCommand(highByte(colour));
	sendCommand(lowByte(colour));

	//Wait for the device to respond
	getResponse();
}

/*
Set the pen style for future drawing.
*/
void Goldilox_uLCD::setPen(int value)
{
	sendCommand(LCD_PEN_SIZE);
	sendCommand(value);

	//Wait for the device to respond
	getResponse();
}

/*
Adds an 8x8 bitmap to the LCD memory and returns an index
to pass to drawBitmap().
*/
int Goldilox_uLCD::addBitmap(unsigned char image[]) {
	int this_image = img_counter;

	sendCommand(LCD_ADD_BITMAP);
	sendCommand((byte)this_image);

	//Itterate through and send each image byte to the screen
	for (int i = 0; i < 8; i++) {
		sendCommand((byte)image[i]);
	}

	//Wait for the device to acknowledge
	getResponse();

	//Increment the counter for the next image
	img_counter++;

	return this_image;
}

/*
Draws a bitmap from the LCD RAM. An index for the bitmap is
provided when it is added by addBitmap().
*/
void Goldilox_uLCD::drawBitmap(int img_id, int x, int y, uint16_t colour) {
	sendCommand(LCD_DRAW_BITMAP);
	sendCommand(img_id);
	sendCommand(x);
	sendCommand(y);
	sendCommand(highByte(colour));
	sendCommand(lowByte(colour));

	//Wait for the device to acknowledge
	getResponse();
}

/*
Sends a whole image of N pixels to the device - obviously this could take a while.
Image is an array of bytes with each byte representing the colour of a pixel.
*/
void Goldilox_uLCD::drawImage(int x, int y, int width, int height, unsigned char image[]) {
	sendCommand(LCD_DRAW_IMAGE);
	sendCommand(x);
	sendCommand(y);
	sendCommand(width);
	sendCommand(height);
	sendCommand(0x08); //8-bit colour mode. The display also supports 16-bit colour (0x10).

	int i = 0;
	while (image[i] != 0) {
		sendCommand((byte)image[i++]);
	}

	//Wait for the device to acknowledge
	getResponse();
}

/*
Waits for a response to be received from the device and returns it to the calling function.
*/
char Goldilox_uLCD::getResponse() {
	while (!displaySerial->available());

	return displaySerial->read();
}

/*
Sends a command to the display serial port.
*/
void Goldilox_uLCD::sendCommand(char cmd) {
	displaySerial->write(cmd);
}