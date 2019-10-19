/*
 * Lab6-pt1 pwm
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


/* fun_prototypes.h */
//prototypes for peripheral initialization functions
void sys_clock();
void setup_timer();
void setup_compare_timer();
void setup_peripherals();
void setup_gpio(); 
/*end of fun */
void tcc0_pwm_control(uint8_t ,unsigned long , uint8_t );
void input_interrupts();
void tcc0_pwm_phase_correct(uint8_t time, unsigned long t_scale, uint8_t duty_cycle);

volatile int input_changed=0;
volatile int duty=0;
unsigned long sClk, pClk;
ISR (TCC0_OVF_vect){	
	

    }
	
ISR(TCC0_CCA_vect){  
	PORTC_OUT|=0X01;			
	PORTR_OUT^=0x02;
}
		
ISR(TCC0_CCB_vect){ 
	PORTC_OUT=PORTC_OUT&~0X01;			
	PORTR_OUT|=0X02;


}
ISR(TCC0_CCC_vect){

	}
	
ISR( PORTF_INT0_vect ){
	 input_changed = ~PORTF_IN ;
	if (input_changed& 0x02)
		duty++;
	else if (input_changed&0x04)
		duty--;
			

	if(TCC0_CCA==0)
		TCC0_CCB=TCC0_PER*duty/100;

		}

// call used peripheral setups
void setup_peripherals(){
	cli();
	sys_clock();
	setup_timer();
	input_interrupts();
	PMIC_CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // enable all priorities
	sei();
}


int main(void)
{	
	PORTC_DIR=0XFF;
	//PORTC.PIN0CTRL=PORT_OPC_PULLUP_gc;
	PORTR_DIR=0X2;
	setup_peripherals();
	//10 ,ms , 8%
	//tcc0_pwm_control(10,pow(10,6),8);
	
	tcc0_pwm_phase_correct(10,(unsigned long)pow(10,6),8);
while (1)
    {
    }
}


void sys_clock(){
	
	//set mcu clock/frequency
	SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
	GetSystemClocks(&sClk, &pClk);
	/*

    OSC_CTRL=OSC_RC32MEN_bm; 
    while(!(OSC_STATUS & OSC_RC32MRDY_bm));   
    CCP=CCP_IOREG_gc; 
    CLK_CTRL=CLK_SCLKSEL_RC32M_gc;
*/
	
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


void tcc0_pwm_control(uint8_t time, unsigned long t_scale, uint8_t duty_cycle){
	int i=0;
	int prescale=1024;
	uint16_t per=0;
	unsigned long pClk=32000000;
	/* 1s period =pclk/divisor */
	
	/*
	2^10=1024  0x7
	2^8=256   0x6
	2^6=64   0x5
	2^3=8    0x4
	2^2=4    0x3
	2^1=2    0x2
	2^0=1    0x1   
	*/
	//turn bitvalue into value for calculation
	for (i=7;i>TCC0_CTRLA;i--){
		prescale=prescale/2;
		if (i>4) //prescale decrement by factor of 4 if >4, or 2 <=4
		prescale=prescale/2;
    }
	t_scale=t_scale/prescale;
	per=pClk/prescale;
	per= per /t_scale*time;
//	per=per*duty_cycle;
	// 1SECOND/TSCALE*(TIME)
	TCC0_PER=per+4;
	TCC0_CCA=(TCC0_PER*duty_cycle)/100+1;	
}


void tcc0_pwm_phase_correct(uint8_t time, unsigned long t_scale, uint8_t duty_cycle){
	int i=0;
	int prescale=1024;
	uint16_t per;
	unsigned long pClk=32000000;

	/* 1s period =pclk/divisor */
	
	/*
	2^10=1024  0x7
	2^8=256   0x6
	2^6=64   0x5
	2^3=8    0x4
	2^2=4    0x3
	2^1=2    0x2
	2^0=1    0x1   
	*/
	//turn bitvalue into value for easier calculation
	for (i=7;i>TCC0_CTRLA;i--){
		prescale=prescale/2;
		if (i>4) //prescale decrement by factor of 4 if >4, or 2 <=4
		prescale=prescale/2;
    }
	t_scale=t_scale/prescale;
	per=pClk/prescale;
	per= per /t_scale*time;
//	per=per*duty_cycle;
	// 1SECOND/TSCALE*(TIME)
	TCC0_PER=per+4;
	TCC0_CCA=0;//(TCC0_PER*duty_cycle/2)/100;
	TCC0_CCB=(TCC0_PER*duty_cycle)/100+1;	
	
	TCC0_CTRLB=1<<TC_WGMODE_DSTOP_gc; //ENABLE DUAL SLOPE PWM,  OTHER OPTIONS: DS_T DS_B DSBOTH
}

void input_interrupts(){
	PORTF_INT0MASK=0x02|0x04;  //turn on interrupts for ports 0:5
	PORTF_INTCTRL= PORT_INT0LVL_MED_gc;
	PORTCFG_MPCMASK=0x02|0x04;  //0:5
	PORTF_PIN0CTRL=     PORT_ISC_FALLING_gc; //SET MPCMASKED PINS TO 0X02 sense low (capacitive touch buttons are active low)
}
