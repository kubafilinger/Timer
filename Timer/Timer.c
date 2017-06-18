/*
 * Timer.c
 *
 * Created: 2017-05-17 08:12:05
 *  Author: Ghost
 */ 

#define F_CPU 8000000L

#define SET_END_MOD 2
#define SET_SEC_MOD 2
#define SET_MIN_MOD 1
#define SET_HOUR_MOD 0
#define RELAY 0

#include "led_driver/led_driver.h"
#include "clock_driver/clock_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	// przekaznik
	DDRB = 0x00000001;
	PORTB = 0;
	
	uint8_t set_time_mode = 1; // set default: hour_mod
	uint8_t modyfi_mode = 1; // set default: true
	uint8_t dot = 0;
	
	LEDInit(4, 1, &DDRD, &PORTD, &DDRC, &PORTC);
	CLOCKInit();
	CLOCKStatus |= (1 << REVERSE_CLOCK);
	setHours(12);
	
	sei();
	
	clearDisplay();
	
	while(1)
	{
		// finish time
		if(CLOCKTime == 0 && CLOCKIsActive())
		{
			CLOCKStop();
			
			set_time_mode = 1;
			modyfi_mode = 1;
			
			PORTB &= ~(1 << RELAY); // RELAY OFF
		}
		
		// LED
		LEDSetPosition(0);
		LEDSetNumberWithZero(getHours());
		
		LEDSetPosition(1);
		
		if(CLOCKIsActive())
		{
			if(CLOCKStatus & (1 << CHANGE_STATUS))
			{
				dot += 1;
				dot %= 2;
				
				CLOCKStatus &= ~(1 << CHANGE_STATUS);
			}
			
			(dot % 2) ? LEDDot(PUT_DOT) : LEDDot(DEL_DOT);
		}
		else
		{
			LEDDot(PUT_DOT);
		}
		
		LEDSetPosition(2);
		LEDSetNumberWithZero(getMinutes());
		
		// Switches
		if(!(PINB & (1 << 2)) && !CLOCKIsActive()) // UP
		{
			if(!CLOCKIsActive())
			{
				if(set_time_mode & (1 << SET_HOUR_MOD))
				{
					setHours(getHours() + 1);
				}
				else if(set_time_mode & (1 << SET_MIN_MOD))
				{
					setMinutes(getMinutes() + 1);
				}
				else if(set_time_mode & (1 << SET_SEC_MOD))
				{
					setSeconds(getSeconds() + 1);
				}

				modyfi_mode = 1;
			}
			
			_delay_ms(20);
			while(!(PINB & (1 << 2)));
		}
		else if(!(PINB & (1 << 3)) && !CLOCKIsActive()) // DOWN
		{
			if(!CLOCKIsActive())
			{
				if(set_time_mode & (1 << SET_HOUR_MOD))
				setHours(getHours() - 1);
				else if(set_time_mode & (1 << SET_MIN_MOD))
				setMinutes(getMinutes() - 1);
				else if(set_time_mode & (1 << SET_SEC_MOD))
				setSeconds(getSeconds() - 1);

				modyfi_mode = 1;
			}
			
			_delay_ms(20);
			while(!(PINB & (1 << 3)));
		}
		else if(!(PINB & (1 << 4))) // OK, STOP, START
		{
			if(CLOCKIsActive()) // STOP
			{
				CLOCKStop();
				
				set_time_mode = 1;
				modyfi_mode = 1;
			}
			else
			{
				if(modyfi_mode) // CHANGE SET TIME
				{
					set_time_mode <<= 1;
					
					if(set_time_mode & (1 << SET_END_MOD))
					{
						set_time_mode = 0;
						modyfi_mode = 0;
						
						CLOCKStart();
						PORTB |= (1 << RELAY); // RELAY ON
					}
				}
				else { // START
					CLOCKStart();
					PORTB |= (1 << RELAY); // RELAY ON
				}
			}
			
			_delay_ms(20);
			while(!(PINB & (1 << 4)));
		}
	}
}