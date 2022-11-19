
/* timers.c */ 

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#include "timers.h" 
// Function to get time for drives library: 
void TimerCallback(TIMER *t)
{ 
    // callback to timer interrupt 
    t->TimerInterrupt();
}

void InitTimer(TIMER* t, int loop_time) 
{
    t->tm_ptr = &(t->tm);
    add_repeating_timer_ms(loop_time, TimerCallback, NULL, t->tm_ptr);
}

void TimerRegisterCallback(TIMER* t, void* Callback)
{
	t->TimerInterrupt = Callback;
}

void SetTimerTime(TIMER* t, int loop_time){ 
    t->tm.delay_us = loop_time*1000;
}


