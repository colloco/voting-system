#include <avr/io.h>
#include <stdio.h> //sprintf
#include <avr/interrupt.h>

#include "global.h"
#include "rfm12.h"
#include "tastenmatrix.h"
#include "lcd.h"

#define RLED_ON PORTB |= (1<<PB1)
#define RLED_OFF PORTB &= ~(1<<PB1)


uint8_t digit = 222; // vorbelegen mit cancel damit nichts im LCD erscheint
uint8_t lastDigit = 222;
uint8_t num = 0;

void send(uint8_t num);
//void receive(void);
void flashLED(void);

int main(void)
{
	uint8_t teststring[] = "teststring\r\n";
	uint8_t packettype = 0xEE;
	rfm12_init();  /* initialize the library */
	sei();

  
	DDRB |= (1<<PB1); // output rote LED
	DDRB |= (1<<PB0); // output gruene LED
	PORTB |= (1<<PB0); // gruene LED an
	
	initKeyboard();
	initLCD();

	// Timer 0 konfigurieren
	TCCR0 = (1<<CS02); // Prescaler 256

	// Overflow Interrupt erlauben
	TIMSK |= (1<<TOIE0);

	// Global Interrupts aktivieren
	sei();

	for (;;)
	{
		rfm12_tx (sizeof(teststring), packettype, teststring);
		rfm12_tick();  /* periodic tick function - call that one once in a while */

		splitNum(num);
		if (digit != lastDigit) 
		{
			if (digit == 222) // 222 entspricht cancel
			{
				num = 0;
			} else if (digit == 111) { // 111 entspricht ok
				RLED_ON;
				cli(); // Interrupts abschalten
	//			send(num);
				sei(); // Interrupts wieder einschalten 
				num = 111;
				RLED_OFF;
			} else if (num < 10 && digit < 10) { // es wurde bisher nur eine Taste gedrückt
				num = num*10 + digit;
			} else if (lastDigit > 100 && digit < 10) { // eine Ziffer wurde gedrückt
				num = digit;
			}

			lastDigit = digit;
		}
	}
}

/* Der Overflow Interrupt Handler wird aufgerufen, wenn TCNT0 von 255 auf 0 wechselt (256 Schritte), d.h. ca. alle 2 ms */
ISR (TIMER0_OVF_vect)
{
	/* Interrupt Aktion alle (8000000/256)/256 Hz = 122,0703125 Hz bzw. 1/122,0703125 s = 8,192 ms */
	digit = getDigit();
	toggleLCD ^= 1;
}

void send(uint8_t num)
{
	unsigned char test[]="Dies ist ein 433MHz Test !!! \r\n";
//	sprintf(test,"%d                           \r\n",num);
//	rf12_txdata(test,32);
}
