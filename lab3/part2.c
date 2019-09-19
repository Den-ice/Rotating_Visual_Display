/*
 * lab3-pt1.c
 *
 * Created: 9/12/2019 9:18:32 AM
 * Author : Sean
 */ 
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <AVRXlib/AVRXSerial.h>

//#define clock_freq 32000000
#define fCLK2M 2000000

int nBScale = -4;
unsigned long nBSel;

volatile uint8_t count=0;


ISR (TCC0_OVF_vect){
				//	USARTC0_DATA=0xb1100011;   //send data
				//	count=count<<1;
				USARTC0_DATA= 0xAA;	
					//count++;
}
ISR(USARTC0_TXC_vect){
	//USARTC0_DATA=0X00;
			//	USARTC0_DATA=' ';   //send data
			
		
}
ISR(USARTC0_DRE_vect){
    //USARTC0_DATA=0x01;

}




int main(void)
{
	PORTR_DIR=0xff;
	PORTR_OUT=0X00;


//INITIALIZATIONS
	cli();
	//* timer interrupt */
	TCC0_PER=0Xf4;					//period
	TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; // medium LEVEL interrupt
	   //PRESCALER /1	/2	/4	/8	/64	/256	/1024
	  //VALUE		0X1	0X2	0X3	0X4	0X5	0X6		0X7
	TCC0_CTRLA=0x2;		
	//* end timer */
	
	//* SPI interrupt*/
	//PORTC.DIR=PIN4_bm|PIN5_bm|PIN7_bm;;
	//SPIC.DATA=0b11001010;
	//SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm|SPI_PRESCALER_DIV128_gc|SPI_MODE0_bm;  //| SPI_CLK2X_bm 
	/*end spi*/
	
	
	/*uart setup */
	PORTC.DIRSET=PIN3_bm;
	//PORTC.PIN3CTRL=
	uint16_t fBuad=57600;
	long power=pow(2,nBScale);
	//nBsel is 34
	nBSel= fCLK2M/(16*(power*fBuad)-1);
    USARTC0.CTRLA= USART_TXCINTLVL_LO_gc;
	USARTC0_BAUDCTRLA=(unsigned char)(nBSel &0x00FF);
	USARTC0_BAUDCTRLB=(char)((nBScale &0x00F)<<4)|((nBSel & 0x0F00>>8));
					//ASYNC MODE                //8 BIT CHARS        //PARITY OFF               //NO STOP BIT
	USARTC0.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | !USART_SBMODE_bm  ; 
	USARTC0.CTRLC=PMIC_LOLVLEX_bm;
	USARTC0.CTRLB=USART_RXEN_bm|USART_TXEN_bm;
	
	/*end uart*/

	/* interrupt controller */
	PMIC_CTRL = PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
sei();

	USARTC0_DATA=0x01;   //send data
    while (1) 
    {;	
						//	USARTC0_DATA=0xb1101010;   //send data


    }
}
