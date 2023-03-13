#include <avr/io.h>
#include <util/delay.h>


int	main()
{
	PORTB &= ~(1 << PB0);
	DDRB |= (1<<PB0);
	DDRB |= (0<<PD2);
	while(1)
	{
		if ((PIND & 1<<PD2))
			PORTB = 0;
		else
			PORTB = (1<<PB0);
	}
	return (0);
}