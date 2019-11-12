/*
 * lab8-part2
 * Created: 2019
 * Authors : Sean Gow & Denice Hickethier
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <AVRXlib/AVRXClocks.h>

void setup_timer();
void setup_compare_timer();

void state_machine( uint8_t input);
volatile uint8_t state=0;
volatile uint8_t motor_state=0x01;
void S0(uint8_t input);
void S1(uint8_t input);
void S2(uint8_t input);
void S3(uint8_t input);


#define ON  1
#define OFF 0
#define UP 0
#define DOWN 1
#define BUTTON 0x01
#define ENDUP 0x02
#define ENDDOWN 0x03
#define BADDOOR 0x04

#define SQUIRREL 4095
#define JUSTRIGT 4095
#define OO_A_MAGNET 1024

volatile uint16_t adc_sample[3];
void setup_adcA();
volatile uint8_t adccount=0;
 unsigned long sClk, pClk;
uint8_t input_changed;
 
 //BADISR_vect catchall  
ISR(      RTC_OVF_vect){
			//PORTB_OUT -= ERROR ;
	}	

	ISR (TCC0_OVF_vect){
	//light sensor
	if (adc_sample[0]==SQUIRREL)
	state_machine(BADDOOR);
	//temp
	if (adc_sample[1]>=JUSTRIGT&&adc_sample[1]<=JUSTRIGT+300)  //temp between 20*C and 25*C
	state_machine(ENDUP);
	//external- hall effect
	if (adc_sample[2]>=OO_A_MAGNET)
	state_machine(ENDDOWN);	
	}

	
ISR( PORTC_INT0_vect ){
	
	input_changed= ~PORTC_IN & BUTTON;
	state_machine(input_changed);
  }

ISR(ADCA_CH0_vect){
		adc_sample[adccount]=ADCA.CH0.RES;
	
}

//TRIGGER 1X LIGHT SENSOR SAMPLE
ISR(TCC0_CCA_vect){  
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCA_CTRLA |= ADC_CH0START_bm ; 
	adccount=0;
}

//TRIGGER 1X TEMPERATURE SAMPLE
ISR(TCC0_CCB_vect){ 
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;
	ADCA_CTRLA |= ADC_CH0START_bm ; 
	adccount=1;
}
//TRIGGER 1X EXTERNAL SENSOR SAMPLE     HALL EFFECT?
ISR(TCC0_CCC_vect){
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;
	ADCA_CTRLA |= ADC_CH0START_bm ; 
	adccount=2;
}

int main(void)
{
		cli();       
//	PORTB_DIR=0xff; //set port to output
	//PORTB_OUT=0Xff; //initialize port to off
	//PORTE_DIR=0x00; //SET PORTS TO INPUT
		//set mcu clock/frequency
		SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
		GetSystemClocks(&sClk, &pClk);
		PORTC_DIR=0;
PORTR_DIR&=0X07;
PORTR_OUT&=0X06;

//LCD BACKLIGHT
PORTE_DIR&=0X08;

PORTC_INT0MASK=0x01|0x02|0x04|0x08|0x10|0x20;  //turn on interrupts for ports 0:5
PORTC_INTCTRL= PMIC_MEDLVLEN_bm;

    PORTCFG_MPCMASK=0x01|0x02|0x04|0x08|0x10|0x20;  //0:5
	PORTC_PIN0CTRL=  PORT_OPC_PULLUP_gc|   PORT_ISC_RISING_gc; //SET MPCMASKED PINS TO 0X02 sense low (capacitive touch buttons are active low)
	
         //clear interrupts

setup_timer();
	setup_adcA();
	PMIC_CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
	sei();
		state_machine(0); //initialize state machine
while(1){
		
		;
}
}


void state_machine(uint8_t input){
	switch (state){
		case 0x00: //closed
			S0(input);
			break;
		case 0x01: //opening
			S1(input);
			break;
		case 0x02://open
			S2(input);
			break;
		case 0x04://closing
			S3(input);
			break;
	}
}	


void S0(uint8_t input){ //closed
	input=input&BUTTON;
		//PORTB_OUT=0XE0;	
		if(input&!BUTTON){
			state     = 0x01;
			state_machine(0);
		}
		else{
		//motor=OFF;
		PORTR_OUT|=0X04;
		}
}

void S1(uint8_t input){//opening
	if (input &ENDUP){
			state==0x02;
		state_machine(0);
		}
		else{
		//direction=UP;
		PORTR_OUT&=~0X02;
		//motor=ON;
		PORTR_OUT&=~0X04;
		}
	};
			
void S2(uint8_t input){//opened
	if (input&BUTTON){
		state=0x03;
		state_machine(0);
	}
else{	
	//motor=OFF;
	PORTR_OUT|=0X04;
	}
	};
	
void S3(uint8_t input){//closing
	if(input&BADDOOR){
		state=0x01;
		state_machine(0);
	}
	else{
		//direction=DOWN;

		PORTR_OUT&=~0X02;
		//motor=ON;
		PORTR_OUT&=~0X04;
	}
	};


void setup_adcA(){
	ADCA_CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCA_REFCTRL |= ADC_REFSEL_INTVCC2_gc; // use 3V3 to chip
	ADCA_PRESCALER |= ADC_PRESCALER_DIV512_gc; //peripheral clock/512
	ADCA_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
	
	ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCA_CTRLA |= ADC_ENABLE_bm;
	ADCA_CH0_INTCTRL = ADC_CH_INTMODE_COMPLETE_gc| ADC_CH_INTLVL_HI_gc;
	
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
	TCC0_CCA=TCC0_PER/6;
	TCC0_CCB=TCC0_PER/5;
	TCC0_CCC=TCC0_PER/4;

	TCC0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
	TCC0_INTCTRLB|=PMIC_MEDLVLEN_bm<<TC0_CCAINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCBINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;
}
