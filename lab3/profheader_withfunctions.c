/*
 * GccApplication8.c
 *
 * Created: 9/19/2019 3:16:32 PM
 * Author : Sean and Denice Hickethier
 */
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include <AVRXlib/AVRXClocks.h>
#include <AVRXlib/AVRXSerial.h>

//#define true 1

volatile int flag=0;
volatile char *wordcount[]= {"one","two","three","four","five\n"};
volatile char *word = &wordcount[0];
volatile XUSARTst Ser;
unsigned long sClk, pClk; //sysclock and perihoeral clock

ISR (TCC0_OVF_vect){
	
		if (flag==1){
			flag=0;
		}
		else{
			USART_send(&Ser,word);
		}
			//++count;
		USARTC0_DATA=0x00; //start transmission


}
/* Interrupt Service Handlers */
ISR(USARTC0_RXC_vect){
	Rx_Handler(&Ser);
}

ISR(USARTC0_TXC_vect){
	Tx_Handler(&Ser);

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
/*
void USART_TX(volatile int *flag){
	if (!(wordcount[(count%26)]=='\0')&&(flag!=1))
	USARTC0_DATA=wordcount[(count++)%26]; //write next char
	else flag=1;
	
	return;
}*/
//ISR(USART_INT_vec){}


int main(void)
{
	
	cli();
	setup_timer();
	//PORTC_OUT=0xff;
	//PORTC_INTCTRL=PMIC_LOLVLEN_bm;
	//set mcu clock/frequency to 32Mhz
	SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
	GetSystemClocks(&sClk, &pClk);
										 //(_USART_TXCIL_LO | _USART_RXCIL_LO)
	USART_init(&Ser, 0xc0, pClk, (_USART_TXCIL_LO | _USART_RXCIL_LO), 576, -4,_USART_CHSZ_8BIT, _USART_PM_DISABLED, _USART_SM_1BIT);
		//USARTC0_CTRLA=0xff;

	USART_buffer_init(&Ser, 160, 180);
	//Ser.fInMode = _INPUT_CR | _INPUT_ECHO | _INPUT_TTY;
	//Ser.fOutMode = _OUTPUT_CRLF;
		//USARTC0_CTRLA=0xff;
	USART_enable(&Ser,(USART_TXEN_bm|USART_RXEN_bm));
	//USARTC0_CTRLA=PMIC_LOLVLEN_bm;
 //				 PMIC_LOLVLEN_bm;
//must set pmic priority

   PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;   // turn on medium priority interrupts

sei();
//Ser.fInMode=1;

//Ser.fOutMode=1;


USART_send(&Ser,word);
while (1)
    {
		if(Ser.serStatus == _USART_TX_EMPTY)
		{
			flag=1;
			
		}
//USART_send(&Ser,word);
    }

}
