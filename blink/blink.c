/**
    Kinematics for mobile robots base on two stepper motors
    Artur Bereit 
    bereitartur5@gmail.com
    git: @vAtomowy 
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#include "drives.h"
#include "timers.h" 
#include "uart.h"
#include "circ_buffer.h"


// PICO led pin 
const uint LED_PIN = PICO_DEFAULT_LED_PIN;

// timer to drive task 
TIMER drive_timer;
TIMER uart_rec_timer; 

// right and left drive   
DRIVE axis_L;
DRIVE axis_R;

// uart buff structure
CIRC_BUFF UartRecBuff; 

void Drives_Tasks(void);
void Load_Buffer(char ch);
void Lazy_Parse_Data(void);

int main() {

    stdio_init_all();
     
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    TimerRegisterCallback(&drive_timer, &Drives_Tasks);
    UartRxInterruptRegisterCallback(&Load_Buffer);
    
    InitTimer(&drive_timer, 50); // time in ms

    InitStepper(&axis_L, 13, 12, 11, 10, 0); 
    InitStepper(&axis_R, 2, 3, 4, 5, 0); 

    InitUart();

    while (true) 
    {
        tight_loop_contents();
    }

}

void Drives_Tasks(void){ 
    DrivesTask(&axis_L);
    DrivesTask(&axis_R);
}


// TODO: Zrobić parsowanie !!! ------------------------------------------------------> 
void Load_Buffer(char ch)
{ 
    CircBufferPut(&UartRecBuff, ch);
    if(ch == '\n')
    { 
        Lazy_Parse_Data();
    }
}
void Lazy_Parse_Data(void)
{ 
    char rcv_data;
    char tab[32] = {0};
    char value[32] = {' '};
    uint inx = 0; 
    int size =0;
    do{ 
        int ex = CircBufferGet(&UartRecBuff, &rcv_data);
        tab[inx] = rcv_data;
        inx++; 
    } while( rcv_data != '\n' ); 
    
    // change state & half/full step
    if(inx == 2)
    { 
        switch(tab[0])
        {
            case 'r':  // set run state
                SetState(&axis_L, RUN);
                SetState(&axis_R, RUN);
                UartPuts("Run !\n\r");
                break;
            case 's' : // set stop state
                SetState(&axis_L, STOP);
                SetState(&axis_R, STOP);
                UartPuts("Stop !\n\r");
                break;
            case 'S' : // set SAFETY state
                SetState(&axis_L, SAFETY);
                SetState(&axis_R, SAFETY);
                UartPuts("Safety_Stop !\n\r");
                break;
            case 'F' : // set FULLSTEP
                SetDivStep(&axis_R, FULLSTEP);
                SetDivStep(&axis_L, FULLSTEP);
                UartPuts("Fullstep_mode !\n\r");
                break;
            case 'H' : // set HALFSTEP
                SetDivStep(&axis_R, HALFSTEP);
                SetDivStep(&axis_L, HALFSTEP);
                UartPuts("Halfstep_mode  !\n\r");
                break;
            default:
            UartPuts("Blad komunikacji E01!\n\r");
            
        }
    }
    else if(inx == 4)
    {
        if(tab[0] == 'p' && tab[1] == '=' ) 
        {  
            uart_putc(UART_ID, tab[0]);
            uart_putc(UART_ID, tab[1]);
            uart_putc(UART_ID, tab[2]);

            if(tab[2] == '?')
            { 
                size = sprintf(value, "Pose is setting on:%d\n\r", GetPose(&axis_L));
                UartPuts(value);  
            }else{ 
                SetPose(&axis_L, (tab[2] - '0')*10); 
                SetPose(&axis_R, (tab[2] - '0')*10);
                size = sprintf(value, "Pose is setting on:%d\n\r", GetPose(&axis_L));
                UartPuts(value); 
            }
            

        }
        else if(tab[0] == 'v' && tab[1] == '=' ) 
        {  
            if(tab[2] == '?')
            { 
                size = sprintf(value, "Velocity is setting on:%d\n\r", GetVel(&axis_L));
                UartPuts(value); 
            }else{ 
                SetVel(&axis_L, (tab[2] - '0')*10); 
                SetVel(&axis_R, (tab[2] - '0')*10);
                size = sprintf(value, "Velocity is setting on:%d\n\r", GetVel(&axis_L));
                UartPuts(value); 
            }
        }
        else 
        { 
            UartPuts("Blad komunikacji E02!\n\r");
        }
    }
    else 
    { 
        UartPuts("Blad komunikacji E03!\n\r");
    }
    
}
