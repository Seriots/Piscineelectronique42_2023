/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/24 13:49:58 by lgiband          ###   ########.fr       */
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

uint16_t state = 0;

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

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<DDB3)|(1<<DDB5)|(1<<DDB2);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);// | (1<<SPR1);
}

void SPI_MasterTransmit(uint8_t cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}
void	transmitLed(uint32_t data)
{
	SPI_MasterTransmit(data >> 24);
	SPI_MasterTransmit(data >> 16);
	SPI_MasterTransmit(data >> 8);
	SPI_MasterTransmit(data);
}

void	boudary_frame(uint8_t bound)
{
	if (bound == 0)
	{
		SPI_MasterTransmit(0);
		SPI_MasterTransmit(0);
		SPI_MasterTransmit(0);
		SPI_MasterTransmit(0);
	}
	else
	{
		SPI_MasterTransmit(0xFF);
		SPI_MasterTransmit(0xFF);
		SPI_MasterTransmit(0xFF);
		SPI_MasterTransmit(0xFF);
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

void	set_spi_led(uint16_t value)
{
	boudary_frame(0);
	if (value < 1023 / 3)
		value = 3;
	else if (value < (1023 / 3) * 2)
		value = 2;
	else if (value < 1023)
		value = 1;
	else
		value = 0;
	switch (value) {
		case 0:
			transmitLed(0xE100FF00);
			transmitLed(0xE100FF00);
			transmitLed(0xE100FF00);
			break;
		case 1:
			transmitLed(0xE100A5FF);
			transmitLed(0xE100A5FF);
			transmitLed(0xE1000000);
			break;
		case 2:
			transmitLed(0xE10000FF);
			transmitLed(0xE1000000);
			transmitLed(0xE1000000);
			break;
		case 3:
			transmitLed(0xE1000000);
			transmitLed(0xE1000000);
			transmitLed(0xE1000000);
			break;
	}
	boudary_frame(1);
	
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
	if ((nbr / 1000) > 0)
		print_digit(get_value((nbr / 1000) % 10), get_digit(1));
	if ((nbr / 100) > 0)
		print_digit(get_value((nbr / 100) % 10), get_digit(2));
	if ((nbr / 10) > 0)
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

//ISR(TIMER1_COMPA_vect)
//{
//	cli();
//	state = (state + 1) % 10000;
//	sei();
//}

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

int	main(void)
{
	usart_init();
	i2c_init();
	adc_init();
	//button_init();
	init_rgb();
	SPI_MasterInit();
	sei();

	usart_printstr("Hello World\r\n");
	
	DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4;
	DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6;

	i2c_clock_init();
	i2c_clock_clear();

	i2c_clock_connect();
	//timer1_init();

		//i2c_write(0x00);

		//i2c_write(0xFF);

		//_delay_ms(500);
		//print_digit_one_time(get_value('2'), get_digit(4));
	while (1)
	{
			state = ADCL;
			state = (state) | (((uint16_t)ADCH) << 8);
			wheel(state / 4);
			FT_printf((state + 1) / 256);
			set_spi_led(state);
			//print_nbr(state);
			print_str((uint8_t*)"hihi 42 I am pyro the little robot", 0x8F);
	}
}
