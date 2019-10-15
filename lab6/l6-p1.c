/*
 * Lab4-Part2-interrupt timer triggered samples
 * Created: 9/19/2019 3:16:32 PM
 * Author : Sean Gow and Denice Hickethier
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
void setup_peripherals();
void setup_gpio(); 
/*end of fun */


unsigned long sClk, pClk; //sysclock and peripheral clock

ISR (TCC0_OVF_vect){				
    }
	
	ISR(TCC0_CCA_vect){  
	}
		
	ISR(TCC0_CCB_vect){ 
	}
	ISR(TCC0_CCC_vect){

	}


// call used peripheral setups
void setup_peripherals(){
	cli();
	sys_clock();
	setup_timer();
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
	//set mcu clock/frequency
	SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
	GetSystemClocks(&sClk, &pClk);
	}
	
void setup_timer(){
	//	1s= 2Mhz/1024= 0x7a1
	//  1s=32m/1024=0x7a12
	TCC0_PER=0X7a12;					//1s
	TCC0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
	
	//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
	setup_compare_timer();
	TCC0_CTRLA=0x7;
	
}

void setup_compare_timer(){
//set compare counters			
	TCC0_CCA=TCC0_PER/8;
	TCC0_CCB=TCC0_PER/4;
	TCC0_CCC=TCC0_PER/2;
	
	TCC0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
	TCC0_INTCTRLB|=PMIC_MEDLVLEN_bm<<TC0_CCAINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCBINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;	
}

