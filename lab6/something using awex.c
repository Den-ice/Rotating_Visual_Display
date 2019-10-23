/*
 * Lab6-pt3 servo pwm
 * pwm locked to range of 1ms to 2ms
 * Created: 10/16/19 3:16:32 PM
 * Author : Sean Gow and Denice Hickethier
 */
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <AVRXlib/AVRXClocks.h>


volatile int input_changed=0;
volatile uint16_t duty=0;
unsigned long sClk;
unsigned long  pClk=32000000;





ISR (TCC0_OVF_vect){	
	//PORTC_OUT|=0X01;
	//PORTR_OUT=PORTC_OUT&~0x02;
    }
	
ISR(TCC0_CCA_vect){  
	//PORTC_OUT=PORTC_OUT&~0X01;
	//PORTR_OUT|=0X02;

}
		
ISR(TCC0_CCB_vect){ 


}
ISR(TCC0_CCC_vect){

	}
	
ISR( PORTF_INT0_vect ){
	//CAPTURE INPUT AT ISR ENTRY
	 input_changed = ~PORTF_IN ;  //todo add bitwise comparison to previous value before update
	duty=TCC0_CCA;
	
	if (input_changed & 0x02){
		duty+=(unsigned long)(TCC0_PER*3)/100;	
	}
	else if (input_changed & 0x04){
		duty-=(unsigned long)(TCC0_PER*3)/100;	
	}
	
	if ( (duty<=TCC0_PER)&& (duty>=0))
			TCC0_CCA=duty;	
			
}

int main(void)
{	
	//port setup
		//directions
			PORTC_DIR=0XFF;
			PORTR_DIR=0X2;
			PORTR_OUT=0x2;
		//pullup
			//PORTC.PIN0CTRL=PORT_OPC_PULLUP_gc;
		//initial state
			//PORTR_DIR=0X2;
	//end port setup
	
//system initializations
	//clear interrupts	
		cli();
	
	//system clocks
		//setup clock using avrx
		//SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
		//GetSystemClocks(&sClk, &pClk);
		//end avrx sysclock
		// set sys clock manually
		OSC_CTRL=OSC_RC32MEN_bm; 
		while(!(OSC_STATUS & OSC_RC32MRDY_bm));   
		CCP=CCP_IOREG_gc; 
		CLK_CTRL=CLK_SCLKSEL_RC32M_gc;
		
	//end systemclocks

	//timer setup
		/*  32MHz Precalculations
				1024  =0x07
			32,000,000/1024 =31250= 1s
				granularity=1/31250 sec ~= 31us
				max range   31us : (31us*65535) = ~2.09+/-s
				256 =0x06
			32,000,000/256=125000
				1/125000= ~8?us
				range= 8us :  ~524ms
			32,000,000/64 =5000000=1s
				granularity=1/250000 =2us
				range 2us:  ~131ms
			32,000,000/8= 4,000,0000=1s
				granularity=1/4,000,000 = 250ns
				range 250ns: ~163.8ms
			32,000,000/4= 8,000,0000=1s
				granularity=1/8,000,000 = 125ns
				range 125ns: ~81.9ms
			32,000,000/2= 16,000,0000=1s
				granularity=1/16,000,000 = 62ns
				range 65ns: ~40.9ms
			32,000,000/1=1s
				granularity=1/32,000,000 = 31ns
				range 31ns: ~20ms	
			*/

		//	1s= 2Mhz/1024= 0x7a1
		//  1s=32m/1024=0x7a12
		//TCC0_PER=0X7a12;					//1s
		TCC0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
		//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
		//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7
		TCC0_CTRLA=0x5;
		
		//compare timer setup
			//set compare counters
				//TCC0_CCA=TCC0_PER/8;
				//TCC0_CCB=TCC0_PER/4;
				//TCC0_CCC=TCC0_PER/2;
				
			//enable compare mode
				TCC0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
				TCC0_INTCTRLB|=PMIC_MEDLVLEN_bm<<TC0_CCAINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCBINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;
	

				AWEXC.CTRL    = 0x1;   // Enable DTICCxENs 
//				AWEXC.DTHS=TCC0_PER*40/100;
	//			AWEXC.DTLS=TCC0_PER*20/100;

				AWEXC.OUTOVEN = 0x01;  
				
			//pwm timer setup
				/*servo specs
				  1ms-2ms over 20ms period
				*/
				//prescale=8;
				TCC0_PER=10400; //20ms calibrated via scope
				//TCC0_CCA=(TCC0_PER/20); //1ms
			
		//end compare timer setup
	//end timer setup

	//setup input interrupts
		PORTF_INT0MASK=0x02|0x04;  //turn on interrupts for ports 0:5
		PORTF_INTCTRL= PORT_INT0LVL_MED_gc;
		PORTCFG_MPCMASK=0x02|0x04;  //0:5
		PORTF_PIN0CTRL=PORT_ISC_RISING_gc; //SET MPCMASKED PINS TO 0X02 sense low (capacitive touch buttons are active low)
	//end input interrupts
	
	// enable all priorities
	PMIC_CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   

	//enable interrupts
	sei(); 
//end system initializations
	
	
//PART 1


while (1)
{
}
}//end main
