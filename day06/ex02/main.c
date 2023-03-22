/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/21 11:32:14 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ft_usart.h"

#define FREQ 15625; 

uint8_t pos = 0;

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

//void	next_step()
//{
//	PORTD &= ~(1 << PD6 | 1 << PD5 | 1 << PD3);
	
//	if (state & 1)
//		PORTD |= 1 << PD5;
//	if (state & 2)
//		PORTD |= 1 << PD6;
//	if (state & 4)
//		PORTD |= 1 << PD3;
//	state = ((state + 1) % 7) + 1;
//}


ISR(TIMER1_COMPA_vect)
{
	cli();
	wheel(pos);
	pos = (pos + 1) % 255;
	sei();
}

int	main()
{
	usart_init();
	init_rgb();
	DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6;

	sei(); //Enable global interrupt

	TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 0x00FF; //Set clock to match 15625
	
	//How to use Interrupts on the timer
	TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	TCCR1B |= (1 << WGM12); //Set to CTC mode
	TIMSK1 |= (1 << OCIE1A);

	//TCNT0 = 0; //Clear the timer
	//TCCR0B |= (1 << CS02) | (1 << CS00); //set timing to 1024
	//OCR0A = 250; //set frequence on 62.5Hz
	//TCCR0A |= (1 << WGM01);
	//TIMSK0 |= (1 << OCIE0A);

		//TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	//TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	//OCR1A = 0b0001111010000101; //Set clock to match 7813
	
	////How to use Interrupts on the timer
	//TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	//TCCR1B |= (1 << WGM12); //Set to CTC mode
	//TIMSK1 |= (1 << OCIE1A);


	for(;;) {
		//FT_printf(sens);
	}
	return (0);
}

