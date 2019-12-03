
/*  SETUP.H^^^*/
#ifndef SETUP_H_
#define SETUP_H_


#include <avr/io.h>
#include <avr/interrupt.h>

void system_clocks();
void rtc32_clock();
void timer();
void cap_comp();
void gpio_setup();
#endif /* SETUP_H_ */

/*  SETUP^^^*/
#include "setup.h"
/* clock initialization with digital frequency locked loop */
void system_clocks(){
	cli();
	//change osc
	//enable rtc with 32k
	CLK.RTCCTRL=CLK_RTCSRC_RCOSC_gc|1;
	//set external oscillator to 32k crystal
	OSC.XOSCCTRL=OSC_XOSCSEL_32KHz_gc|OSC_X32KLPM_bm|OSC_XOSCPWR_bm;
	//use 32k cal register to production calibration do not use with dfll
	//OSC.RC32KCAL=PRODSIGNATURES_RCOSC32K;
	//INTERNAL CALIBRATED DIGITAL? OSCILLATOR
	CCP=CCP_IOREG_gc;
	OSC.PLLCTRL=OSC_PLLSRC_RC32M_gc|OSC_PLLFAC3_bm; //0x04 32mhz*4=128mhz?
	//calibrate 32mhz to 32k crystal
	OSC.DFLLCTRL=OSC_RC32MCREF_RC32K_gc|OSC_RC2MCREF_RC32K_gc;
	OSC.CTRL=OSC_RC32MEN_bm|OSC_XOSCEN_bm|OSC_PLLEN_bm|OSC_RC32KEN_bm|OSC_FRQRANGE_04TO2_gc;
	while(!(OSC_STATUS & (OSC_RC32MRDY_bm|OSC_PLLRDY_bm|OSC_RC2MRDY_bm )));
	CCP=CCP_IOREG_gc;	//signal protected ioreg change
	CLK.RTCCTRL|=CLK_RTCEN_bm;
	while(!(OSC_STATUS & (OSC_RC32MRDY_bm|OSC_PLLRDY_bm|OSC_RC2MRDY_bm )));
	CCP=CCP_IOREG_gc; 	//signal protected ioreg change
	//SET DEFAULT SYSCLOCK
	CLK_CTRL=CLK_SCLKSEL_RC32M_gc;
	while(!(OSC_STATUS & OSC_RC32KRDY_bm));
	DFLLRC2M.CTRL=DFLL_ENABLE_bm;
	DFLLRC32M.CTRL=DFLL_ENABLE_bm;
	CCP=CCP_IOREG_gc;
	//CHANGE CLOCK PRESCALERS
	CLK.PSCTRL=CLK_PSADIV_1_gc| CLK_PSBCDIV_2_2_gc;
	
	
	//systemclock output on pin 7 |
	PORTCFG.CLKEVOUT= PORTCFG_CLKEVPIN_PIN7_gc|PORTCFG_CLKOUT_PC7_gc|PORTCFG_CLKOUTSEL_CLK4X_gc|PORTCFG_EVOUT_PC7_gc;
}
/*turn on rtc32 */
void rtc32_clock(){
	
	//rtc clock
	RTC32.CTRL;
	RTC32.SYNCCTRL;
	RTC32.INTCTRL;
	RTC32.INTFLAGS;
	RTC32.CNT;
	RTC32.PER;
	RTC32.COMP;

	//rtc battery  //must be turned on before rtc enabled
	VBAT.CTRL=VBAT_ACCEN_bm|VBAT_XOSCEN_bm|VBAT_XOSCSEL_bm;
	//VBAT.STATUS;
	//VBAT.BACKUP0;
	//VBAT.BACKUP1;
	
	RTC32.CTRL=RTC32_ENABLE_bm;
}

//lab4 pt2
void  timer(){
		//	1s= 2Mhz/1024= 0x7a1
	//  1s=32m/1024=0x7a12
	TCC0_PER=0X7;					//1s
	TCC0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
	
	//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
	cap_comp();
	TCC0_CTRLA=0x7;
	
}
void cap_comp(){
		
	//set compare counters			
	TCC0_CCA=TCC0_PER/8;
	TCC0_CCB=TCC0_PER/4;
	TCC0_CCC=TCC0_PER/2;

	TCC0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
	TCC0_INTCTRLB|=PMIC_MEDLVLEN_bm<<TC0_CCAINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCBINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;	

}

void gpio_setup(){
	//pin directions
	PORTC_DIR=0xff;
	
	//pull ups/downs
	
	
	
	//trigger edge
	PORTCFG_MPCMASK=0x01|0x02|0x04|0x08|0x10|0x20;  //0:5
	PORTC_PIN0CTRL= PORT_OPC_PULLUP_gc |    PORT_ISC_FALLING_gc; //SET MPCMASKED PINS TO 0X02 sense low (capacitive touch buttons are active low)
	
	
	//interrupts
	PORTC_INT0MASK=0x01|0x02|0x04|0x08|0x10|0x20;  //turn on interrupts for ports 0:5
	PORTC_INTCTRL= PMIC_HILVLEN_bm;
	
}

void setup_spi(){
	PORTC.DIR|=PIN4_bm|PIN5_bm|PIN7_bm;;
	PORTC.PIN5CTRL=PORT_OPC_PULLUP_gc;
	SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm|SPI_PRESCALER_DIV128_gc|SPI_MODE0_bm;  //| SPI_CLK2X_bm
	SPIC.DATA=0b11001010;  //something to transmit


}
/*Physics 402 Final Project 


*/


#include "setup.h"
//#include "codevisionevents.h"
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
	


/* //Interrupt Service Handlers 
ISR(USARTC0_RXC_vect){
	Rx_Handler(&Ser);
}
*/
/*
ISR(USARTC0_TXC_vect){
	Tx_Handler(&Ser);
}

ISR(USARTC0_DRE_vect){
}

ISR(ADCA_CH0_vect){
}

ISR(ADCB_CH0_vect){
}
*/

ISR( PORTC_INT0_vect ){
}


int main(){
	cli();
	event_system_init();
	system_clocks();
	rtc32_clock();
	timer();
	gpio_setup();
	sei();
}
