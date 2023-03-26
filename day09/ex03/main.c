/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/24 11:36:14 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ft_usart.h"
#include "ft_i2c.h"

#define TOP 0x01
#define TOP_RIGHT 0x02
#define BOTTOM_RIGHT 0x04
#define BOTTOM 0x08
#define BOTTOM_LEFT 0x10
#define TOP_LEFT 0x20
#define MIDDLE 0x40
#define DOT 0x80

uint8_t state = 0;

void	FT_printf(uint8_t n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
}

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

void button_init(void)
{
	//EICRA |= 0b10;				/* Enable button */
	//EIMSK |= 1;
	//PCMSK2 |= (1 << PCINT20);
    //PCICR |= (1 << PCIE2);
}

uint8_t	get_digit(uint8_t digit)
{
	return ((0xFF - (1 << (digit + 3))));
}

uint8_t	get_value(uint8_t digit)
{
	if (digit == 0 || digit == '0')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT + TOP_LEFT);
	if (digit == 1 || digit == '1')
		return (0x00 + TOP_RIGHT + BOTTOM_RIGHT);
	if (digit == '2')
		return (0x00 + TOP + TOP_RIGHT + MIDDLE + BOTTOM_LEFT + BOTTOM + DOT);
	if (digit == 2 || digit == '2')
		return (0x00 + TOP + TOP_RIGHT + MIDDLE + BOTTOM_LEFT + BOTTOM);
	if (digit == 3 || digit == '3')
		return (0x00 + TOP + TOP_RIGHT + MIDDLE + BOTTOM_RIGHT + BOTTOM);
	if (digit == 4 || digit == '4')
		return (0x00 + TOP_LEFT + MIDDLE + TOP_RIGHT + BOTTOM_RIGHT);
	if (digit == 5 || digit == '5')
		return (0x00 + TOP + TOP_LEFT + MIDDLE + BOTTOM_RIGHT + BOTTOM);
	if (digit == 6 || digit == '6')
		return (0x00 + TOP + TOP_LEFT + MIDDLE + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT);
	if (digit == 7 || digit == '7')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT);
	if (digit == 8 || digit == '8')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT + TOP_LEFT + MIDDLE);
	if (digit == 9 || digit == '9')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + TOP_LEFT + MIDDLE);
	if ((digit | 32) == 'a')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT + TOP_LEFT + MIDDLE + BOTTOM_LEFT);
	if ((digit | 32) == 'b')
		return (0x00 + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT + MIDDLE + TOP_LEFT);
	if ((digit | 32) == 'c')
		return (0x00 + TOP + TOP_LEFT + BOTTOM_LEFT + BOTTOM);
	if ((digit | 32) == 'd')
		return (0x00 + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT + MIDDLE);
	if ((digit | 32) == 'e')
		return (0x00 + TOP + BOTTOM + BOTTOM_LEFT + TOP_LEFT + MIDDLE);
	if ((digit | 32) == 'f')
		return (0x00 + TOP + TOP_LEFT + MIDDLE + BOTTOM_LEFT);
	if ((digit | 32) == 'g')
		return (0x00 + TOP + TOP_LEFT + BOTTOM_LEFT + BOTTOM + BOTTOM_RIGHT);
	if ((digit | 32) == 'h')
		return (0x00 + TOP_LEFT + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM_LEFT + MIDDLE);
	if ((digit | 32) == 'i')
		return (0x00 + TOP_LEFT + BOTTOM_LEFT);
	if ((digit | 32) == 'j')
		return (0x00 + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM);
	if ((digit | 32) == 'k')
		return (0x00 + TOP_LEFT + BOTTOM_RIGHT + BOTTOM_LEFT + MIDDLE);
	if ((digit | 32) == 'l')
		return (0x00 + TOP_LEFT + BOTTOM_LEFT + BOTTOM);
	if ((digit | 32) == 'm')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM_LEFT + TOP_LEFT);
	if ((digit | 32) == 'n')
		return (0x00 + MIDDLE + BOTTOM_RIGHT + BOTTOM_LEFT);
	if ((digit | 32) == 'o')
		return (0x00 + BOTTOM + BOTTOM_RIGHT + MIDDLE + BOTTOM_LEFT);
	if ((digit | 32) == 'p')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_LEFT + TOP_LEFT + MIDDLE);
	if ((digit | 32) == 'q')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT + TOP_LEFT + MIDDLE);
	if ((digit | 32) == 'r')
		return (0x00 + BOTTOM_LEFT + MIDDLE);
	if ((digit | 32) == 's')
		return (0x00 + TOP + TOP_LEFT + MIDDLE + BOTTOM_RIGHT + BOTTOM);
	if ((digit | 32) == 't')
		return (0x00 + MIDDLE + BOTTOM + BOTTOM_LEFT + TOP_LEFT);
	if ((digit | 32) == 'u')
		return (0x00 + TOP_LEFT + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT);
	if ((digit | 32) == 'v')
		return (0x00 + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT);
	if ((digit | 32) == 'w')
		return (0x00 + TOP_LEFT + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT + MIDDLE);
	if ((digit | 32) == 'x')
		return (0x00 + MIDDLE + TOP + BOTTOM_LEFT + BOTTOM_RIGHT);
	if ((digit | 32) == 'y')
		return (0x00 + TOP_LEFT + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + MIDDLE);
	if ((digit | 32) == 'z')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_LEFT + BOTTOM);
	if (digit == '-')
		return (0x00 + MIDDLE);
	if (digit == '_')
		return (0x00 + BOTTOM);
	return (0x00);
}
		
