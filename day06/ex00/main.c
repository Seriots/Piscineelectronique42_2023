/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/21 11:35:01 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ft_usart.h"

#define FREQ 15625; 

uint8_t state = 0;


void	next_step()
{
	PORTD &= ~(1 << PD6 | 1 << PD5 | 1 << PD3);
	if (state == 0)
		PORTD |= 1 << PD5;
	else if (state == 1)
		PORTD |= 1 << PD6;
	else if (state == 2)
		PORTD |= 1 << PD3;
	state = (state + 1) % 3;
}


ISR(TIMER1_COMPA_vect)
{
	cli();
	next_step();
	sei();
}


int	main()
{
	usart_init();
	DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6;

	sei(); //Enable global interrupt

	TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 0b0011110100001001; //Set clock to match 7813 31250 = 0b
	
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

