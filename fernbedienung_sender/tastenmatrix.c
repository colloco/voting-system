#include <avr/io.h>

#include "tastenmatrix.h"

#define COLUMN1 PD0
#define COLUMN2 PD1
#define COLUMN3 PD2
#define ROW1 PD3
#define ROW2 PD4
#define ROW3 PD5
#define ROW4 PD6


/* Datenrichtungsregister für Port. Bit im Register gesetzt (1) für Ausgang, Bit gelöscht (0) für Eingang. */
#define OUTPUT(port,pin) (port |= (1<<pin))
#define INPUT(port, pin) (port &= ~(1<<pin))

/* Bei Pins, die mittels DDRx auf Eingang geschaltet wurden, können über PORTx die internen Pull-Up Widerstände aktiviert oder deaktiviert werden (1 = aktiv). */
#define PULLUPON(port, pin) (port |= (1<<pin))
#define PULLUPOFF(port, pin) (port &= ~(1<<pin))

#define VOLTAGEON(port, pin) (port |= (1<<pin))
#define VOLTAGEOFF(port, pin) (port &= ~(1<<pin))

void initKeyboard( void )
{
	INPUT(DDRD, ROW1);
	PULLUPON(PORTD, ROW1);
	INPUT(DDRD, ROW2);
	PULLUPON(PORTD, ROW2);
	INPUT(DDRD, ROW3);
	PULLUPON(PORTD, ROW3);
	INPUT(DDRD, ROW4);
	PULLUPON(PORTD, ROW4);
	
	OUTPUT(DDRD, COLUMN1);
	VOLTAGEON(PORTD, COLUMN1);
	OUTPUT(DDRD, COLUMN2);
	VOLTAGEON(PORTD, COLUMN2);
	OUTPUT(DDRD, COLUMN3);
	VOLTAGEON(PORTD, COLUMN3);
}

uint8_t getDigit(void) {

	uint16_t pad_status = 0x0000;
	uint8_t digit;

	VOLTAGEOFF(PORTD, COLUMN1);
	asm("nop");
	pad_status = ~(PIND)&0b01111000;
	asm("nop");
	VOLTAGEON(PORTD, COLUMN1);
	asm("nop");
	VOLTAGEOFF(PORTD, COLUMN2);
	asm("nop");
	pad_status |= (~(PIND)&0b01111000)<<4;
	asm("nop");
	VOLTAGEON(PORTD, COLUMN2);
	asm("nop");
	VOLTAGEOFF(PORTD, COLUMN3);
	asm("nop");
	pad_status |= (~(PIND)&0b01111000)<<8;
	asm("nop");
	VOLTAGEON(PORTD, COLUMN3);
	
	switch (pad_status) {
		case 0x0400: // 0
			digit = 0;
		break;
		case 0x0800: // 1
			digit = 1;
		break;
		case 0x0080: // 2
			digit = 2;
		break;
		case 0x0008: // 3
			digit = 3;
		break;
		case 0x1000: // 4
			digit = 4;
		break;
		case 0x0100: // 5
			digit = 5;
		break;
		case 0x0010: // 6
			digit = 6;
		break;
		case 0x2000: // 7
			digit = 7;
		break;
		case 0x0200: // 8
			digit = 8;
		break;
		case 0x0020: // 9
			digit = 9;
		break;
		case 0x4000: // ok
			digit = 111;
		break;
		case 0x0040:  // cancel
			digit = 222;
		break;
		case 0x0000: // no action
			digit = 100;
		break;
		default:
			digit = 121; // mehrere Tasten gleichzeitig
		break;
	}

	return digit;
}
