#include <avr/io.h>
#include <util/twi.h> //twi presque= i2c
#include <util/delay.h>
#include <stdio.h>


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

void uart_printint32(uint32_t n)
{

	// transmit character until NULL is reached
	if (n > 9)
		uart_printint(n / 10);
	uart_tx(n % 10 + '0');
}

void uart_printhex(char *prompt, uint8_t n, int jump)
{
    uint8_t h = n >> 4;
    uint8_t l = n & 0x0F;

	uart_putstr(prompt);
    if (h < 10) uart_tx(h + '0');
    else uart_tx(h - 10 + 'A');

    if (l < 10) uart_tx(l + '0');
    else uart_tx(l - 10 + 'A');
	if (jump)
		uart_putstr("\n\r");
}

//void uart_printfloat(float value, uint8_t precision)
//{
//    char buffer[10];
//    char *ptr = buffer;

//    dtostrf(value, 6, precision, buffer);
//    while (*ptr != '\0') {
//        uart_tx(*ptr++);
//    }
//}

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
	uart_putstr("\n\r");
}

void i2c_start() {
	//TWEN == activer le module TWI
	//TWSTA == générer une condition de START
	//TWINT == clear le flag d'interruption et demande de lancer start
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); //0b10100100 == start
	while (!(TWCR & (1<<TWINT))); //Attendre que le flag d'interruption soit à 1
	_delay_ms(10);
	//uart_putstr("Started\n\r");

}

void i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void i2c_stop() {
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
	while(!(TWCR & (1<<TWSTO)));
	_delay_ms(10);
	//uart_putstr("Stoped\n\r");
}

uint8_t i2c_read( uint8_t ack ) {
	TWCR = (1<<TWINT) | ((ack ? 1 : 0) <<TWEA) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	_delay_ms(50);
	return TWDR;
}

int main() {
	uint32_t TMP = 0;
	uint32_t BUFFER[7] = {0, 0, 0, 0, 0, 0, 0};
	uart_init();
	i2c_init();
	_delay_ms(1000);

	i2c_start();
	uart_printhex("Start: ", TWSR, 1);
	i2c_write(0x38<<1 | 0);
	uart_printhex("0x38<<1 | 0: ", TWSR, 1);

	i2c_write(0xBE); //init
	uart_printhex("0xBE: ", TWSR, 1);
	i2c_write(0x08);
	uart_printhex("0x08: ", TWSR, 1);
	i2c_write(0x00);
	uart_printhex("0x00: ", TWSR, 1);

	_delay_ms(20);

	while (1)
	{
		i2c_start();
		//uart_printhex("Start: ", TWSR, 1);
		i2c_write(0x38<<1 | 0);
		//uart_printhex("0x38<<1 | 0: ", TWSR, 1);
		i2c_write(0xAC); // trigger measurement
		//uart_printhex("0xAC: ", TWSR, 1);
		i2c_write(0x33);
		//uart_printhex("0x33: ", TWSR, 1);
		i2c_write(0x00);
		//uart_printhex("0x00: ", TWSR, 1);

		_delay_ms(100); // wait for measurement to complete 80ms minimum

		i2c_stop();
		_delay_ms(40);
		i2c_start();
		//uart_printhex("Restart: ", TWSR, 1);
		i2c_write(0x38<<1 | 1);
		//uart_printhex("0x38<<1 | 1: ", TWSR, 1);
		do
		{
			_delay_ms(100);
			TMP = i2c_read(1);
		}
		while ((TMP & 1<<7) != 0);
		BUFFER[0] = TMP;

		for (uint8_t j = 1; j < 6; j++)
		{
			TMP = i2c_read((j==5 ? 0 : 1));
			BUFFER[j] = TMP;
		}
		i2c_stop();
		//uart_putstr("Buffer: ");
		int32_t Srh = BUFFER[1] << 12 | BUFFER[2] << 4 | (BUFFER[3] >> 4) & 0xf;
		int32_t St = (((uint32_t)BUFFER[3] & 0b1111)) << 16 | BUFFER[4] << 8 | BUFFER[5];
		//uart_printint32(Srh);
		//uart_tx(' ');
		//uart_printint32(St);
		//uart_putstr("\n\r");

		double RH = (double)Srh / (double)(0x100000) * 100.0;
    	double T = (double)St / (double)(0x100000) * 200.0 - 50.0;

		char RHstr[128];
		sprintf(RHstr, "Humidite relative: %.2f%% Temperature: %.2f.C\n\r", RH, T);
		uart_putstr(RHstr);

		//uart_printint32((uint32_t)T);
		//print("Humidité relative: {:.2f}%".format(RH))
		//print("Température: {:.2f}°C".format(T))
		//for (int i = 0; i < 6; i++)
		//{
		//	uart_printhex("", BUFFER[i], 0);
		//	uart_tx(' ');
		//}
		//uart_putstr("\n\r");
		_delay_ms(1000);
	}

	return 0;
}
