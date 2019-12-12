/*
 *
 * Created: 12/3/2019 
 *  Author: Sean Gow & Denice Hickethier
 */ 



#include "setup.h"

#define F_CPU 32000000L

/* clock initialization with digital frequency locked loop 
lots of timing changes and protected registers
set clock at 32mhz and use dfll to eliminate future clock issues
enable all clocks sources
*/
void system_clocks(){
	
	//change osc
	//enable rtc with 32k
	CLK.RTCCTRL=CLK_RTCSRC_RCOSC_gc|1;
	
	//set external oscillator to 32k crystal
	OSC.XOSCCTRL=OSC_XOSCSEL_32KHz_gc|OSC_X32KLPM_bm|OSC_XOSCPWR_bm;
	//32k cal register to production calibration do not use with dfll
	//OSC.RC32KCAL=PRODSIGNATURES_RCOSC32K;
	
	//set protected register flag to modify
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
	CLK.PSCTRL=CLK_PSADIV_1_gc| CLK_PSBCDIV_1_1_gc;
   //systemclock output on pin 7 | 
	//PORTCFG.CLKEVOUT= PORTCFG_CLKOUT_PC7_gc|PORTCFG_CLKOUTSEL_CLK1X_gc;
	
	//PORTCFG.CLKEVOUT= PORTCFG_CLKEVPIN_PIN7_gc|PORTCFG_CLKOUT_PC7_gc|PORTCFG_CLKOUTSEL_CLK1X_gc|PORTCFG_EVOUT_PC7_gc;
	
}
/*rtc32 unused 
*/
/*
void rtc32_clock(){
	
	//rtc clock
	RTC32.CTRL=0;
	RTC32.SYNCCTRL=0;
	RTC32.INTCTRL=0x3;
	RTC32.INTFLAGS=PMIC_HILVLEN_bm;
	RTC32.CNT=0;
	while ( RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm );
	RTC32.PER=1024;
	//RTC32.COMP;
	//rtc battery  //must be turned on before rtc enabled
	VBAT.CTRL=VBAT_ACCEN_bm|VBAT_XOSCEN_bm|VBAT_XOSCSEL_bm;
	//VBAT.STATUS;
	//VBAT.BACKUP0;
	//VBAT.BACKUP1;
	
	RTC32.CTRL=RTC32_ENABLE_bm;	
}*/

//PWM ON PORTD PIN0-> J4-pin #5
//pwm initialization
void  tcd_pwm(){
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
	TCD0_CTRLA=0x7;
	TCD0_CTRLB|=TC0_CCAEN_bm|TC0_CCBEN_bm|TC_WGMODE_SINGLESLOPE_gc;
	PORTD.DIR|=0x01;
	tcd0_pwm_control(20,1000,5);
	
}

/*period calculator for a given time, and t_scale is the units*/
unsigned long calc_period(uint8_t time, uint16_t t_scale){
	uint8_t i;
	uint16_t prescale=1024;
	unsigned long per=0;
	
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
	for (i=7;i>(TCD0_CTRLA & 0x7);i--){  
		prescale=prescale/2;
		if (i>4) //prescale decrement by factor of 4 if >4, or 2 <=4
			prescale=prescale/2;
		else if (i==4) 
			prescale=prescale/4; 
    }
	per=F_CPU/prescale*time/t_scale;
	return per;	
}


/*pwm for adjusting ESC/drone motor speed controller using servo like signaling */
void tcd0_pwm_control(uint8_t time, uint16_t t_scale, float duty_cycle){
	TCD0_PER=calc_period(time,t_scale)+4;
	TCD0_CCA=(TCD0_PER*duty_cycle)/100;	
	TCD0_CCB=(TCD0_PER*(100-duty_cycle))/100;	
	
}


void gpio_setup(){
	
	PORTC_DIR|=0xff;
	PORTB_DIR|=0x10;
	PORTR_DIR|=0X01;
	PORTE_DIR=0;
	

	//simple hall effect sensor	trigger, needs filtering/ converted to analog comparator
    //filtering worked using onboard analog low-pass filter
	PORTA_OUT=0X00;
	
	PORTA.PIN2CTRL = PORT_ISC_FALLING_gc|PORT_OPC_PULLDOWN_gc;
	PORTA.INT0MASK = PIN2_bm;
	PORTA.INTCTRL = PORT_INT0LVL_HI_gc;
	PMIC.CTRL |= PMIC_HILVLEN_bm |PMIC_LOLVLEN_bm|PMIC_MEDLVLEN_bm;
	
	//end hall effect
	
	
}

//master spi
/*Setup usart in master mode for dma support to free cpu cycles
regular spi doesnt support dma according to manual
*/
void setup_spi(){
//	PORTC.DIR|=PIN4_bm|PIN5_bm|PIN7_bm;;
	//PORTC.PIN5CTRL=PORT_OPC_PULLUP_gc;
	//SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm |SPI_MODE_2_gc|SPI_DORD_bm;
	PORTC.DIR=PIN1_bm|PIN3_bm;
	PORTC.OUT=0|PIN2_bm;
	USARTC0.CTRLB=USART_TXEN_bm;
	//USARTC0.CTRLB=USART_DREIF_bm;
	USARTC0.CTRLC= USART_CMODE_MSPI_gc|USART_CHSIZE_8BIT_gc;	//MASTER SPI

}

