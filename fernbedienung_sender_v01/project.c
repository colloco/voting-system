#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA |= (1<<PA7);
	while(1)
	{
		PORTA |= (1<<PA7);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		PORTA &= ~(1<<PA7);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
		_delay_ms(0xFF);
	}
} 
