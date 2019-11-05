/*
 * lab8-part1
 * Combination lock state machine
 * using B1 board with 4 buttons & 4leds
 * Created: 10/31/2019
 * Authors : Sean Gow & Denice Hickethier
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

void state_machine( uint8_t input);
volatile uint8_t state;


void S0(uint8_t input);
void S1(uint8_t input);
void S2(uint8_t input);
void S3(uint8_t input);
void S4(uint8_t input);
void S5(uint8_t input);
void S6(uint8_t input);



const uint8_t pin0=0x04;
const uint8_t pin1=0x02;
const uint8_t pin2=0x08;
const uint8_t pin3=0x01;
volatile int ERROR=0;
 uint8_t input_changed;
//volatile uint8_t saved_input=0XFF;
//BADISR_vect catchall  
ISR(      RTC_OVF_vect){
			//PORTB_OUT -= ERROR ;
	}
ISR( PORTE_INT0_vect ){
	input_changed= ~PORTE_IN &0x0f;
	state_machine(input_changed);
  }


int main(void)
{
	PORTB_DIR=0xff; //set port to output
	PORTB_OUT=0Xff; //initialize port to off
	PORTE_DIR=0x00; //SET PORTS TO INPUT

	cli();                //clear interrupts

   //RTC TIMER LOW PRIORITY 
	CLK_RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
	RTC_PER=500;
	RTC_INTCTRL= PMIC_LOLVLEX_bm;
	RTC_CTRL = 0x01;
	
	PORTE_INT0MASK=0x01|0x02|0x04|0x08|0x10|0x20;  //turn on interrupts for ports 0:5	
	PORTE_INTCTRL= PMIC_MEDLVLEN_bm;
	
	PORTCFG_MPCMASK=0x01|0x02|0x04|0x08|0x10|0x20;  //0:5
	PORTE_PIN0CTRL= PORT_OPC_PULLUP_gc |    PORT_ISC_FALLING_gc; //SET MPCMASKED PINS TO 0X02 sense low (capacitive touch buttons are active low)
	
	
	PMIC_CTRL = PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
	sei();
while(1){
		
		;
}
}



void state_machine(uint8_t input){
	switch (state){
		case 0x00: //reset
			//S0(input);
			//break;
		case 0x01:
			S1(input);
			break;
		case 0x02:
			S2(input);
			
			break;
		case 0x04:
			S3(input);
			break;
		case 0x08:
			S4(input);
			break;
		case 0x10:
			S5(input);
			break;
		case 0x20:
			S6(input);
			break;
	}
}
void S0(uint8_t input){
	PORTB_OUT=0XF0;
	
}

void S1(uint8_t input){
	if (input==pin0){
		state=0x02;
		PORTB_OUT=~0X20;
	}
	else {
	state=0x40;
	PORTB_OUT=~0X60;
	}
	};
			
void S2(uint8_t input){
	if (input==pin1){
	state=0x04;
	PORTB_OUT=~0X30;
	}
	else
	{state=0x40;
	PORTB_OUT=~0X60;
	}
	
	};
void S3(uint8_t input){
	if (input==pin2){
	state=0x08;
	PORTB_OUT=~0X40;
	}else
	{state=0x40;
	PORTB_OUT=~0X60;
	}
	};
void S4(uint8_t input){
	if (input==pin3){
	state=0x10;
	PORTB_OUT=~0X50;
	}
	else{
	state=0x40;
	PORTB_OUT=~0X60;

	}
	};
	//OPEN
void S5(uint8_t input){

	};
	//LOCKED
void S6(uint8_t input){

	};
