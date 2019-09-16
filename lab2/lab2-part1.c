/*
 * Lab2 Part1
 * Using timer interrupt for binary counter  .25s interval
 *
 * Created: 9/5/2019 9:51:41 AM
 * Authors : Sean Gow & Denice Hickethier 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

//Interrupt Service Routine 
ISR(      TCC0_OVF_vect        ){ //handle timer counter overflow interrupt
	
	PORTB_OUT -= 0x10;  //subtract from high nibble to count up on leds
}

/* setup timer interrupt for .25s intervals
   granularity=prescaler/frequency
   interval=timer period *granularity
   64/2mhz=32us
   7812*64/2mhz=~.2499s
   1024/2mhz=512us 
   488*1024/2mhz=~.249856
   */
int main(void)
{

   PORTB_DIR=0XF0; //set port direction
   PORTB_OUT=0XF0; //initialize port to off
  
   cli(); //clear interrupt counter & register  
  
  
  //TCC0_PER=0x1e84;					//7812 counter range/period
  //TCC0_CTRLA=0x05;					// 64 PRESCALER
  
  TCC0_PER=0x01e8;	    //488 counter period
  TCC0_CTRLA=0X07;		//1024 PRESCALER
// 242.2ms intervals
	

 // TCC0_PER=0x01de;	    //478 counter period
 // TCC0_CTRLA=0X07;		//1024 PRESCALER

//237.4ms intervals
//242.2ms-237.4ms=4.8ms/10ticks = .48ms *1000= 480us measured granularity 
//programmed granularity 512us
// 512-480/512*100= 6.25% error

  
  TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; //assign medium priority to tcc0 interrupt
  PMIC_CTRL = PMIC_MEDLVLEN_bm;   // turn on medium priority interrupts
  sei();

   
   /*
   CLKSEL[3:0] Group Configuration Description
   0000 OFF None (i.e., timer/counter in OFF state)
   0001 DIV1 Prescaler: ClkPER
   0010 DIV2 Prescaler: ClkPER/2
   0011 DIV4 Prescaler: ClkPER/4
   0100 DIV8 Prescaler: ClkPER/8
   0101 DIV64 Prescaler: ClkPER/64
   0110 DIV256 Prescaler: ClkPER/256
   0111 DIV1024 Prescaler: ClkPER/1024
   1nnn EVCHn Event channel n, n= [0,...,7] 
   */
  
   
    /* Replace with your application code */
    while (1) 
    { 	
    }
}

