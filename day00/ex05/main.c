#include <avr/io.h>
#include <util/delay.h>

int	FT_printf(int n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
}

int	main()
{
	PORTB &= ~(1 << PB0);
	//DDRB |= (1<<PB0);
	//DDRB |= (1<<PB1);
	//DDRB |= (1<<PB2);
	//DDRB |= (1<<PB4);
	DDRD |= (0<<PD2);
	DDRD |= (0<<PD4);
	long prestate1 = (PIND & 1<<PD2);
	long prestate2 = (PIND & 1<<PD4);
	long n = 0;
	while(1)
	{
		if (prestate1 != (PIND & 1<<PD2))
		{
			if (prestate1 == 0)
			{
				n++;
				_delay_ms(2);
			}
			prestate1 = (PIND & 1<<PD2);
		}
		if (prestate2 != (PIND & 1<<PD4))
		{
			if (prestate2 == 0)
			{
				n--;
				_delay_ms(2);
			}
			prestate2 = (PIND & 1<<PD4);
		}
		FT_printf(n);
	}
	return (0);
}