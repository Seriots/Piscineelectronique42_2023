#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//PD2 = SW1
// INT0, PCINT18
// INT0 -> EIMSK(0), 81
// PCINT18 -> PCMSK2(2), 73
// DIEOE= signal name
// DIEOV= Flag to press/unpress
// ISC00 -> EICRA(0)
int	FT_printf(long n)
{
	//PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
	PORTB &= ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB4);
	PORTB |= (n<<1 & 1<<PB1) | (n<<1 & 1<<PB2) | (n<<2 & 1<<PB4);

}

ISR(INT0_vect) {
	cli(); //Disable global interrupt
	PORTB ^= (1 << PB0);
	EIFR |= (1 << INTF0); //Clear interrupt flag
	_delay_ms(30);
	sei();
 // réactiver les interruptions
}


void main() {
	// Configuration de la broche 2 comme entrée
	DDRD &= ~(1 << PD2);
	DDRB |= 1 << PB0;

	// Activation de l'interruption externe 0 pour la broche 2
	EICRA |= (1 << ISC01); //set le mode p80
	EIMSK |= (1 << INT0); //Enable INT0

	sei(); //Enable global interrupt
	//SREG &= ~(1 << 7); //Disable global interrupt
	for(;;) {
	}
}