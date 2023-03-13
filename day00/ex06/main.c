#include <avr/io.h>
#include <util/delay.h>

#define P(x) (1 << x)

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

	int		sens = 0;
	uint8_t	n = 1;
	while(1)
	{
		if (sens == 0)
		{
			n *= 2;
			if (n == 8)
				sens = 1;
		}
		else
		{
			n /= 2;
			if (n == 1)
				sens = 0;
		}
		FT_printf(n);
		_delay_ms(120);
	}
	return (0);
}