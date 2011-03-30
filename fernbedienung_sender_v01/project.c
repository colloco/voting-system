#include <avr/io.h>
#include <avr/interrupt.h>

#include "led.h"
#include "tastenmatrix.h"

uint8_t digit = 222; // vorbelegen mit cancel damit nichts im LCD erscheint
uint8_t lastDigit = 222;
uint8_t num = 0;

void send(uint8_t num);

int main(void)
{
	initLed();
	initKeyboard();
	

	// Timer 0 konfigurieren
	TCCR0 = (1<<CS02); // Prescaler 256

	// Overflow Interrupt erlauben
	TIMSK |= (1<<TOIE0);

	// Global Interrupts aktivieren
	sei();
	for (;;)
	{
		if (digit != lastDigit) {
			LedOn(0);
		}
	}
}

/* Der Overflow Interrupt Handler wird aufgerufen, wenn TCNT0 von 255 auf 0 wechselt (256 Schritte), d.h. ca. alle 2 ms */
ISR (TIMER0_OVF_vect)
{
	/* Interrupt Aktion alle (8000000/256)/256 Hz = 122,0703125 Hz bzw. 1/122,0703125 s = 8,192 ms */
	digit = getDigit();
	//toggleLCD ^= 1;
}
