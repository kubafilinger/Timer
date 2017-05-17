ISR(TIMER2_OVF_vect)
{
	TCCR2 = (1 << CS22) | (1 << CS20);
	
	if(CLOCKStatus & (1 << ACTIVE_STATUS))
	{
		if(CLOCKStatus & (1 << REVERSE_CLOCK))
		{
			if(CLOCKTime > 0)
			CLOCKTime -= 1;
		}
		else
			CLOCKTime = (CLOCKTime + 1);
		
		if(!(CLOCKStatus & (1 << CHANGE_STATUS)))
			CLOCKStatus |= (1 << CHANGE_STATUS);	
	}
}

int getSeconds()
{
	return (int)((CLOCKTime % HOUR_SECONDS) % MINUTE_SECONDS);
}

int getMinutes()
{
	return (int)((CLOCKTime % HOUR_SECONDS) / MINUTE_SECONDS);
}

int getHours()
{
	return (int)(CLOCKTime / HOUR_SECONDS);
}

void setSeconds(int new_seconds)
{
	int seconds = getSeconds();
	volatile uint32_t tmpTime = CLOCKTime;
	
	new_seconds %= 60;
	
	if(new_seconds < 0)
		return;
	
	tmpTime -= (uint32_t)seconds;
	tmpTime += (uint32_t)new_seconds;
	
	CLOCKTime = tmpTime;
}

void setMinutes(int new_minutes)
{
	int minutes = getMinutes();
	volatile uint32_t tmpTime = CLOCKTime;
	
	new_minutes %= 60;
	
	if(new_minutes < 0)
		return;
	
	tmpTime -= (uint32_t)minutes * MINUTE_SECONDS;
	tmpTime += (uint32_t)new_minutes * MINUTE_SECONDS;
	
	CLOCKTime = tmpTime;
}

void setHours(int new_hours)
{
	int hours = getHours();
	volatile uint32_t tmpTime = CLOCKTime;
	
	if(new_hours < 0)
		return;
		
	tmpTime -= (uint32_t)hours * HOUR_SECONDS;	
	tmpTime += (uint32_t)new_hours * HOUR_SECONDS;
	
	CLOCKTime = tmpTime;
}

void CLOCKReset()
{
	CLOCKTime = 0;
}

void CLOCKStart()
{
	/*
		this must be before set CLOCKStatus, because in otherwise timer will increase its value twice
	*/
	TIMSK |= (1 << TOIE2);
	CLOCKStatus |= (1 << ACTIVE_STATUS);
}

void CLOCKStop()
{
	/*
		this must be before set CLOCKStatus, because in otherwise timer will increase its value twice
	*/
	TIMSK &= ~(1 << TOIE2);
	CLOCKStatus &= ~(1 << ACTIVE_STATUS);
}

int CLOCKIsActive()
{
	if(CLOCKStatus & (1 << ACTIVE_STATUS))
		return 1;
	else
		return 0;
}

void CLOCKInit()
{
	ASSR |= (1 << AS2); // set timer in async mode
	
	TCNT2 = 0;
	TCCR2 = (1 << CS22) | (1 << CS20);
	
	//while(ASSR & 0x07);
}