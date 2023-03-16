#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL // Définir la fréquence du CPU à 16 MHz
#define UART_BAUDRATE 115200 // Définir la vitesse de transmission UART à 115200 bauds

//#define MYUBRR ((F_CPU/(UART_BAUDRATE*16UL))-1)
#define MYUBRR (F_CPU/8/UART_BAUDRATE-1)/2
//This clock is the baud rate generator clock output (= fosc/(UBRRn+1)).
//The initialization process normally consists of settingthe baud rate, setting frame format and enabling the Transmitter or the Receiver depending on the usage
const unsigned char login[] = "a";
const unsigned char password[] = "a";

unsigned char test_login[32] = {0};
unsigned char test_password[32] = {0};
uint8_t state = 0;
uint8_t i = 0;

void uart_tx(unsigned char data)
{
    // wait for transmit buffer to be empty
	while (!(UCSR0A & (1 << UDRE0)));
    // load data into transmit register
    UDR0 = data;
}

void uart_prinstr(char* s)
{
    // transmit character until NULL is reached
    while(*s > 0) uart_tx(*s++);
}

void uart_printint(uint8_t n)
{

	// transmit character until NULL is reached
	if (n > 9)
		uart_printint(n / 10);
	uart_tx(n % 10 + '0');
}

unsigned char uart_rx(void)
{
	// Attendre la réception d'un caractère

	while (!(UCSR0A & (1 << RXC0))) ;

	PORTB ^= 1 << PB0;
	// Envoyer le caractère reçu sur le port série
	return UDR0;
}

void	uart_init(void)
{
    UBRR0L = (uint8_t)(MYUBRR & 0xFF); 
    UBRR0H = (uint8_t)((MYUBRR >> 8) & 0xFF);

	//UCSR0A = 0;

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0C &= ~(1 << USBS0) & ~(1 << UPM01) & ~(1 << UPM00);

	// Turn on the transmission and reception circuitry

	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);

}

int	test_mdp()
{
	uint8_t j = 0;

	while (test_login[j] != '\0' && login[j] != '\0')
	{
		if (test_login[j] != login[j])
			return 0;
		j++;
	}
	if (test_login[j] != login[j])
		return 0;
	j = 0;
	while (test_password[j] != '\0' && password[j] != '\0')
	{
		if (test_password[j] != password[j])
			return 0;
		j++;
	}
	if (test_password[j] != password[j])
		return 0;
	return 1;
}

void	load_fiesta()
{
	PORTB |= (1 << PB1) | (1 << PB2) | (1 << PB4);

    //DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
	//PORTD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
	// Configure Timer/Counter1 en mode PWM 8 bits


    // Configure PD5 en sortie
}

void	remove_char(unsigned char *str)
{
	// move cursor back
	if (i == 0 && str != 0)
		return ;
	if (str != 0)
		str[i] = '\0';
	i--;
    //uart_tx('\x1B');
    //uart_tx('[');
    //uart_tx('D');
    uart_tx('\b');

    // overwrite previous character with space
    uart_tx(' ');
	
    uart_tx('\b');

    // move cursor backward again
    //uart_tx('\x1B');
    //uart_tx('[');
    //uart_tx('D');
}

void	load_login()
{
	uart_prinstr("Login: ");
	state = 1;
}

void	load_intro()
{
	uart_prinstr("Welcome to the fiesta!\n\r");
	uart_prinstr("Please enter your login and password to continue.\n\r");
	load_login();
}

ISR(USART_RX_vect)
{
	//cli();
	unsigned char rchar = UDR0;
	//if (state == 0)
	//{
	//	if (rchar == '\r')
	//	{
	//		uart_tx('');
	//		uart_prinstr("\n\rLogin: ");
	//		state = 1;
	//	}
	//}
	if (state == 1)
	{
		if (rchar == '\r')
		{
			test_login[i] = '\0';
			i = 0;
			uart_prinstr("\n\rPassword: ");
			state = 2;
		}
		else if (rchar == 127)
		{
			remove_char(test_login);
		}
		else if (rchar >= 32 && i < 32)
		{
			uart_tx(rchar);
			test_login[i] = rchar;
			i++;
		}
	}
	else if (state == 2)
	{
		if (rchar == '\r')
		{
			test_password[i] = '\0';
			i = 0;
			uart_prinstr("\n\r");
			if (test_mdp() == 1)
			{
				uart_prinstr("Welcome Aboard, Captain!\n\r");
				state = 3;
				load_fiesta();
			}
			else
			{
				uart_prinstr("Hoo No!\n\r");
				load_login();
			}
			test_login[0] = 0;
			test_password[0] = 0;
		}
		else if (rchar == 127)
			remove_char(test_password);
		else if (rchar >= 32 && i < 32)
		{
			uart_tx('*');
			test_password[i] = rchar;
			i++;
		}
	}
}

void main() {
	//unsigned char rchar = 0;
    // Initialisation de l'UART
	uint8_t i1 = 0;
	uint8_t i2 = 0;
	uint8_t i3 = 0;
	uint16_t j = 0;
	DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4; //SET PB0, PB1, PB2, PB4 as output
	PORTB &= ~(1 << PB1); //Unset PB1
	 //Configure Timer/Counter1 en mode PWM 8 bits
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Fast PWM mode, non-inverting output on OC0B (PD5)
    TCCR0B |= (1 << CS01);  // Prescaler = 8

     // Set PD3 as output
    TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // Fast PWM mode, non-inverting output on OC2B (PD3)
    TCCR2B |= (1 << CS21);  // Prescaler = 8
    uart_init();

	//PCMSK2 |= 1 << PCINT16; //set interrupt on PCINT18
 	//PCICR |= (1 << PCIE2);
	
	sei(); //Enable global interrupt
    
	load_intro();
	for(;;) {
		if (state == 4)
		{
			if (j >= 15000)
			{
				OCR0B = i1;
				OCR0A = i2;
				OCR2B = i3;
				i1++;
				i2+=6;
				i3+=18;
				if (i1 == 256)
					i1 -= 255;
				if (i2 == 256)
					i2 -= 255;
				if (i3 == 256)
					i3 -= 255;
				j = 0;
			}
			else
				j++;
		}
		else if (state == 3)
		{
			DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6; //SET PD3, PD5, PD6 as output
			state = 4;
		}

	}

    return ;
}
