/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/26 15:45:54 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "ft_usart.h"
#include "ft_i2c.h"
#include "ft_display.h"
#include "ft_spi.h"

#define MAXMODE 12

typedef struct s_msg {
	uint8_t msg[254];
	uint8_t size;
	uint8_t speed;
} t_msg;	

void	show_mode(uint8_t n);

uint8_t start_count = 0;
uint8_t hour = 0;
uint8_t button[3] = { 0, 0, 0 };
uint8_t msg_buffer[4] = { -1, -1, -1, -1 };
uint8_t mode = 0;
uint32_t info = 0;
uint8_t color = 0;

ISR(TIMER1_COMPA_vect)
{
	start_count += 1;
	hour = !hour;

	if (mode == 4) {
		PORTD &= ~(1 << PD3) & ~(1 << PD5) & ~(1 << PD6);
		boudary_frame(0);
		switch (color) {
			case 2:
				transmitLed(0xE1FF0000);
				transmitLed(0xE1FF0000);
				transmitLed(0xE1FF0000);

				PORTD |= (1 << PD3);
				break;
			case 1:
				transmitLed(0xE10000FF);
				transmitLed(0xE10000FF);
				transmitLed(0xE10000FF);
				PORTD |= (1 << PD5);
				break;
			case 0:
				transmitLed(0xE100FF00);
				transmitLed(0xE100FF00);
				transmitLed(0xE100FF00);
				PORTD |= (1 << PD6);
				break;
		}
		boudary_frame(1);
		color = (color + 1) % 3;
	}
}

void	reset_rgb()
{
	boudary_frame(0);
	transmitLed(0xE0000000);
	transmitLed(0xE0000000);
	transmitLed(0xE0000000);
	PORTD &= ~(1 << PD6) & ~(1 << PD3) & ~(1 << PD5);
	boudary_frame(1);
}

void	increment()
{
	reset_rgb();
	mode = (mode + 1) % MAXMODE;
	show_mode(mode);
}

void	decrement()
{
	reset_rgb();
	if (mode == 0)
		mode = MAXMODE - 1;
	else
		mode--;
	show_mode(mode);
}

void	show_mode(uint8_t n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
}

void	timer1_init()
{
	TCNT1 = 0; //Clear the timer

	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 0b0011110100001001;//0001111010000101; //Set clock to match 7813 31250 = 0b
	
	TCCR1B |= (1 << WGM12); //Set to CTC mode
	TIMSK1 |= (1 << OCIE1A);
}

void button_init(void)
{
	DDRD &= ~(1 << PD2) & ~(1 << PD4); /* SW1 */
	/* SW1 */
	EICRA |= 0b10;				/* Enable button */
	EIMSK |= 1;
	
	/* SW2 */
	PCMSK2 |= (1 << PCINT20);
    PCICR |= (1 << PCIE2);
}

void adc_init() {
	// Configurer la référence ADC sur AVCC
	ADMUX |= (1 << REFS0);// | (1 << ADLAR);

	// Configurer la résolution ADC à 8 bits
	ADCSRA |=  (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	// Configurer la broche d'entrée ADC sur PC0
	//ADMUX |= (1 << MUX1);

	// Activer l'ADC
	ADCSRA |= (1 << ADEN) | (1 << ADATE);
	//ADCSRB |= (1 << ADTS0);// | (1 << ADTS1) | (1 << ADTS2);

	// Démarrer une conversion ADC
	//ADCSRA |= (1 << ADSC);

	//// Attendre la fin de la conversion
	//while (ADCSRA & (1 << ADSC));
	ADCSRA |= (1 << ADSC);
}


void	start()
{
	reset_rgb();
	while (start_count < 4)
	{
		i2c_clock_connect();
		if (start_count < 3)
		{
			i2c_clock_set_all(0xFF);	
			PORTB = 0b00010111;
		}
		else
		{
			i2c_clock_set_all(0x00);	
			PORTB = 0b00000000;
		}
		i2c_stop();
		_delay_ms(20);
	}
	//TIMSK1 &= ~(1 << OCIE1A);
}

void	update_sw_1_2()
{
	if ((PIND & 1<<PD2))
	{
		if (button[0] == 0)
			increment();
		button[0] = 1;
	}
	else
		button[0] = 0;
	if ((PIND & 1<<PD4))
	{
		if (button[1] == 0)
			decrement();
		button[1] = 1;
	}
	else
		button[1] = 0;
}

void	update_sw3()
{
	i2c_start();
	i2c_start_send((0b0100000 << 1) | 0);
	i2c_write(0b00000000);
	i2c_stop();
	i2c_start();
	i2c_start_recv((0b0100000 << 1) | 1);
	
	i2c_read(0, &button[2]);
	i2c_stop();
	button[2] &= 0x01;
	_delay_ms(1);
}

void	print_hour(uint16_t nbr, uint8_t led)
{
	print_digit(get_value((nbr / 1000) % 10), get_digit(1) | led);
	print_digit(get_value((nbr / 100) % 10) + DOT * hour, get_digit(2) | led);
	print_digit(get_value((nbr / 10) % 10), get_digit(3) | led);
	print_digit(get_value((nbr) % 10) + DOT * hour, get_digit(4) | led);
}

void	display_info()
{
	uint8_t led = 0x00 | button[0] << 3 | button[1] << 2 | button[2] << 1;
	switch (mode) {
		case 0:
		case 1:
		case 2:
			print_nbr(info, led);
			break;
		case 3:
			print_tmcu(info, led);
			break;
		case 4:
		
			print_42(led);
			break;
		case 5:
			print_text(info, led, 'C');
			break;
		case 6:
			print_text(info, led, 'F');
			break;
		case 7:
			print_text(info, led, 'H');
			break;
		case 8:
			print_nbr(info, led);
			break;
		case 9:
			print_date(info, led);
			break;
		case 10:
			print_hour(info, led);
			break;
		case 11:
			print_msg(msg_buffer, led);
			break;
		default:
			i2c_clock_connect();
			i2c_write(led | 0xF0);
			i2c_write(0x80);
			i2c_stop();
			_delay_ms(1);
			break;
	}
}

void	set_rv1()
{
	uint16_t state;

	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3);  

	state = ADCL;
	state = (state) | (((uint16_t)ADCH) << 8);
	info = state;
}

