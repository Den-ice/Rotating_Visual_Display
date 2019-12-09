#include "setup.h"

#define F_CPU 32000000L

/* clock initialization with digital frequency locked loop */
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
/*rtc32 enabled
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
void  tcd_pwm(){
		//	1s= 2Mhz/1024= 0x7a1
	//  1s=32m/1024=0x7a12
	//TCD0_PER=0X7a12;					//1s
	//TCD0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
	//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
	TCD0_CTRLA=0x7;
	TCD0_CTRLB|=TC0_CCAEN_bm|TC0_CCBEN_bm|TC_WGMODE_SINGLESLOPE_gc;
	PORTD.DIR|=0x01;
	//TCD0_INTCTRLB=255;
	//20ms with 5% dutycycle
	tcd0_pwm_control(20,1000,5);
	
}
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
	
	/*  SETUP^^^*/
	

	
	
	//trigger edge
	//PORTCFG_MPCMASK=0x01|0x02|0x04|0x08|0x10|0x20;  //0:5
	//PORTC_PIN0CTRL= PORT_OPC_PULLUP_gc |    PORT_ISC_FALLING_gc; //SET MPCMASKED PINS TO 0X02 sense low (capacitive touch buttons are active low)
	
	
	//interrupts
	PORTD_INT0MASK=0x01|0x02|0x04|0x08|0x10|0x20;  //turn on interrupts for ports 0:5
	PORTD_INTCTRL= PMIC_MEDLVLEN_bm;
	
	/*
	//simple hall effect sensor	trigger, needs filtering/ converted to analog comparator
	PORTA_OUT=0x10;
	
	PORTA.PIN4CTRL = PORT_ISC_RISING_gc|PORT_OPC_TOTEM_gc;
	PORTA.INT0MASK = PIN4_bm;
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc;
	
	//end hall effect
	*/
	//simple hall effect sensor	trigger, needs filtering/ converted to analog comparator
	PORTA_OUT=0X00;
	
	PORTA.PIN2CTRL = PORT_ISC_FALLING_gc|PORT_OPC_PULLDOWN_gc;
	PORTA.INT0MASK = PIN2_bm;
	PORTA.INTCTRL = PORT_INT0LVL_HI_gc;
	PMIC.CTRL |= PMIC_HILVLEN_bm |PMIC_LOLVLEN_bm|PMIC_MEDLVLEN_bm;
	
	//end hall effect
	
	
	
	
	//input generated events
	
	
	
}

//master spi
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


void setup_dmac(uint32_t src, uint32_t dst){
	DMA.CTRL=DMA_ENABLE_bm;

	DMA.CH0.CTRLA =DMA_CH_BURSTLEN_1BYTE_gc|DMA_CH_SINGLE_bm;
	
	//DMA.CH0.ADDRCTRL= DMA_CH_SRCRELOAD_BLOCK_gc;
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc |  DMA_CH_SRCDIR_INC_gc;
	DMA.CH0.ADDRCTRL |= DMA_CH_DESTRELOAD_NONE_gc | DMA_CH_DESTDIR_FIXED_gc;
	DMA.CH0.TRIGSRC  |= DMA_CH_TRIGSRC_USARTC0_DRE_gc;  //SOFTWARE TRIGGER CHANGE TO TIMER TRIGGER
	DMA.CH0.REPCNT=1;
	DMA.CH0.TRFCNT =88;
	//DMA.CH0.REPCNT=100;
	//FIX ADDRESSES
	//uint32_t src=&data;
	DMA.CH0.SRCADDR0=  (uint8_t)(( (uint32_t) src)) & 0xFF;;
	DMA.CH0.SRCADDR1= (uint8_t)(( (uint32_t) src) >> 8 ) & 0xFF;
	DMA.CH0.SRCADDR2= (uint8_t)(( (uint32_t) src) >> 16 ) & 0xFF;
	//uint32_t dst=&SPIC_DATA;
	DMA.CH0.DESTADDR0=(uint8_t)(( (uint32_t) dst)) & 0xFF;;
	DMA.CH0.DESTADDR1=(uint8_t)(( (uint32_t) dst) >> 8 ) & 0xFF;;
	DMA.CH0.DESTADDR2=(uint8_t)(( (uint32_t) dst) >> 16 ) & 0xFF;;
	

	DMA_CH0_CTRLA|= DMA_CH_ENABLE_bm;
	DMA.CH0.CTRLB=DMA_CH_TRNINTLVL_LO_gc;
	//DMA.CH0.CTRLA|=DMA_CH_TRFREQ_bm; //MANUAL TRIGGER	
//	while(!(DMA.CH0.CTRLB&DMA_CH_TRNIF_bm));
	//while(1);
}


void analog_comparator(){
//WASNT NEEDED
//USED ON BOARD ANALOG FILTER
	
}

void tce_setup(){
	
	TCE0_PER=0X7a12;
	TCE0_CCA=TCE0_PER/64;
	TCE0.CTRLA|=TC_CLKSEL_DIV256_gc;
	TCE0.CTRLB|=TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm|TC0_CCDEN_bm; //CAPTURE
	TCE0.CTRLC;//COMPARE
	TCE0.CTRLD|= TC_EVACT_CAPT_gc;
	TCE0.INTCTRLA= PMIC_MEDLVLEN_bm;
	
	TCE0.INTCTRLB=TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCBEN_bm|TC0_CCDEN_bm;

	
	
	
	
	
	
	//TCE0.INTCTRLB= PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;	

	//TCD0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
	/*Task: Configure TCC0 with Input Capture Channel A enabled. The Input Capture is
triggered by the falling edge of PC0.
3. Configure PC0 for input, triggered on falling edge.
4. Select PC0 as multiplexer input for event channel 0.
5. Configure TCC0 for input capture by setting event source and event action to
“Input capture” in CTRLD.
6. Enable input capture channel A by setting the CCAEN bit in CTRLB.
7. Start the TC by selecting a clock source (CLKSEL in CTRLA).
8. Wait for Input Capture Interrupt Flag A in INTFLAGS to be set.
9. Read input capture value from the CCA[H:L] register.
10. Go to step 6. 
*/
	

	
}



void tcf_setup(){
	
	TCF0.CTRLA|=TC_CLKSEL_DIV256_gc;
	TCF0.CTRLB|=TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm|TC0_CCDEN_bm;
	TCF0.CTRLC;//COMPARE
	TCF0.CTRLD|= TC_EVACT_CAPT_gc;
	TCF0.INTCTRLA= PMIC_MEDLVLEN_bm;
	TCF0.INTCTRLB=
	TCF0_PER=calc_period(10,1000);
	TCF0_CCA=TCE0_PER/2;
	//TCE0.INTCTRLB= PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;	

	//TCD0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
	/*Task: Configure TCC0 with Input Capture Channel A enabled. The Input Capture is
triggered by the falling edge of PC0.
3. Configure PC0 for input, triggered on falling edge.
4. Select PC0 as multiplexer input for event channel 0.
5. Configure TCC0 for input capture by setting event source and event action to
“Input capture” in CTRLD.
6. Enable input capture channel A by setting the CCAEN bit in CTRLB.
7. Start the TC by selecting a clock source (CLKSEL in CTRLA).
8. Wait for Input Capture Interrupt Flag A in INTFLAGS to be set.
9. Read input capture value from the CCA[H:L] register.
10. Go to step 6. 
*/
	

	
}
