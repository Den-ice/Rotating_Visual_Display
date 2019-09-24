/*
 * lab3-pt1.c
 *
 * Created: 9/12/2019 9:18:32 AM
 * Author : Sean Gow & Denice Hickethier
 */ 
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <AVRXlib/AVRXClocks.h>
//#include <AVRXlib/AVRXSerial.h>
//#include "fun_prototypes.h"  


#define fCLK 2000000L  //cpu is at 32mhz, 2000000  works for 57.6k sync
#define fbaud 57600

/* fun_prototypes.h */
//prototypes for peripheral initialization functions
void sys_clock();
void setup_timer();
void setup_spi();
void setup_usart();
void setup_peripherals();
void setup_gpio(); 
/*end of fun */

int nBScale =-5;
unsigned long nBSel;


volatile char *c="\0hello,\0how\0are\0you\0¿\0";
volatile char *wordcount="\0one\0two\0three\0four\0five\n";
volatile int flag=0;

volatile int count=0;

//setup system clock
unsigned long sClk, pClk; //sys clock and peripheral clock
ISR (TCC0_OVF_vect){
	//*word=x[count%5];
	//if ((wordcount[count%0]=='\0')||(wordcount[count%0]=='\n')){
		if (flag==1){
			flag=0;
		}
			++count;
		USARTC0_DATA=0x00; //start transmission

//	USARTC0_DATA=0x00;
}

ISR(USARTC0_TXC_vect){
	
		USART_TX(&flag);

	//}
	PORTR_OUT^=0X03;
    
}
ISR(USARTC0_DRE_vect){


}


int main(void)
{
	sys_clock();	 	 //system frequency
	setup_gpio();	 	 //led pins
	setup_peripherals(); //interrupts, spi, timer,usart
	
	USARTC0_DATA=0X01;
    while (1){	
    
    }
}


void USART_TX(volatile int *flag){
	if (!(wordcount[(count%26)]=='\0')&&(flag!=1))
			USARTC0_DATA=wordcount[(count++)%26]; //write next char
		else flag=1;
			
			return;
}








/*\  Program Code/functions /*\


/* fun_prototypes.c */
/*\/\/\/ initialization functions \/\/*/

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
	PORTC.DIRSET=PIN3_bm;
	PORTC.PIN3CTRL=PORT_OPC_PULLUP_gc;
	//nBSel= //( (10 / pow(2.0,(double)nBScale)) * (double)((double)pClk / (16.0 * (double)fbaud)) - 1.0);
	nBSel=fCLK/(16*(pow(2,nBScale)*57611))-1;
	
	USARTC0_BAUDCTRLA=(unsigned char)(nBSel &0x00FF);
	USARTC0_BAUDCTRLB=(char)((nBScale &0x00F)<<4)|((nBSel & 0x0F00>>8));	
	USARTC0.CTRLA= USART_TXCINTLVL_LO_gc;
				//ASYNC MODE                //8 BIT CHARS        //PARITY OFF               //NO STOP BIT     //low interrupt
	USARTC0.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc	|0x00	;
	USARTC0.CTRLB=USART_RXEN_bm|USART_TXEN_bm;
	
}

// call used peripheral setups
void setup_peripherals(){
	cli();
	setup_timer();
	//setup_spi();
	setup_usart();
	PMIC_CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // enable all priorities
	sei();
}

void setup_gpio(){
	PORTR_DIR|=0x3; //enable leds
	PORTR_OUT|=0X02;//turn R0 led on
	
}

/* end of fun*.c */
