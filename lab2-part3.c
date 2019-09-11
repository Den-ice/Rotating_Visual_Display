/*
 * lab2-pt3.c
 *
 * Created: 9/10/2019 11:21:07 AM
 * Authors : Sean Gow & Denice Hickethier 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t count=0;

ISR(      RTC_OVF_vect       ){

if(count%3==0)
		PORTB_OUT-= 0x10;
	                     //modify only low nibble    //preserve high nibble         
		PORTB_OUT = ((PORTB_OUT +0x01) && 0x0F) + (PORTB_OUT && 0xf0) ;
		count++;
}


int main(void)
{
	PORTB_DIR=0xff; //set port direction
	PORTB_OUT=0Xff; //initialize port to off

cli();                           //clear interrupt counter & register

	CLK_RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;//rtc clock setup
	RTC_PER=500; //set 500 ticks
	RTC_INTCTRL=PMIC_LOLVLEX_bm;//set low priority
	RTC_CTRL = 0x01; //set granularity to 1ms
	PMIC_CTRL = PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
	sei();
while(1){
	
	
}
}
