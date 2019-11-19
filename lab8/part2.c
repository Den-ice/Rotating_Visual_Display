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
void setup_spi();
void setup_peripherals();
void setup_avrx_usart();
void setup_gpio(); 
/*end of fun */

int dark[2];

int dim[2];


int bright[2];

void setup_adcA(); //new for lab4
void setup_adcB(); //new for lab4
volatile XUSARTst Ser;

unsigned long sClk, pClk; //sysclock and peripheral clock
 char rx_buf[1];
volatile uint16_t hall;
char *sample_word;
volatile uint16_t light=0;
volatile uint16_t close[2],avg[2],far[2];

volatile int D,I,B,C,A,F=0;


ISR (TCC0_OVF_vect){

//calculate fuzziness truth value
if(light<=(dim[0]-300)){
	D= 100;
	I=0;
	B=0;
}
else if (light<=dark[1]){
	I=(light-200)/7;
	D=100-I;
	B=0;
}
else if (light<=bright[0]){
	I=100;
	D=0;
	B=0;
}
else if (light<=dim[1]){
	B=(light-bright[0])/4;
	I=100-B;
	D=0;
}
else{
	B=100;
	D=0;
	I=0;
}
if(hall<=(avg[0]-200)){
	C= 100;
	A=0;
	F=0;
}
else if (hall<=close[1]){
	A=(hall-2600)/2;
	C=100-I;
	F=0;
}
else if (hall<=far[0]){
	A=100;
	C=0;
	F=0;
}
else if (hall<=avg[1]){
	F=(hall-far[0])/9;
	A=100-F;
	C=0;
}
else{
	F=100;
	C=0;
	A=0;
}



	ADCA_CTRLA |= ADC_CH0START_bm ; //
		ADCB_CTRLA |= ADC_CH0START_bm ; //
	
		free(sample_word);
		sample_word=malloc(50);
		sprintf(sample_word,"Dark: %d , Dim: %d , Bright: %d  Hall effect:  close:%d  near: %d  far:%d raw:%d\r",D,I,B,C,A,F, hall);
		USART_send(&Ser,sample_word );
				
}
/* Interrupt Service Handlers */
ISR(USARTC0_RXC_vect){
	Rx_Handler(&Ser);
	//wait_for_it=1;
}

ISR(USARTC0_TXC_vect){
	Tx_Handler(&Ser);	
}

ISR(USARTC0_DRE_vect){
}

ISR(ADCA_CH0_vect){
	//free(sample_word);
	//sample_word=malloc(50);
	light=ADCA.CH0.RES;
	//sprintf(sample_word,"The light value is : %d \n",light);
	//USART_send(&Ser,sample_word );
}

ISR(ADCB_CH0_vect){
	//adc_sample[adccount]=ADCB.CH0.RES;
//	PORTR_OUT^=0x02;

	hall=ADCB.CH0.RES;
}


// call used peripheral setups
void setup_peripherals(){
	cli();
	sys_clock();
//	setup_gpio();
	setup_timer();
	setup_adcA();   //light sensor
	setup_adcB();	//accelerometer on adc0,1,2

	//setup_spi();
	setup_avrx_usart();
	PMIC_CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // enable all priorities
	sei();
}


int main(void)
{	
	PORTR_DIR=0X03;
	PORTE_DIR&=0X08;
	PORTR_OUT=0X0F;
	
	//hall
close[0]=0000;
close[1]=2600;
avg[0]=2800;
avg[1]=3400;
far[0]=3200;
far[1]=4095;

	dark[0]=0;
	dark[1]=900;
	dim[0]=500;
	dim[1]=1600;
	bright[0]=1200;
	bright[1]=4095;

	sample_word=malloc(50);
	setup_peripherals();

while (1)
    {
		
  	if ((B>=80)&& (A>=80)){
			PORTR_OUT=~0X01;
		
		}
		else
		
		 if((I>=80)&&(F>=80))
		{
			PORTR_OUT=~0X02;		
		}
		else if ((D>=90)&&(C>=80)){
			PORTR_OUT=~0X03;
		}
		else{ 
			PORTR_OUT=0X03;
		}

    }
}



void setup_adcA(){
		ADCA_CTRLB |= ADC_RESOLUTION_12BIT_gc;
		ADCA_REFCTRL |= ADC_REFSEL_INTVCC2_gc; // use 3V3 to chip
		ADCA_PRESCALER |= ADC_PRESCALER_DIV512_gc; //peripheral clock/512
		ADCA_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
		
		ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
		ADCA_CTRLA |= ADC_ENABLE_bm; 	
		ADCA_CH0_INTCTRL = ADC_CH_INTMODE_COMPLETE_gc| ADC_CH_INTLVL_HI_gc;	
		ADCA_CTRLA |= ADC_CH0START_bm ; //
		
}


void setup_adcB(){
	ADCB_CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCB_REFCTRL |= ADC_REFSEL_INTVCC2_gc; // use 3V3 to chip
	ADCB_PRESCALER |= ADC_PRESCALER_DIV256_gc; //peripheral clock/512

	ADCB_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
	ADCB_CH0_MUXCTRL =ADC_CH_MUXPOS_PIN0_gc|ADC_CH_MUXNEG_PIN0_gc;//|ADC_CH_MUXPOS_PIN1_gc|ADC_CH_MUXPOS_PIN2_gc;
	ADCB_CTRLA |= ADC_ENABLE_bm; 
	ADCB_CH0_INTCTRL = ADC_CH_INTMODE_COMPLETE_gc| ADC_CH_INTLVL_HI_gc;
	ADCB_CTRLA |= ADC_CH0START_bm ; //
	
}

void sys_clock(){
	//set mcu clock/frequency
	SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
	GetSystemClocks(&sClk, &pClk);
	}
	
void setup_timer(){
	//	1s= 2Mhz/1024= 0x7a1
	//  1s=32m/1024=0x7a12
	TCC0_PER=0X7a1;					//1s
	TCC0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
	
	//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
	//setup_compare_timer();
	TCC0_CTRLA=0x7;
	
}




//Configure USART

void setup_avrx_usart(){
	
	USART_init(&Ser, 0xc0, pClk, (_USART_TXCIL_LO | _USART_RXCIL_LO), 576, -4,_USART_CHSZ_8BIT, _USART_PM_DISABLED, _USART_SM_1BIT);
	USART_buffer_init(&Ser, 160, 180);
	Ser.fInMode = _INPUT_CR ; //| _INPUT_ECHO | _INPUT_TTY;
	Ser.fOutMode = _OUTPUT_CRLF;
	USART_enable(&Ser,(USART_TXEN_bm|USART_RXEN_bm));
	
}



