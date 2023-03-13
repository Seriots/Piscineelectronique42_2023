#include <avr/io.h>

//Three I/O memory address locations are allocated for each port, one each for the Data Register – PORTx, Data
//Direction Register – DDRx, and the Port Input Pins – PINx. The Port Input Pins I/O location is read only, while
//the Data Register and the Data Direction Register are read/write. However, writing a logic one to a bit in the
//PINx Register, will result in a toggle in the corresponding bit in the Data Register. In addition, the Pull-up Disable
//– PUD bit in MCUCR disables the pull-up function for all pins in all ports when set
// page: 84
int	main()
{
	//PB0 == led D1
	DDRB = 1<<PB0; //set write mode on PB0
	PORTB = 1<<PB0; //Set high (1) on bit corresponding to PB0 on PORTB 
	return (1);
}