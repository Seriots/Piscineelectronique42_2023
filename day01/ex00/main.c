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
	PORTB &= ~(1 << PB1); //Unset PB1

	DDRB = 0b00011111; //SET PB0, PB1, PB2, PB4 as output
	TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 0b0001111010000101; //Set clock to match 7813
	
	//How to use Interrupts on the timer
	TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	TCCR1B |= (1 << WGM12); //Set to CTC mode




	while(1)
	{
	}
	return (0);
}