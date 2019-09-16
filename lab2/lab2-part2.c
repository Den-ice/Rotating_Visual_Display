/*
 * lab2-part2.c
 * Saturating the ISR
 *
 * Created: 9/10/2019 9:22:52 AM
 * Authors : Sean Gow & Denice Hickethier 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


ISR(      TCC0_OVF_vect        ){
	PORTB_OUT = ~PORTB_OUT ;  //toggle portb state
    
}

int main(void)
{
	   PORTB_DIR=0x01; //set portb pin 0 to out direction
	   PORTB_OUT=0x01; //set initial state of portb  for isr toggle
	   
	   cli();                           //clear interrupt counter & register
	   TCC0_PER=0x2f;					// rough isr saturation point  
	   TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; // medium interrupt
	   TCC0_CTRLA=0x01;					//use 1X prescaler
	   PMIC_CTRL = PMIC_MEDLVLEN_bm;   // turn on medium priority interrupts
	   sei();

    /* Replace with your application code */
    while (1) 
    {
    }
}
