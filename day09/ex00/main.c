/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/24 10:58:02 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ft_usart.h"
#include "ft_i2c.h"

uint8_t state = 0;

void	i2c_clock_clear(void)
{
	i2c_start();
	i2c_start_send((0b0100000 << 1) | 0);
	i2c_write(0b010);
	i2c_write(0x0F);
	i2c_write(0x00);
	i2c_stop();
}

void	i2c_clock_init(void)
{
	i2c_start();
	i2c_start_send((0b0100000 << 1) | 0);
	i2c_write(0b110);
	i2c_write(0b00000000);
	i2c_write(0b00000000);
	i2c_stop();
}

void	i2c_clock_connect(void)
{
	i2c_start();
	i2c_start_send((0b0100000 << 1) | 0);
	i2c_write(0b010);
}

ISR(TIMER1_COMPA_vect)
{
	cli();
	i2c_clock_connect();
	if (state == 0)
		i2c_write(0x07);
	else
		i2c_write(0x0F);
	i2c_write(0x00);
	i2c_stop();
	state = !state;
	sei();
}

void	timer1_init()
{
	TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 0b0001111010000101; //Set clock to match 7813 31250 = 0b
	
	//How to use Interrupts on the timer
	TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	TCCR1B |= (1 << WGM12); //Set to CTC mode
	TIMSK1 |= (1 << OCIE1A);
}

int	main()
{	
	usart_init();
	i2c_init();
	//button_init();
	timer1_init();
	sei(); //Enable global interrupt

	usart_printstr("Hello World\r\n");

	i2c_clock_init();
	i2c_clock_clear();


	for(;;) {
		

	}
	return (0);
}

