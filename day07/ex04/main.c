/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/22 13:02:17 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ft_usart.h"

// Fonction d'initialisation de l'ADC
void adc_init() {
	// Configurer la référence ADC sur AVCC
	ADMUX |= (1 << REFS0);// | (1 << ADLAR);

	// Configurer la résolution ADC à 8 bits
	ADCSRA |=  (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	// Configurer la broche d'entrée ADC sur PC0
	//ADMUX |= (1 << MUX1);
	ADMUX &= ~(1 << MUX3);
	ADMUX &= ~(1 << MUX2);
	ADMUX &= ~(1 << MUX1);
	ADMUX &= ~(1 << MUX0);			

	// Activer l'ADC
	ADCSRA |= (1 << ADEN) | (1 << ADATE);
	//ADCSRB |= (1 << ADTS0);// | (1 << ADTS1) | (1 << ADTS2);

	// Démarrer une conversion ADC
	//ADCSRA |= (1 << ADSC);

	//// Attendre la fin de la conversion
	//while (ADCSRA & (1 << ADSC));
	ADCSRA |= (1 << ADSC);
}

void	FT_printf(long n)
{
	PORTB &= ~(1 << PB0) & ~(1 << PB1) & ~(1 << PB2) & ~(1 << PB4);
	switch (n)
	{
		case 4:
			PORTB |= (1 << PB4);
		case 3:
			PORTB |= (1 << PB2);
		case 2:
			PORTB |= (1 << PB1);
		case 1:
			PORTB |= (1 << PB0);
	}

}

void	init_rgb()
{
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Fast PWM mode, non-inverting output on OC0B (PD5)
    TCCR0B |= (1 << CS01);  // Prescaler = 8

    TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // Fast PWM mode, non-inverting output on OC2B (PD3)
    TCCR2B |= (1 << CS21);  // Prescaler = 8
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

// Lire la valeur ADC
//uint8_t adc_value = ADCH;

int	main()
{	
	uint16_t value;
	usart_init();
	adc_init();
	init_rgb();

	//DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4;
	DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6;

	for(;;) {
		
	   	value = ADCL;
		value = (value) | (((uint16_t)ADCH) << 8);
		wheel(value / 4);
		FT_printf((value + 1) / 256);
        _delay_ms(20);
	}
	return (0);
}

