*
 * lab2-pt3.c
 *
 * Created: 9/10/2019 11:21:07 AM
 * Author : Sean
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t count=0;

ISR(      RTC_OVF_vect       ){

if(count%3==0)
		PORTB_OUT-= 0x10;
		
		PORTB_OUT += (PORTB_OUT +0x01) && 0x0F ;
		count++;
}


int main(void)
{
	PORTB_DIR=0xff; //set port direction
	PORTB_OUT=0Xff; //initialize port to off

cli();                           //clear interrupt counter & register

	CLK_RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
	RTC_PER=500;
	RTC_INTCTRL=PMIC_LOLVLEX_bm;
	RTC_CTRL = 0x01;
	RTC_INTCTRL = PMIC_LOLVLEX_bm;
	
	/*TCC0_PER=0x3D1;					//set period counter to above isr
	TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; //int med
	TCC0_CTRLA=0x07;					//use 1X divider
	*/
	PMIC_CTRL = PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
	sei();
while(1){
	
	
}
}
