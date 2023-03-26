
#include <util/delay.h>

#include "ft_i2c.h"
#include "ft_display.h"
#include "ft_usart.h"

uint8_t	get_digit(uint8_t digit)
{
	return ((0xF0 - (1 << (digit + 3))));
}

uint8_t	get_value(uint8_t digit)
{
	if (digit == 0 || digit == '0')
		return (0x00 + TOP + TOP_RIGHT + BOTTOM_RIGHT + BOTTOM + BOTTOM_LEFT + TOP_LEFT);
	if (digit == 1 || digit == '1')
		return (0x00 + TOP_RIGHT + BOTTOM_RIGHT);
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
	i2c_stop();
	_delay_ms(1);
}

void	print_digit_one_time(uint8_t value, uint8_t digit)
{
	i2c_clock_connect();
	i2c_write(digit);
	i2c_write(value);
}

void	print_nbr(uint16_t nbr, uint8_t led)
{
	if ((nbr / 1000) > 0)
		print_digit(get_value((nbr / 1000) % 10), get_digit(1) | led);
	if ((nbr / 100) > 0)
		print_digit(get_value((nbr / 100) % 10), get_digit(2) | led);
	if ((nbr / 10) > 0)
		print_digit(get_value((nbr / 10) % 10), get_digit(3) | led);
	print_digit(get_value((nbr) % 10), get_digit(4) | led);
}

void	print_tmcu(uint16_t nbr, uint8_t led)
{
	if ((nbr / 100) > 0)
		print_digit(get_value((nbr / 100) % 10), get_digit(1) | led);
	if ((nbr / 10) > 0)
		print_digit(get_value((nbr / 10) % 10), get_digit(2) | led);
	print_digit(get_value((nbr) % 10) + DOT, get_digit(3) | led);
	print_digit(get_value('c'), get_digit(4) | led);
}

void	print_text(uint16_t nbr, uint8_t led, uint8_t unite)
{
	if ((nbr / 100) > 0)
		print_digit(get_value((nbr / 100) % 10), get_digit(1) | led);
	if ((nbr / 10) > 0)
		print_digit(get_value((nbr / 10) % 10), get_digit(2) | led);
	print_digit(get_value((nbr) % 10) + DOT, get_digit(3) | led);
	print_digit(get_value(unite) + DOT, get_digit(4) | led);
}

void	print_42(uint8_t led)
{
	print_digit(get_value('-'), get_digit(1) | led);
	print_digit(get_value('4'), get_digit(2) | led);
	print_digit(get_value('2'), get_digit(3) | led);
	print_digit(get_value('-'), get_digit(4) | led);
}

void	print_date(uint16_t nbr, uint8_t led)
{
	print_digit(get_value((nbr / 1000) % 10), get_digit(1) | led);
	print_digit(get_value((nbr / 100) % 10), get_digit(2) | led);
	print_digit(get_value((nbr / 10) % 10), get_digit(3) | led);
	print_digit(get_value((nbr) % 10) + DOT, get_digit(4) | led);
}

void	print_msg(uint8_t *msg, uint8_t led)
{

	print_digit(get_value(msg[3]), get_digit(4) | led);
	print_digit(get_value(msg[2]), get_digit(3) | led);
	print_digit(get_value(msg[1]), get_digit(2) | led);
	print_digit(get_value(msg[0]), get_digit(1) | led);
}

