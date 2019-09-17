/*
 * lab3-pt1.c
 *
 * Created: 9/12/2019 9:18:32 AM
 * Author : Sean
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <AVRXlib/AVRXSerial.h>

uint8_t count=0;
/*
ISR( SPIC_INT_vect     ){
		
		//	SPIC_DATA=0b11001010;

}*/

ISR (TCC0_OVF_vect){
	PORTR_OUT-=0X01; //VISUALIZE
	SPIC_DATA=count++;
	
}



void uart_setup(){
/*		USARTE0
		USARTC0
		USARTC1
		USARTD0
		USARTD1
		USARTF0
		USARTF1
	*/
	//USART
	/*
	CTRLA;
	CTRLB;
	CTRLC; 
	BAUDCTRLA;
	BAUDCTRLB;
	
	*/
    
	return;
}

int main(void)
{
	PORTR_DIR=0xFF;
	PORTR_OUT=0X00;

	cli();
	//* timer interrupt */
	TCC0_PER=0XF4;					//period
	TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; // medium LEVEL interrupt
	   //PRESCALER /1	/2	/4	/8	/64	/256	/1024
	  //VALUE		0X1	0X2	0X3	0X4	0X5	0X6		0X7
	TCC0_CTRLA=0x04;		
	//* end timer */
	//* SPI interrupt*/
	PORTC.DIR=PIN4_bm|PIN5_bm|PIN7_bm;;
	SPIC.DATA=0b11001010;
	SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm|SPI_PRESCALER_DIV128_gc|SPI_MODE0_bm;  //| SPI_CLK2X_bm 
	/*end spi*/
	/*uart interrupt */
	uart_setup();

	/* interrupt controller */

	PMIC_CTRL = PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
	   
sei();
    while (1) 
    {;		
    }
}
