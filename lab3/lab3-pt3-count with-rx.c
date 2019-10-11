/*
 * Lab3- PART3
 * 
 * Created: 9/19/2019 3:16:32 PM
 * Author : Sean Gow and Denice Hickethier
 */
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>

#include <AVRXlib/AVRXClocks.h>
#include <AVRXlib/AVRXSerial.h>


/* fun_prototypes.h */
//prototypes for peripheral initialization functions
void sys_clock();
void setup_timer();
void setup_spi();
void setup_usart();
void setup_peripherals();
void setup_avrx_usart();
void setup_gpio(); 
/*end of fun */

void parse_it();

volatile int count=0;
volatile XUSARTst Ser;
char *wordcount[]= {"one\n","two\n","three\n","four\n","five\n","six\n","seven\n","eight\n","nine\n","ten\n","eleven\n","twelve\n","thirteen\n","fourteen\n","fifteen\n"};
char *word;

unsigned long sClk, pClk; //sysclock and peripheral clock
char rx_buf[1];

volatile int wait_for_it=0; //sephamore /flag
volatile int enable_count=1;
volatile int flag=0;

ISR (TCC0_OVF_vect){
	if (enable_count==1){
		word=wordcount[count%5];
		count++;	
		USART_send(&Ser, word);
	}
}

/* Interrupt Service Handlers */
ISR(USARTC0_RXC_vect){
	Rx_Handler(&Ser);
	wait_for_it=1;
}

ISR(USARTC0_TXC_vect){
	Tx_Handler(&Ser);
}

ISR(USARTC0_DRE_vect){
}

/*
//non avrx library usart tx function
void USART_TX(volatile int *flag){
	if (!(wordcount[(count%26)]=='\0')&&(flag!=1))
	USARTC0_DATA=wordcount[(count++)%26]; //write next char
	else flag=1;	
	return;
}
*/

// call used peripheral setups
void setup_peripherals(){
	cli();
	sys_clock();
	setup_timer();
		
	//setup_spi();
	//setup_usart();
	setup_avrx_usart();
	PMIC_CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // enable all priorities
	sei();
}


int main(void)
{		
	setup_peripherals();
while (1)
    {
		if (wait_for_it==1)			
		{
			wait_for_it=0;
			USART_read(&Ser,rx_buf);
			USART_RxFlush(&Ser);
			parse_it(rx_buf);	
		}
    }
}

void parse_it(char *text){	
	switch (text[0]){
		case '1': count=1;break;
		case '2': count=2;break;
		case '3': count=3;break;
		case '4': count=4;break;
		case '5': count=5;break;
		case '6': count=6;break;
		case '7': count=7;break;
		case '8': count=8;break;
		case '9': count=9;break;
		case 'a': count=10;break;
		case 'b': count=11;break;
		case 'c': count=12;break;
		case 'd': count=13;break;
		case 'e': count=14;break;
		case 'f': count=15;break;
		case 's': enable_count^=0x01;
			USART_send(&Ser,"toggle_count");
			break;

	}
	count =count-1;
	return;	
}

void sys_clock(){
	//set mcu clock/frequency
	SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
	GetSystemClocks(&sClk, &pClk);
	}
	
void setup_timer(){
	//	1s= 2Mhz/1024= 0x7a1
	//  1s=32m/1024=0x7a12
	TCC0_PER=0X7a12;					//1s
	TCC0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
	TCC0_CTRLA=0x7;
	//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
}
//Configure SPI
void setup_spi(){
	PORTC.DIR|=PIN4_bm|PIN5_bm|PIN7_bm;;
	PORTC.PIN5CTRL=PORT_OPC_PULLUP_gc; 
	SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm|SPI_PRESCALER_DIV128_gc|SPI_MODE0_bm;  //| SPI_CLK2X_bm
	SPIC.DATA=0b11001010;  //something to transmit
}

//Configure USART
void setup_usart(){
	
	int nBScale =-5;
	unsigned long nBSel;
	long fCLK =2000000L ;  //cpu is at 32mhz, syncs works for 57.6k sync
	long fbaud= 57611 ;
	
	PORTC.DIRSET=PIN3_bm;		//set pin3 to output
	PORTC.PIN3CTRL=PORT_OPC_PULLUP_gc;	//pullup pin3
	nBSel=fCLK/(16*(pow(2,nBScale)*fbaud))-1; //calculate settings, tweaked to sync at ~57.6k
	
	USARTC0_BAUDCTRLA=(unsigned char)(nBSel &0x00FF);	
	USARTC0_BAUDCTRLB=(char)((nBScale &0x00F)<<4)|((nBSel & 0x0F00>>8));	
	USARTC0.CTRLA= USART_TXCINTLVL_LO_gc;
				//ASYNC MODE                //8 BIT CHARS        //PARITY OFF               //NO STOP BIT     //low interrupt
	USARTC0.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc	|0x00	;
	USARTC0.CTRLB=USART_RXEN_bm|USART_TXEN_bm;  
	
}
void setup_avrx_usart(){
	
	USART_init(&Ser, 0xc0, pClk, (_USART_TXCIL_LO | _USART_RXCIL_LO), 576, -4,_USART_CHSZ_8BIT, _USART_PM_DISABLED, _USART_SM_1BIT);
	USART_buffer_init(&Ser, 160, 180);
	Ser.fInMode = _INPUT_CR ; //| _INPUT_ECHO | _INPUT_TTY;
	//Ser.fOutMode = _OUTPUT_CRLF;
	USART_enable(&Ser,(USART_TXEN_bm|USART_RXEN_bm));
}

void setup_gpio(){
	PORTR_DIR|=0x3; //enable leds
	PORTR_OUT|=0X02;//turn R0 led on

}
