*
 * lab2-pt4.c
 *
 * Created: 9/10/2019 11:21:07 AM
 * Author : Sean
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t count=0;

ISR(      PORTF_INT0_vect      ){
count++;
}


int main(void)
{
	
PORTR_DIR=0x3;
PORTR_OUT=0xFF;

	cli();                           //clear interrupt counter & register
PORTF_DIR=0xf0;
PORTF_PIN0CTRL  = 0x01;
PORTF_PIN1CTRL = 0x01; //SW2
PORTF_INTCTRL =PMIC_LOLVLEX_bm;
	PORTCFG_MPCMASK = 0x01 | 0x02;
	PORTF_PIN1CTRL = 0x01;
	

	PMIC_CTRL = PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
	sei();
while(1){
	
	
}
}
