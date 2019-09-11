
 * lab2-pt4.c
 * user input
 * increase/decrease timer interval by button push
 *
 * Created: 9/10/2019 11:21:07 AM
 * Author : Sean
 */ 




/*
Table 4-6. J4 I/O expansion header.
Pin J4 XMEGA pin Shared with onboard functionality
1 OC0A TIM / OC0LA Split TIM            PE0 QTOUCH0 (PE0)
2 OC0B TIM / OC0LB Split TIM XCK0 USART PE1 QTOUCH1 (PE1)
3 OC0C TIM / OC0LC Split TIM RXD0 USART PE2 QTOUCH2 (PE2)
4 OC0D TIM / OC0LD Split TIM TXD0 USART PE3 QTOUCH3 (PE3)
5 OC0A Swap TIM / OC0HA Split TIM PE4 Power LED (PE4)*/

#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint8_t input_changed;
volatile uint8_t saved_input=0XFF;
//BADISR_vect catchall  
ISR(      RTC_OVF_vect){
			PORTB_OUT -= 0X10 ;
	}
ISR( PORTE_INT0_vect ){

input_changed= ~PORTE_IN ;
	if (input_changed &0x01){
		;
	//	PORTB_OUT=~0X10;
		RTC_PER+=100;
			}
else	if (input_changed &0x02){
		;
		//PORTB_OUT=~0X20;
		RTC_PER-=100;
	}
else	if (input_changed &0x04){
		;
	//	PORTB_OUT=~0X40;
		RTC_PER=500;
	}
else	if (input_changed &0x08){
		;
		//PORTB_OUT= 0X80;
		RTC_PER=0;
	}
	//START RTC CLOCK

	RTC_CNT = 0x01;
	

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
		
		
}
}
