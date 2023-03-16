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

void UART_puts(char* s)
{
    // transmit character until NULL is reached
    while(*s > 0) uart_tx(*s++);
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

ISR(TIMER1_COMPA_vect)
{
	uart_tx('Z');
}

void main() {
    // Initialisation de l'UART
	DDRB |= 1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4; //SET PB0, PB1, PB2, PB4 as output
	PORTB &= ~(1 << PB1); //Unset PB1

    uart_init();

	sei(); //Enable global interrupt
	
	TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 15625; //Set clock to match 7813 15625 en biaire =  0b0001111010000101
	
	//How to use Interrupts on the timer
	TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	TCCR1B |= (1 << WGM12); //Set to CTC mode

	//set interrupt on timer1
	TIMSK1 |= (1 << OCIE1A);

    for(;;) {}

    return ;
}

//#include <avr/io.h>
//#include <util/delay.h>

//#define F_CPU 16000000UL // Définir la fréquence du CPU à 16 MHz
//#define UART_BAUDRATE 115200 // Définir la vitesse de transmission UART à 115200 bauds

//// Calcul de la valeur UBRRnL
//#define MYUBRR ((F_CPU/(UART_BAUDRATE*16UL))-1)

//// Fonction pour initialiser l'UART
//void uart_init(void) {
//    // Configuration du UBRRnL pour la vitesse de transmission UART
//    UBRR0L = (uint8_t)(MYUBRR & 0xFF); 
//    UBRR0H = (uint8_t)((MYUBRR >> 8) & 0xFF);

//    // Configuration du format de trame : 8 bits de données, pas de parité, 1 bit de stop
//    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

//    // Activation de l'émetteur et du récepteur UART
//    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
//}

//// Fonction pour envoyer un caractère sur le port série du PC
//void uart_tx(uint8_t data) {
//    // Attente que le tampon d'émission soit vide
//    while (!(UCSR0A & (1 << UDRE0)));

//    // Écriture des données dans le tampon de sortie
//    UDR0 = data;
//}

//int main(void) {
//    // Initialisation de l'UART
//    uart_init();

//    while(1) {
//        // Écriture de "Z" sur le port série
//        uart_tx('Z');

//        // Attente de 1 seconde avant de répéter
//        _delay_ms(1000);
//    }

//    return 0;
//}