/*WIP: DMA transfers to free cpu up to generate next 88bytes per position for dynamic pattern
TODO: Find out why leds dont like dma transfers
*/
/*
void setup_dmac(uint32_t src, uint32_t dst){
	DMA.CTRL=DMA_ENABLE_bm;
	DMA.CH0.CTRLA =DMA_CH_BURSTLEN_1BYTE_gc|DMA_CH_REPEAT_bm;//DMA_CH_SINGLE_bm;
	//DMA.CH0.ADDRCTRL= DMA_CH_SRCRELOAD_BLOCK_gc;
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc |  DMA_CH_SRCDIR_INC_gc;
	DMA.CH0.ADDRCTRL |= DMA_CH_DESTRELOAD_BURST_gc | DMA_CH_DESTDIR_FIXED_gc;
	DMA.CH0.TRIGSRC  = DMA_CH_TRIGSRC_USARTC0_DRE_gc;  //TODO: FIGURE OUT HOW TO TRIGGER USING EVENTS/TIMER AND USART_DRE UNTIL TRANSACTION IS COMPLETE 
	DMA.CH0.REPCNT=88;
	DMA.CH0.TRFCNT =88;
	DMA.CH0.SRCADDR0=  (uint8_t)(( (uint32_t) src)) & 0xFF;;
	DMA.CH0.SRCADDR1= (uint8_t)(( (uint32_t) src) >> 8 ) & 0xFF;
	DMA.CH0.SRCADDR2= (uint8_t)(( (uint32_t) src) >> 16 ) & 0xFF;
	//uint32_t dst=&SPIC_DATA;
	DMA.CH0.DESTADDR0=(uint8_t)(( (uint32_t) dst)) & 0xFF;;
	DMA.CH0.DESTADDR1=(uint8_t)(( (uint32_t) dst) >> 8 ) & 0xFF;;
	DMA.CH0.DESTADDR2=(uint8_t)(( (uint32_t) dst) >> 16 ) & 0xFF;;
	DMA.CH0.CTRLB=DMA_CH_TRNINTLVL_LO_gc;
	//DMA.CH0.CTRLA|=DMA_CH_TRFREQ_bm; //MANUAL TRIGGER	    
	DMA_CH0_CTRLA|= DMA_CH_ENABLE_bm;
}
*/

/*TODO:
trigger timer sequence to start at end of 1 revolution
timer to send on/ off signals to leds using dma to transmit to usart master spi
//had issues with lights not working when using dma triggered pulses

*/
void tce_setup(){
	TCE0_PER=0XFFFF;
	//TCE0_CCA=TCE0_PER/64;
	TCE0.CTRLA|=TC_CLKSEL_DIV1024_gc;
//	TCE0.CTRLB|=TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm|TC0_CCDEN_bm; //CAPTURE
	//TCE0.CTRLC;//COMPARE
//	TCE0.CTRLD|= TC_EVACT_CAPT_gc;
	TCE0.INTCTRLA= PMIC_MEDLVLEN_bm;
	TCE0.INTCTRLB=PMIC_MEDLVLEN_bm<<TC0_CCAEN_bp|PMIC_MEDLVLEN_bm<<TC0_CCBEN_bp|PMIC_MEDLVLEN_bm<<TC0_CCBEN_bp|PMIC_MEDLVLEN_bm<<TC0_CCDEN_bp;
	
	TCE1_PER=0XFFFF;
	//TCE0_CCA=TCE0_PER/64;
	TCE1.CTRLA|=TC_CLKSEL_DIV1024_gc;
	TCE1.CTRLB|=TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm|TC0_CCDEN_bm; //CAPTURE
	//TC10.CTRLC;//COMPARE
//	TC10.CTRLD|= TC_EVACT_CAPT_gc;
	TCE1.INTCTRLA= PMIC_MEDLVLEN_bm;
	TCE1.INTCTRLB=0xff;
}

//capture event on porta pin 2, trigger capture on tcf0, trigger event for porta pin 2

void eventtrigger(){
	//PORTA;
	
	PORTA.PIN2CTRL|=PORT_ISC_FALLING_gc;
	PORTD.DIRCLR=0X04;
	
	TCF0.CTRLD=(uint8_t)TC_EVSEL_CH7_gc|TC_EVACT_CAPT_gc;
	TCF0.CTRLB|=TC0_CCAEN_bm;
	TCF0.PER=0XFFFFFF;
	//TCF0.INTCTRLA=PMIC_MEDLVLEN_bm;
	TCF0.CTRLA=TC_CLKSEL_DIV1024_gc;
	
	EVSYS.CH0MUX=EVSYS_CHMUX_PORTA_PIN2_gc;
	//TCF0.INTCTRLB=0X3;
}