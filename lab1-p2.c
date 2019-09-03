/*
 * LAB1 part2
 * 8/28/2019
 * Authors : Sean Gow & Denice Hickethier 

 */ 

#include <avr/io.h>

// CPU Frequency 2Mhz =2*10^6Hz
// 1000ms=1second
#define CYCLES_MS 2000  // rough estimate of clocks per ms  =cpufreq/1000ms
#define CAL_FACTOR 

//wait_ms: this function creates a poor timer by wasting clock cycles by counting
void wait_ms(uint16_t delay_in_ms);

/*
main program
Count in binary to 15 in 8 seconds using the LED lights on a xmega-b1 xplained demo board
We use a sfr(special function register) address to control the leds,
the sfr addresses we used are predefined in the avr/io.h file as PORTB_DIR, and PORTB_OUT
assigning values to these address locations controls the direction and output of the mcu portb pins

The led's are located on the high nibble which we want to set to output PORTB_DIR = 0xf0

LEDs are active low, ie: binary is inverted -> we must count down to count up...
counter -> LEDs
15=1111    0000  -> displays as binary 0
14=1110	   0001   ->displays as binary 1
13=1101    0010   ->...      as binary 2
...
...

*/
int main(void)
{
	uint8_t count=15; //start count at 15 
	PORTB_DIR=0xf0;   //set direction of port b pins to out
	PORTB_OUT=0xf0;   //set initial state of portB pins off, active low
	
	optimizethis:     //horrible use of goto and while loop to bypass optimizations
					  //future: "alt+f7" ->toolchain ->optimizations = none
	while(count>0){	  //while we have something to count
		wait_ms(500); //wait ~500ms,          optimized away unless optimizations disabled
		count--;      //decrement counter to increment LEDs
		PORTB_OUT=    count << 4 ; //select first 4 bits of count and bitshift left by 4 bits and write to portb out
	}
	count=15;        //reset count to 15 for LEDs to start again at zero
	goto optimizethis;//abort, hide, run away, divide by zero, shafsma no likey
}

void wait_ms(uint16_t delay_in_ms){
	uint32_t i	; //counter unsigned int 32 bits wide, large enough for 2000counts*500ms
	for ( i=0;i<CYCLES_MS*delay_in_ms;i++){
		;//do nothing
	}
}