void	print_digit(uint8_t value, uint8_t digit)
{
	i2c_clock_connect();
	i2c_write(digit);
	i2c_write(value);
	i2c_write(digit);
	i2c_write(0x00);
}

void	print_digit_one_time(uint8_t value, uint8_t digit)
{
	i2c_clock_connect();
	i2c_write(digit);
	i2c_write(value);
}

void	print_nbr(uint16_t nbr)
{
	print_digit(get_value((nbr / 1000) % 10), get_digit(1));
	print_digit(get_value((nbr / 100) % 10), get_digit(2));
	print_digit(get_value((nbr / 10) % 10), get_digit(3));
	print_digit(get_value((nbr) % 10), get_digit(4));
}

int	display_str(uint8_t *buf, uint8_t new_char, uint8_t speed)
{
	static uint16_t speedRate;
	int	next = 0;

	if (speedRate > (uint8_t)(255 - speed))
	{
		buf[0] = buf[1];
		buf[1] = buf[2];
		buf[2] = buf[3];
		buf[3] = new_char;
		speedRate = 0;
		next = 1;
	}
	else
		speedRate++;
	print_digit(get_value(buf[3]), get_digit(4));
	print_digit(get_value(buf[2]), get_digit(3));
	print_digit(get_value(buf[1]), get_digit(2));
	print_digit(get_value(buf[0]), get_digit(1));
	return (next);
}

void	print_str(uint8_t *str, uint16_t speed)
{
	uint8_t	i = 0;
	uint8_t buf[4] = {-1, -1, -1, -1};

	while (str[i])
	{
		if (display_str(buf, str[i], speed))
			i++;
	}
	while (buf[0] != (uint8_t)-1)
	{
		display_str(buf, -1, speed);
	}
}

ISR(TIMER1_COMPA_vect)
{
	cli();
	print_digit_one_time(get_value(state), get_digit(4));
	state = (state + 1) % 10;
	sei();
}

void	timer1_init()
{
	TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 0b0011110100001001;//0001111010000101; //Set clock to match 7813 31250 = 0b
	
	//How to use Interrupts on the timer
	TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	TCCR1B |= (1 << WGM12); //Set to CTC mode
	TIMSK1 |= (1 << OCIE1A);
}


int	main(void)
{
	usart_init();
	i2c_init();
	//button_init();
	sei();

	usart_printstr("Hello World\r\n");

	i2c_clock_init();
	i2c_clock_clear();

	i2c_clock_connect();
	timer1_init();

		//i2c_write(0x00);

		//i2c_write(0xFF);

		//_delay_ms(500);
		//print_digit_one_time(get_value('2'), get_digit(4));
	while (1)
	{
		//_delay_ms(1000);
	}
}
