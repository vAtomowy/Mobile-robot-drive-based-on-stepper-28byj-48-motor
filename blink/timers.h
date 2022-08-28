
/* timers.h */ 

typedef struct timer_struct
{   
    struct repeating_timer tm;
    struct repeating_timer* tm_ptr;
    void(*TimerInterrupt)(void);
} TIMER;

// Function to get time for drives library: 
void InitTimer(TIMER* t, int loop_time);
void TimerCallback(TIMER *t);
void TimerRegisterCallback(TIMER* t, void* Callback);