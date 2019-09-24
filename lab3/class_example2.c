/* 
Example serial output for an XMEGA to periodically send messages over a UART.
*/

#include <avr/interrupt.h> // interrupts
#include <avr/io.h> // port, pin, register names
#include <stdlib.h> // standard lib functions
#include <math.h> // general math functions
#include <string.h>
#include <AVRXlib/AVRXClocks.h> //library required for the clocks SetSystemClock

//global variable declaration
char *StringPtr;
char *strs[] = {"one\n", "two\n", "three\n", "quatro\n", "cinco\n"};
char Ubuf[10];
uint8_t chrIdx = 0, nChr = 0;
volatile char bSend = 0;	//volatile resets after each ISR
uint8_t nStr = 0;

//function prototype
void sendChar(unsigned char data);
void sendString(char* StringPtr);

// the RTC queues the next string to be sent (in main loop)
ISR(RTC_OVF_vect)
{
	
	bSend = 1;
}

// the ISR needs to check if the full string has been sent
// if so, then don't send anything else
// if not, send the next character
ISR(USARTC0_TXC_vect)
{
	
	if (chrIdx < nChr)  // setting nChr in sendString makes this more efficient
                        // than repeated calls to strlen()
        USARTC0_DATA = Ubuf[chrIdx++];  // very important to post-increment!!
}

void Setup_RTC_Timer()
{
	CLK_RTCCTRL = CLK_RTCSRC_RCOSC32_gc | CLK_RTCEN_bm; //sets the timer clock to the Real Time Clock 32.768kHz
	RTC_PER = 10000; //sets the number of timer counts that will be executed
	RTC_INTCTRL = PMIC_LOLVLEX_bm; //generates the interrupt and sets the priority to low level
	RTC_CTRL = 0X01;	//sets the prescaler for the timer clock to 1 ms
	PORTB_DIR = 0xF0;	//sets the port B direction to output
	PORTB_OUT = 0x00;	//sets the status of the LEDs to "on"
}

void Setup_Serial()
{
	PORTC_DIR |= (1 << 3);//sets the direction of pin 3 as output without changing the other bits
	PORTC_OUT |= (1 << 3);// sets the output of pin 3 high without changing other bits
	USARTC0_BAUDCTRLA = 0x0C; //period setting
	USARTC0_BAUDCTRLB = 0x40; //optional scale setting
	USARTC0_CTRLA |= (1<<3);//set medium level interrupt for trx interrupt
	USARTC0_CTRLC = (1<<0) | (1<<1); //sets 1 stop bit, o data bits, no parity
	USARTC0_CTRLB = (1<<3) | (1<<4); //enable usart receiver and transmitter
}

// the sendString funciton initiates the sending of a character array 
// via the serial port
void sendString(char *StringPtr)
{
	strcpy(Ubuf, StringPtr);	//copies string from pointer to buffer
	chrIdx = 0; //sets the number of strings to zero
	nChr = strlen(Ubuf);// calculates the total string length
	USARTC0_DATA = Ubuf[chrIdx++];	//outputs the character index values
	PORTB_OUT = (~nChr)<<4;
}


int main(void)
{
	unsigned long pClk, sClk;
	cli();	//clears interrupt flags
	SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc);
	GetSystemClocks(&sClk, &pClk);
	Setup_RTC_Timer();	//sets up the RTC clock
	Setup_Serial();	//initializes serial connection

    PMIC_CTRL = PMIC_LOLVLEX_bm; // turns on the low level interrupts
	PMIC_CTRL |= PMIC_MEDLVLEN_bm; //turn on medlevel interrupt for TXC
       
	sei();	//initializes interrupts
		
	while (1)
	{
		if (bSend)//will only go in here if it went into the ISR, if bsend=1
		{
			bSend = 0;	//flag variable
			sendString(strs[nStr]);
			nStr++;
			if (nStr > 4) //counts the number of times the sendString will go
			nStr = 0;
		}
	}
}
