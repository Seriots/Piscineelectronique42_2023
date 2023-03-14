#include <avr/io.h>

	//CS12 and CS10 is 1024
	//CS12 and CS11 is 256
	//CS12 and CS10 and CS11 is 1024
	//CS12 and CS11 is 256
	//CS12 is 64
	//CS11 is 8
	//CS10 is 1
int	FT_printf(int n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);

}


int	main()
{
	PORTB &= ~(1 << PB0); //Unset PB0
	PORTB &= ~(1 << PB1); //Unset PB1
	PORTB &= ~(1 << PB2); //Unset PB12

	DDRB = 0b00000110; //SET PB0, PB1, PB2, PB4 as output
	TCNT1 = 0; //Clear the timer

	TCCR1B |= (1 << CS12); //set timing to 1024
	OCR1A = 3125; //Set match to 781.3 15625 * 4 = 62500 / 2
	OCR1B = 9375; //Set match to 781.3 15625 * 4 = 62500 / 2
	ICPR1 = 31250; //Set top to 7813
	ICR1 = 31250; //Set top to 7813
	
	TCCR1B |= (1 << WGM13); //Set to Pwn with top=IRC1 and match OCR1A
	TCCR1A |= (1 << COM1A1); //Set to 0 on upcounting and 1 on downcounting mode	
	TCCR1A |= (1 << COM1B1); //Set to 0 on upcounting and 1 on downcounting mode	


	while(1)
	{
	}
	return (0);
}