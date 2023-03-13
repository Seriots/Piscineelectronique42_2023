#include <avr/io.h>

int	main()
{
	PORTB &= ~(1 << PB0);
	DDRB |= (1<<PB0);
	while(1)
	{
		PORTB ^= (1<<PB0);
		for (uint32_t i = 0; i <= 242424UL; i++) ; //(8000000 - 24) / 33
	}
	return (0);
}

//ldd 8 * 2, adiw 1 * 2, adc = 2 * 1, std = 4 * 2, cpi = 1*1, sbci = 2 * 1, cpc = 1, brlo = 1/2
//16 + 2 + 2 + 8 + 1 + 2 + 1 + 1/2 = 32.5  + 0.5 


//2 + 4 + 1 + 2 + 1 + 1 + 1 + 2 + 8 + 2 = 24
//OPTI
//in = 1 * 1, eor = 1 * 1, ldi = 4 * 1, sbiw = 2, sbc = 2 * 1, brne = 1/2, rjmp = 2
//4 op dans la boucle
//1 + 1 + 1 + 4