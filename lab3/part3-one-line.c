/*
 * Lab3-Part3
 * TX count using avrx library
 * no rx yet
 *
 * Created: 9/19/2019 3:16:32 PM
 * Author : Sean and Denice Hickethier
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

volatile int count=0;
volatile int done_tx=0;
volatile char *wordcount[]= {"one","two","three","four","five"};
//volatile char *word;
volatile XUSARTst Ser;
unsigned long sClk, pClk; //sysclock and perihoeral clock

ISR (TCC0_OVF_vect){
	//word=wordcount[count++%5];
	//USART_send(&Ser, word);
	USART_send(&Ser, wordcount[count++%5]);		//count from one to five in 1 second intervals
}

/* Interrupt Service Handlers */
ISR(USARTC0_RXC_vect){
	Rx_Handler(&Ser);
}

ISR(USARTC0_TXC_vect){
	Tx_Handler(&Ser);

}


/*
void USART_TX(volatile int *flag){
	if (!(wordcount[(count%26)]=='\0')&&(flag!=1))
	USARTC0_DATA=wordcount[(count++)%26]; //write next char
	else flag=1;
	
	return;
}*/
//ISR(USART_INT_vec){}


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
    }

}





void sys_clock(){
	//set mcu clock/frequency to 32Mhz
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
	long fCLK =2000000L ;  //cpu is at 32mhz, 2000000  works for 57.6k sync
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
	Ser.fInMode = _INPUT_CR | _INPUT_ECHO | _INPUT_TTY;
	Ser.fOutMode = _OUTPUT_CRLF;
	USART_enable(&Ser,(USART_TXEN_bm|USART_RXEN_bm));
}


void setup_gpio(){
	PORTR_DIR|=0x3; //enable leds
	PORTR_OUT|=0X02;//turn R0 led on

}
