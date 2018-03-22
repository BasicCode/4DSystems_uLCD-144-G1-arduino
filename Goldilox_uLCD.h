
#ifndef Goldilox_uLCD_h
#define Goldilox_uLCD_h

#include "Arduino.h"

//LCD commands
#define LCD_ACK 0x06
#define LCD_NAK 0x15
#define LCD_BAUD_DETECT 0x55
#define LCD_BG_COLOUR 0x42
#define LCD_SET_FONT 0x46
#define LCD_STRING 0x73
#define LCD_RECTANGLE 0x72
#define LCD_PEN_SIZE 0x70
#define LCD_CLEAR 0x45
#define LCD_LINE 0x4C
#define LCD_TRIANGLE 0x47
#define LCD_CIRCLE 0x43
#define LCD_ADD_BITMAP 0x41
#define LCD_DRAW_BITMAP 0x44
#define LCD_DRAW_IMAGE 0x49
#define LCD_VERSION 0x56

class Goldilox_uLCD {
public:
	Goldilox_uLCD();

	void initLCD(HardwareSerial *newSerial, int baud, uint8_t rst_pin);

	void putString(int font,int row, int col, uint16_t colour, String string);

	void Circle(int pen, int x, int y, uint16_t colour, int radius);

	void clearLCD();

	void BGColour(uint16_t colour);

	void checkComms();

	void versionInfo();

	void Line(int x1, int y1, int x2, int y2, uint16_t colour);

	void Square(int pen, int x1, int y1, int x2, int y2, uint16_t colour);

	void Triangle(int pen, int x1, int y1, int x2, int y2, int x3, int y3, uint16_t colour);

	void setPen(int value);

	int addBitmap(unsigned char image[]);

	void drawBitmap(int img_id, int x, int y, uint16_t colour);

	void drawImage(int x, int y, int width, int height, unsigned char image[]);

private:
	HardwareSerial *displaySerial;
	uint8_t reset_pin;
	int img_counter;

	char getResponse();

	void sendCommand(char cmd);
};
#endif