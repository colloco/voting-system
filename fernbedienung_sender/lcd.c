#include <avr/io.h>
#include "lcd.h"

typedef struct
{
	uint8_t port;
	uint8_t bit;
	uint8_t ddr;
} segment;

const segment segment1[] = {	
	{PORTC ,PC1, DDRB}, // 1A
	{PORTC, PC0, DDRB}, // 1B
	{PORTC, PC4, DDRE}, // 1C
	{PORTC, PC5, DDRE}, // 1D
	{PORTC, PC6, DDRD}, // 1E
	{PORTC, PC2, DDRD}, // 1F
	{PORTC, PC3, DDRD}, // 1G
	{PORTC, PC7, DDRD}  // DP1
};

const segment segment2[] = {	
	{PORTA ,PA1, DDRB}, // 2A
	{PORTA, PA0, DDRB}, // 2B
	{PORTA, PA4, DDRE}, // 2C
	{PORTA, PA5, DDRE}, // 2D
	{PORTA, PA6, DDRD}, // 2E
	{PORTA, PA2, DDRD}, // 2F
	{PORTA, PA3, DDRD}, // 2G
	{PORTA, PA7, DDRD}  // DP2
};
const segment bp[] = {	
	{PORTB ,PB3, DDRB}, // BP
};

const char* lookup[] = {
	/*0*/ "012345",
	/*1*/ "01",
	/*2*/ "50632",
	/*3*/ "50612",
	/*4*/ "4601",
	/*5*/ "54612",
	/*6*/ "543216",
	/*7*/ "501",
	/*8*/ "0123456",
	/*9*/ "645012",
	/*DP*/ "7",
};



uint8_t toggleLCD = 0;
uint8_t digitSend[6] = {0b01110110,0b01100111,0b01010111,0b00110111,0b01110011,0b01110101};

void initLCD( void )
{
	DDRB |= (1<<PB3); // output BP
	DDRA = 0xFF; // output PortA
	DDRC = 0xFF; // output PortC
}

void writeDigitSend()
{
	static uint8_t i = 0;
	static int count = 0;

	if (toggleLCD == 1)
	{
		PORTB &= ~(1<<PB3); // voltage off BP
		
		PORTA = digitSend[i];
	} else {
		PORTB |= (1<<PB3); // voltage on BP
		
		PORTA = ~digitSend[i];
	}
	
	count++;
	if (count >= 15000)
	{
		i++;
		count = 0;
	}
	if (i > 5)
	{
		i = 0;
	}	
}

void writeDigit1(uint8_t digit)
{
	if (toggleLCD == 1)
	{
		PORTB &= ~(1<<PB3); // voltage off BP
		
		PORTA = digit;
	} else {
		PORTB |= (1<<PB3); // voltage on BP
		
		PORTA = ~digit;
	}
}

void writeDigit2(uint8_t digit)
{
	if (toggleLCD == 1)
	{
		PORTB &= ~(1<<PB3); // voltage off BP
		
		PORTC = digit;
	} else {
		PORTB |= (1<<PB3); // voltage on BP
		
		PORTC = ~digit;
	}
}

void showDigit(uint16_t digit, uint8_t place)
{
	uint8_t figure;
	switch (digit) {
		case 0: // 0
			figure = 0b01110111;
		break;
		case 1: // 1
			figure = 0b00010001;
		break;
		case 2: // 2
			figure = 0b01101011;
		break;
		case 3: // 3
			figure = 0b00111011;
		break;
		case 4: // 4
			figure = 0b00011101;
		break;
		case 5: // 5
			figure = 0b00111110;
		break;
		case 6: // 6
			figure = 0b01111110;
		break;
		case 7: // 7
			figure = 0b00010011;
		break;
		case 8: // 8
			figure = 0b01111111;
		break;
		case 9: // 9
			figure = 0b00111111;
		break;
		case 111: // ok
			writeDigitSend();
		break;
		case 222:  // cancel
			figure = 0b00000000;
		break;
		default:
			figure = 0b11111111;
		break;
	}
	
	if (digit != 111)
	{
		if (place == 1)
		{
			writeDigit1(figure);
		} else {
			writeDigit2(figure);
		}
	}
}

// Der Funktion splitNum() wird die Variable num übergeben. Diese wird hier in 2 Ziffern zerlegt.
void splitNum(uint16_t num)
{
	uint8_t digit1;
	uint8_t digit2;
	if (num > 10 && num < 100) // falls num zweistellig ist
	{
		digit1 = num%10; // erste Ziffer mit Modulo
		num = num/10; // hier wird die Ziffer aus dem 10-Werten ermittelt
		digit2 = num%10;
				
		showDigit(digit1, 1); // der Funktion showDigit() wird eine Ziffer mit der dazugehörigen Stelle übergeben
		showDigit(digit2, 2);
	} else if ((num > 0 && num < 10) || num > 100) { 
		showDigit(num, 1);
		writeDigit2(0b00000000);
	} else {
		writeDigit1(0b00000000);
		writeDigit2(0b00000000);
	}
	
}
