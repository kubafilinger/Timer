ISR(TIMER0_OVF_vect)
{
	static volatile int src_nr = 0, counter = 0;
	
	if(counter++ == 10)
	{
		src_nr = (src_nr + 1) % total_displays;
		
		*display_port = (led_type & KATODA) ? (1 << src_nr) : ~(1 << src_nr);
		*digit_port = number[src_nr];
	
		counter = 0;
	}
}

void LEDInit(uint8_t total_displays_f, uint8_t led_type_f, volatile uint8_t *digit_ddr_wsk, volatile uint8_t *digit_port_wsk, volatile uint8_t *display_ddr_wsk, volatile uint8_t *display_port_wsk)
{
	total_displays = total_displays_f;
	led_type = led_type_f % 2;
	digit_ddr = digit_ddr_wsk;
	digit_port = digit_port_wsk;
	display_ddr = display_ddr_wsk;
	display_port = display_port_wsk;
	
	*digit_ddr = 0xFF;
	*digit_port = 0;
	*display_ddr = 0;
	*display_port = 0xFF;
	
	int i;
	for(i = 0; i < total_displays; i++)
	{
		*display_ddr |= (1 << i);
	}
	
	position = 0;
	set_timer0();
}

void LEDSetValue(char *val)
{
	int i;
	
	for(i = 0; i < total_displays; i++)
	{
		if(val[i] == '\0')
			break;
		
		number[(i + position) % total_displays] = (led_type & KATODA) ? ~convertChar(val[i]) : convertChar(val[i]);
	}
}

void LEDSetPosition(uint8_t pos)
{
	position = pos % MAX_NUM_DISPLAY;
}

void LEDSetNumber(int nr)
{
	char s[MAX_NUM_DISPLAY + 1]; // +1 end char
	sprintf(s, "%d\0", nr);
	LEDSetValue(s);
}

void LEDSetNumberWithZero(int nr)
{
	char s[MAX_NUM_DISPLAY + 1]; // +1 end char
	
	if(nr < 10)
		sprintf(s, "0%d\0", nr);
	else
		sprintf(s, "%d\0", nr);
	
	LEDSetValue(s);
}

void clearDisplay()
{
	int i;
	
	for(i = 0; i < total_displays; i++)
		number[i] = (led_type & KATODA) ? ~0 : 0;
}

uint8_t convertChar(char s)
{
	uint8_t result = 0;
	
	switch(s)
	{
		case '0':
			result = 0b00111111;
			break;
		
		case '1':
			result = 0b00000110;
			break;
		
		case '2':
			result = 0b01011011;
			break;
		
		case '3':
			result = 0b01001111;
			break;
		
		case '4':
			result = 0b01100110;
			break;
		
		case '5':
			result = 0b01101101;
			break;
		
		case '6':
			result = 0b01111101;
			break;
		
		case '7':
			result = 0b00000111;
			break;
		
		case '8':
			result = 0b01111111;
			break;
		
		case '9':
			result = 0b01101111;
			break;
			
		default:
			result = 0;
			break;
	}
	
	return result;
}

void set_timer0()
{
	TCCR0 = 1 << CS01;
	TIMSK |= 1 << TOIE0;
}