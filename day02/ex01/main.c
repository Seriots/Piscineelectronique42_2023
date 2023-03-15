#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

	//CS12 and CS10 is 1024
	//CS12 and CS11 is 256
	//CS12 and CS10 and CS11 is 1024
	//CS12 and CS11 is 256
	//CS12 is 64
	//CS11 is 8
	//CS10 is 1
int	FT_printf(long n)
{
	PORTB = (n<<4 & 1<<PB4);
}
	//if (OCR1A + incr > FREQ)
	//{
	//	sens = 2;
	//	OCR1A -= (OCR1A + incr - FREQ);

	//}

	//else if (OCR1A < incr && sens == 2)
	//{
	//	OCR1A -= (incr - OCR1A);
	//	sens = 1;
	//}
	//else if (sens == 1)
	//	OCR1A+=incr;
	//else if (sens == 2)
	//	OCR1A-=incr;

uint16_t FREQ = 15625; 
uint16_t incr = 500;
uint8_t sens = 1;

ISR(TIMER0_COMPA_vect)
{
	cli();
	if (sens) {
		OCR1A += incr;
		if (OCR1A >= FREQ) {
			sens = 0;
		}
	} else {
		OCR1A -= incr;
		if (OCR1A == 0) {
			sens = 1;
		}
	}
	sei();
}


int	main()
{
	PORTB &= ~(1 << PB1); //Unset PB1
	DDRB |= 1 << PB1 | 1 << PB2 | 1 << PB0 | 1 << PB4; //SET PB0, PB1, PB2, PB4 as output

	sei(); //Enable global interrupt

	TCNT1 = 0; //Clear the timer
	TCCR1B |= (1 << CS11) ;//| (1 << CS10); //set timing to 16000000/(8*FREQ(15625)) = 128Hz
	OCR1A = 0;
	ICR1 = FREQ; 
	TCCR1B |= (1 << WGM13); //Set to Pwn with top=IRC1 and match OCR1A
	TCCR1A |= (1 << COM1A1); //Set to 0 on upcounting and 1 on downcounting mode	
	
	
	TCNT0 = 0; //Clear the timer
	TCCR0B |= (1 << CS02) | (1 << CS00); //set timing to 1024
	OCR0A = 250; //set frequence on 62.5Hz
	TCCR0A |= (1 << WGM01);
	TIMSK0 |= (1 << OCIE0A);

	for(;;) {
		//FT_printf(sens);
	}
	return (0);
}