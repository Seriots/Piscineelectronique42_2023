#include <avr/io.h>
#include <util/twi.h> //twi presque= i2c


// Protocole i2c: start, adresse, mode(lecture/ecriture) -> reponse d'acquitement de l'esclave(0 signal ack) ->
//  ecriture: data->8bit envoye par le maitre, puis ack(0) de l'esclave, -> stop quand tout les bits sont envoye
// lecture: data->8bit envoye par l'esclave, puis si ack(0) du maitre alors nouvel envoie, si nack(1) arret de l'envoi
#define F_CPU 16000000UL
#define F_I2C 100000UL//100000UL
#define UART_BAUDRATE 115200 // Définir la vitesse de transmission UART à 115200 bauds

#define MYUBRR (F_CPU/8/UART_BAUDRATE-1)/2
#define TWI_BITRATE ((F_CPU / F_I2C) - 16) / 2

void uart_tx(unsigned char data)
{
    // wait for transmit buffer to be empty
	while (!(UCSR0A & (1 << UDRE0)));
    // load data into transmit register
    UDR0 = data;
}

void uart_putstr(char* s)
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

void i2c_init() {
	TWSR = 0; //the Prescaler bits in the TWI Status Register
	TWBR = TWI_BITRATE; //TWI Bit Rate Register;
	uart_putstr("Bit rate = ");
	uart_printint(TWBR); //TWBR = 72
	uart_tx('\n\r');
}

void i2c_start() {
	//TWEN == activer le module TWI
	//TWSTA == générer une condition de START
	//TWINT == clear le flag d'interruption et demande de lancer start
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); //0b10100100 == start
	while (!(TWCR & (1<<TWINT))); //Attendre que le flag d'interruption soit à 1
	if (TWSR == 0x08)
		uart_putstr("start\n\r");
	else
		uart_putstr("start error\n\r");
}

void i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void i2c_stop() {
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	uart_putstr("stop\n\r");
}

int main() {
	uart_init();
	i2c_init();
	i2c_start();
	if ((TWSR) != 0x08) { // vérification du statut
		return 1;
	}
	//i2c_write(0x38); // adresse de l'AHT20 en mode écriture

	//if ((TWSR & 0b00011111) != 0x18) { // vérification du statut
	//	return 1;
	//}
	//i2c_write(0xAC); // commande pour démarrer la mesure de température
	//if ((TWSR & 0xF8) != 0x28) { // vérification du statut
	//	return 2;
	//}
	i2c_stop();

	return 0;
}