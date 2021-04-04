#include <stdio.h>

// ALL parallel IO ports created by QSYS have a 32 bit wide interface as far as the processor,
// that is, it reads and writes 32 bit data to the port, even though the
// port itself might only be configures as an 8 or 10 or even 4 bit wide port
//
// To access the port and read switches and writes to leds/hex displays etc, we set
// up "int pointer", i.e. pointers to 32 bit data
// to read/write data from/to the port and discard any upper bits we don't need.
// in the case of reading the switches and copying to leds/hex displays, it doesn't matter as
// they are all 8 bits so the upper unused bit don't matter,
// but push button port is 4 bit input port so reading with will give us 28 bits of leading 0's
// followed by the 4 bits corresponding to the push buttons in bits 0-3 of the data we read fom port

// the addresses below were defined by us when we created our system with Qys and assigned
// addresses to each of the ports we added (open up Qsys and check the Address Tab if you are uncertain)

#define SWITCHES    (volatile unsigned int *)(0xFF200000)
#define PUSHBUTTONS (volatile unsigned int *)(0xFF200010)

#define LEDS        (volatile unsigned int *)(0xFF200020)
#define HEX0_1      (volatile unsigned int *)(0xFF200030)
#define HEX2_3      (volatile unsigned int *)(0xFF200040)
#define HEX4_5      (volatile unsigned int *)(0xFF200050)

#define RS232_ReceiverFifo 						(*(volatile unsigned char *)(0xFF210200))
#define RS232_TransmitterFifo 					(*(volatile unsigned char *)(0xFF210200))
#define RS232_InterruptEnableReg 				(*(volatile unsigned char *)(0xFF210202))
#define RS232_InterruptIdentificationReg 		(*(volatile unsigned char *)(0xFF210204))
#define RS232_FifoControlReg 					(*(volatile unsigned char *)(0xFF210204))
#define RS232_LineControlReg 					(*(volatile unsigned char *)(0xFF210206))
#define RS232_ModemControlReg 					(*(volatile unsigned char *)(0xFF210208))
#define RS232_LineStatusReg 					(*(volatile unsigned char *)(0xFF21020A))
#define RS232_ModemStatusReg 					(*(volatile unsigned char *)(0xFF21020C))
#define RS232_ScratchReg 						(*(volatile unsigned char *)(0xFF21020E))
#define RS232_DivisorLatchLSB 					(*(volatile unsigned char *)(0xFF210200))
#define RS232_DivisorLatchMSB 					(*(volatile unsigned char *)(0xFF210202))

#define Bluetooth_ReceiverFifo        			(*(volatile unsigned char *)(0xFF210220))
#define Bluetooth_TransmitterFifo     			(*(volatile unsigned char *)(0xFF210220))
#define Bluetooth_InterruptEnableReg  			(*(volatile unsigned char *)(0xFF210222))
#define Bluetooth_InterruptIdentificationReg 	(*(volatile unsigned char *)(0xFF210224))
#define Bluetooth_FifoControlReg 				(*(volatile unsigned char *)(0xFF210224))
#define Bluetooth_LineControlReg 				(*(volatile unsigned char *)(0xFF210226))
#define Bluetooth_ModemControlReg 				(*(volatile unsigned char *)(0xFF210228))
#define Bluetooth_LineStatusReg 				(*(volatile unsigned char *)(0xFF21022A))
#define Bluetooth_ModemStatusReg 				(*(volatile unsigned char *)(0xFF21022C))
#define Bluetooth_ScratchReg 					(*(volatile unsigned char *)(0xFF21022E))
#define Bluetooth_DivisorLatchLSB 				(*(volatile unsigned char *)(0xFF210220))
#define Bluetooth_DivisorLatchMSB 				(*(volatile unsigned char *)(0xFF210222))

#define FALSE 0
#define TRUE 1


/**************************************************************************
/* Subroutine to initialise the RS232 Port by writing some data
** to the internal registers.
** Call this function at the start of the program before you attempt
** to read or write to data via the RS232 port
**
** Refer to UART data sheet for details of registers and programming
***************************************************************************/


void Init_BT(void)
{
	// set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
	*Bluetooth_LineControlReg |= 1 << 7
	// set Divisor latch (LSB and MSB) with correct value for required baud rate

	//Baud rate divisor value = (frequency of BR_clk) / (desired baud rate x 16)
	int baut_divisor = ()/( *16);
	*Bluetooth_DivisorLatchLSB = baut_divisor & 0xff;
	*Bluetooth_DivisorLatchMSB =

	// set bit 7 of Line control register (LCR) back to 0 and
	*Bluetooth_LineControlReg &= ~(1 << 7)
	// program other bits in (LCR) for 8 bit data, 1 stop bit, no parity etc

	// bit 1-0 : 11 = 8 bits
	// bit 2 : 0 = 1 stop bit
	// bit 3 : 0 = no parity
	*Bluetooth_LineControlReg = 0x03; // 0x03 = 11
	// Reset the Fifo’s in the FIFO Control Reg by setting bits 1 & 2
	*Bluetooth_FifoControlReg |= 1 << 1;
	*Bluetooth_FifoControlReg |= 1 << 2;
	// Now Clear all bits in the FIFO control registers
	*Bluetooth_FifoControlReg = *Bluetooth_FifoControlReg ^0x06 // 0x06 = 110

}

// every BT message must end in \r\n
int putcharBT(char c)
{
	// wait for Transmitter Holding Register bit (5) of line status register to be '1‘
    while (*Bluetooth_LineStatusReg != (*Bluetooth_LineStatusReg | 1 << 5));
	// indicating we can write to the device

	// write character to Transmitter fifo register
	*Bluetooth_TransmitterFifo = c;
	// return the character we printed
	return c;
}

int getcharBT( void )
{
	// wait for Data Ready bit (0) of line status register to be '1'
	while (*Bluetooth_LineStatusReg != (*Bluetooth_LineStatusReg | 1 << 0));
	// read and return new character from ReceiverFiFo register
	return (int) *Bluetooth_ReceiverFifo;

}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read from the FIFO
int BTTestForReceivedData(void)
{
	// if Bluetooth LineStatusReg bit 0 is set to 1
	if (*Bluetooth_LineStatusReg == *Bluetooth_LineStatusReg | 1 << 0){
	    return TRUE;
	}
	// return TRUE, otherwise return FALSE
	return FALSE;

}


void sendMessageBT(char * str) {

	int j = 0;
	while(str[j] != '\0'){
		putcharBT(str[j]);
		j++;
	}

	// BT messages must be terminated by these characters
	putcharBT('\r');
	putcharBT('\n');
}

// flush the BT UART receive buffer by removing any unread characters
void BT_Flush( void ){
	// while bit 0 of Line Status Register == ‘1’ (i.e. data available)
	while (*Bluetooth_LineStatusReg == *Bluetooth_LineStatusReg | 1 << 0){
	   int temp = *Bluetooth_ReceiverFifo;
	}
	// read unwanted char out of FIFO receive bufferreturn;
	// no more characters, so return
	return;
}

void main(void)
{
    int switches ;
	printf("Hello from the CPEN 391 System\n");

    while(1)    {
        switches = *SWITCHES ;
        *LEDS = switches;
        *HEX0_1 = switches;
        *HEX2_3 = switches;
        *HEX4_5 = switches;

        printf("Switches = %x\n", switches) ;
        printf("Buttons = %x\n", *PUSHBUTTONS) ;
    }
}
