#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL // Définir la fréquence du CPU à 16 MHz
#define UART_BAUDRATE 115200 // Définir la vitesse de transmission UART à 115200 bauds

//#define MYUBRR ((F_CPU/(UART_BAUDRATE*16UL))-1)
#define MYUBRR (F_CPU/8/UART_BAUDRATE-1)/2
//This clock is the baud rate generator clock output (= fosc/(UBRRn+1)).
//The initialization process normally consists of settingthe baud rate, setting frame format and enabling the Transmitter or the Receiver depending on the usage

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

unsigned char uart_rx(void)
{
	// wait for transmit buffer to be empty
	// Attendre la réception d'un caractère

	while (!(UCSR0A & (1 << RXC0)))
	{
	// Attente active
	}
	PORTB ^= 1 << PB0;
	// Envoyer le caractère reçu sur le port série
	return UDR0;
}

void	uart_init(void)
{
	// l’UART du MCU doit être configuré en 115200 8N1.
    UBRR0L = (uint8_t)(MYUBRR & 0xFF); 
    UBRR0H = (uint8_t)((MYUBRR >> 8) & 0xFF);

	//UCSR0A = 0;

	// Format des trames: 8 bits, 1 stop bits, none parity

	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0C &= ~(1 << USBS0) & ~(1 << UPM01) & ~(1 << UPM00);

	// Turn on the transmission and reception circuitry

	UCSR0B = (1 << TXEN0) | (1 << RXEN0);

}

ISR(PCINT2_vect)
{
	unsigned char rchar = uart_rx();
	uart_tx(rchar);
	//uart_prinstr("hello world!?..//\r\n");
}

void main() {
	//unsigned char rchar = 0;
    // Initialisation de l'UART
	DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4; //SET PB0, PB1, PB2, PB4 as output
	PORTB &= ~(1 << PB1); //Unset PB1

    uart_init();
	
	TCNT1 = 0; //Clear the timer

	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 15625;//31250 == 0.5Hz, 15625 == 1Hz, 7812 == 2Hz, 3906 == 4Hz, 1953 == 8Hz, 976 == 16Hz, 488 == 32Hz, 244 == 64Hz, 122 == 128Hz, 61 == 256Hz, 30 == 512Hz, 15 == 1024Hz, 7 == 2048Hz, 3 == 4096Hz, 1 == 8192Hz
	
	//How to use Interrupts on the timer
	TCCR1A &= ~(1 << COM1A0) & ~(1 << COM1A1); // No display the light
	TCCR1B |= (1 << WGM12); //Set to CTC mode

	//set interrupt on timer1
	TIMSK1 |= (1 << OCIE1A);

	PCMSK2 |= 1 << PCINT16; //set interrupt on PCINT18
 	PCICR |= (1 << PCIE2);
	
	sei(); //Enable global interrupt
    
	for(;;) {
	}

    return ;
}
