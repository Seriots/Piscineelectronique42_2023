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
uint16_t n = 0;
uint8_t x = 0;
int	FT_printf(uint16_t n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
	//PORTB &= ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB4);
	//PORTB |= (n<<1 & 1<<PB1) | (n<<1 & 1<<PB2) | (n<<2 & 1<<PB4);

}

ISR(INT0_vect) {
	cli(); //Disable global interrupt
	n++;
	FT_printf(n);
	_delay_ms(1);
	EIFR |= (1 << INTF0); //Clear interrupt flag
	sei();
 // réactiver les interruptions
}

ISR(PCINT2_vect) {
	cli(); //Disable global interrupt
	if (x == 0)
	{
		n--;
		FT_printf(n);
		_delay_ms(1);
		//EIFR |= (1 << INTF1); //Clear interrupt flag
		x = 1;
	}
	else
		x = 0;
	sei();
 // réactiver les interruptions
}


void main() {
	// Configuration de la broche 2 comme entrée
	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD4);
	DDRB |= 1 << PB1 | 1 << PB2 | 1 << PB0 | 1 << PB4; //SET PB0, PB1, PB2, PB4 as output


	// Activation de l'interruption externe 0 pour la broche 2
	EICRA |= (1 << ISC01); //set le mode p80
	EIMSK |= (1 << INT0); //Enable INT0

    PCMSK2 |= (1 << PCINT20);
    // Activation des interruptions sur changement de niveau pour les broches sélectionnées
    PCICR |= (1 << PCIE2);

	sei(); //Enable global interrupt

	for(;;) {
	}
}