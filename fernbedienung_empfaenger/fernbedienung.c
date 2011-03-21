#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

#include "rfm12.h"

int main ( void ) {
	uint8_t teststring[] = "teststring\r\n";
	uint8_t packettype = 0xEE;
	rfm12_init();  /* initialize the library */
	sei();
	
	
	DDRB |= (1<<PB0);
	DDRB |= (1<<PB1);
	PORTB |= (1<<PB1);
			
	while (1000)
	{
		/* ... */
		rfm12_tx (sizeof(teststring), packettype, teststring);
		rfm12_tick();  /* periodic tick function - call that one once in a while */
		PORTB |= (1<<PB0);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		PORTB &= ~(1<<PB0);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
	}
}