void	set_ldr()
{
	uint16_t state;

	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3);  
	ADMUX |= (1 << MUX0);

	state = ADCL;
	state = (state) | (((uint16_t)ADCH) << 8);
	info = state;
}

void	set_ntc()
{
	uint16_t state;

	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3);  
	ADMUX |= (1 << MUX1);

	state = ADCL;
	state = (state) | (((uint16_t)ADCH) << 8);
	info = state;
}

void	set_tmcu()
{
	uint16_t state;

	ADMUX |= (1 << REFS0) | (1 << REFS1);
	ADMUX &= ~(1 << MUX0) & ~(1 << MUX1) & ~(1 << MUX2) & ~(1 << MUX3);  
	ADMUX |= (1 << MUX3);

	state = ADCL;
	state = (state) | (((uint16_t)ADCH) << 8);
	info = (state - 342) + 25;
}

void	busy_delay(uint16_t delay, uint8_t unite)
{
	uint16_t i = 0;

	while (i < delay)
	{
		print_text(info, 0x00 | button[0] << 3 | button[1] << 2 | button[2] << 1, unite);
		i+=4;
	}
}

void	set_temp(uint8_t unite)
{
	uint8_t TMP;
	uint32_t BUFFER[6];
	uint32_t T;
	i2c_start();
	i2c_start_send(0x38<<1 | 0);
	i2c_write(0xAC); // trigger measurement
	i2c_write(0x33);
	i2c_write(0x00);

	i2c_stop();
	busy_delay(100, unite);
	do
	{
		i2c_start();
		i2c_start_recv(0x38<<1 | 1);
		i2c_read(1, &TMP);
		if ((TMP & 1<<7) == 0)
			break ;
		i2c_stop();
		busy_delay(100, unite);
	}
	while ((TMP & 1<<7) != 0);

	BUFFER[0] = TMP;
	//usart_dump(TMP);
	//usart_printstr(" ");
	for (uint8_t j = 1; j < 6; j++)
	{
		i2c_read((j==5 ? 0 : 1), &TMP);
		BUFFER[j] = TMP;
		//usart_dump(TMP);
		//usart_printstr(" ");
	}
	//usart_printstr(" \r\n");

	i2c_stop();

	//char buf[64];
	int32_t Srh = BUFFER[1] << 12 | BUFFER[2] << 4 | ((BUFFER[3] >> 4) & 0xf);
	int32_t St = (((uint32_t)BUFFER[3] & 0b1111)) << 16 | BUFFER[4] << 8 | BUFFER[5];


	if (unite == 'C')
		T = ((double)St / (0x100000UL) * 200.0 - 50.0) * 10;
	else if (unite == 'F')
		T = (((double)St / (0x100000UL) * 200.0 - 50.0) * 1.8 + 32) * 10;
	else
		T = (double)Srh / (0x100000UL) * 100 * 10;
	//sprintf(buf, "%ld\r\n", T);
	//usart_printstr(buf);
	info = T;
}

