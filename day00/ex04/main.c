#include <avr/io.h>
#include <util/delay.h>


int	main()
{
	PORTB &= ~(1 << PB0);
	DDRB |= (1<<PB0);
	DDRB |= (0<<PD2);
	long prestate = (PIND & 1<<PD2);
	while(1)
	{
		if (prestate != (PIND & 1<<PD2))
		{
			if (prestate == 0)
			{
				PORTB ^= (1<<PB0);
				_delay_ms(2);
			}
			prestate = (PIND & 1<<PD2);
		}
	}
	return (0);
}