void	set_date(uint8_t unite)
{
	uint8_t time[7];

	(void)unite;
	i2c_start();
	i2c_start_send(0x51<<1 | 0);
	i2c_write(0x02);
	i2c_stop();
	i2c_start();
	i2c_start_recv(0x51<<1 | 1);
	//usart_printstr("date: \r\n");
	for (uint8_t i = 0; i < 7; i++)
	{
		i2c_read((i==6 ? 0 : 1), &time[i]);
		//usart_dumpln("second: ", time[i], "\r\n");
	}
	i2c_stop();
	if (unite == 'Y')
		info = 2000 + ((time[6]>>4) & 0x0f) * 10 + (time[6] & 0x0f);
	else if (unite == 'M')
		info = ((time[5]>>4) & 0x01) * 10 + (time[5] & 0x0f) + ((time[3]>>4) & 0x03) * 1000 + ((time[3]) & 0x0f) * 100;
	else if (unite == 'H')
		info = ((time[2]>>4) & 0x03) * 1000 + (time[2] & 0x0f) * 100 + ((time[1]>>4) & 0x07) * 10 + (time[1] & 0x0f);
}

t_msg	init_msg(char *buffer, uint8_t speed, uint8_t read)
{
	static t_msg msg;
	if (!read)
	{
		msg.speed = speed;
		msg.size = 0;
		while (buffer[msg.size] && msg.size < 254)
		{
			msg.msg[msg.size] = buffer[msg.size];
			msg.size++;
		}
		msg.msg[msg.size] = 0;
	}
	return (msg);
}

void	set_msg()
{
	static uint8_t i;
	static uint8_t speedRate;
	t_msg msg = init_msg(0, 0, 1);
	
	if (speedRate > (uint8_t)(254 - msg.speed))
	{
		if (i < msg.size)
		{
			msg_buffer[0] = msg_buffer[1];
			msg_buffer[1] = msg_buffer[2];
			msg_buffer[2] = msg_buffer[3];
			msg_buffer[3] = msg.msg[i];
			i++;
		}
		else if (msg_buffer[3] != 0xFF || msg_buffer[2] != 0xFF || msg_buffer[1] != 0xFF || msg_buffer[0] != 0xFF)
		{
			msg_buffer[0] = msg_buffer[1];
			msg_buffer[1] = msg_buffer[2];
			msg_buffer[2] = msg_buffer[3];
			msg_buffer[3] = 0xFF;
		}
		else
			i = 0;
		speedRate = 0;
	}
	else
		speedRate ++;
}

void	set_info()
{
	switch (mode) {
		case 0:
			set_rv1();
			break;
		case 1:
			set_ldr();
			break;
		case 2:
			set_ntc();
			break;
		case 3:
			set_tmcu();
			break;
		case 5:
			set_temp('C');
			break;
		case 6:
			set_temp('F');
			break;
		case 7:
			set_temp('H');
			break;
		case 8:
			set_date('Y');
			break;
		case 9:
			set_date('M');
			break;
		case 10:
			set_date('H');
			break;
		case 11:
			set_msg();
			break;
		default:
			break;
	}
}

void	temp_configure()
{
	i2c_start();
	i2c_start_send((0x38 << 1) | 0);

	i2c_write(0xBE); //init
	i2c_write(0x08);
	i2c_write(0x00);

	i2c_stop();
}

void	setDateValue(uint8_t value, uint8_t addr)
{
	i2c_start();
	i2c_start_send(0x51<<1 | 0);
	i2c_write(addr);
	i2c_write(value);
	i2c_stop();
}

void	setDate(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
	setDateValue(0x00, 0x02);
	setDateValue(minute, 0x03);
	setDateValue(hour, 0x04);
	setDateValue(day, 0x05);
	setDateValue(month, 0x07);
	setDateValue(year, 0x08);
}

int	main(void)
{
	usart_init();
	i2c_init();
	timer1_init();
	i2c_clock_init();
	adc_init();
	SPI_MasterInit();
	temp_configure();
	init_msg("Hihi 42 I am Pyro the little robot", 0xCF, 0);
	//setDate(0x23, 0x26, 0x03, 0x15, 0x011); //flemme du parsing
	sei();
	DDRD &= ~(1 << PD2) & ~(1 << PD4);
	DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD3);
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

	start();

	//button_init();
	
	for (;;) {
		update_sw_1_2();
		update_sw3();
		set_info();
		display_info();
		//_delay_ms(20);
	}
}
