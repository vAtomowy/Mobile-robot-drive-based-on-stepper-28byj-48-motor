
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#include "drives.h"
#include "uart.h"
#include "circ_buffer.h"  
#include "parse.h"

char message[32];
extern uint time_drive_L;
extern uint time_drive_R;

PARSE_ERRORS ParseData(CIRC_BUFF *circ_buf, DRIVE*drv_1, DRIVE*drv_2) 
{ 
    char rcv_data;
    char tab[32] = {0};
    char value[32] = {' '};
    uint inx = 0; 
    int size = 0;
    float val = 0;
    char buf[10];
    do{ 
        int ex = CircBufferGet(circ_buf, &rcv_data);
        tab[inx] = rcv_data;
        inx++; 
    }while( rcv_data != '\n' ); 

    if(inx > 32)
    {
        // string is too long
        UartPuts("E01\n\r");
        return E01;
    }
    else 
    { 
        char *ParsePtr = strtok(tab, "_");
        if(!(strcmp("drv", ParsePtr) == 0))
        { 
            // header is bad
            UartPuts("E02\n\r");
            return E02;
        }
        else
        {   
            // simplest sort -> sort using white list
            char *ParsePtr2 = strtok(NULL, "=");
            // sprintf(message, "%s\n\r", ParsePtr2);
            // UartPuts(message);

            if((strcmp("POS", ParsePtr2) == 0) || (strcmp("VEL", ParsePtr2) == 0 ))
            { 
                char *ParsePtr3 = strtok(NULL, ",");

                if(strcmp("1", ParsePtr3) == 0)
                { 
                    char *ParsePtr4 = strtok(NULL, "\n");

                    if(strcmp("?", ParsePtr2) != 0)
                    {
                        val = atof(ParsePtr4);
                        if(strcmp("POS", ParsePtr2) == 0)
                        {
                            if(val < 0){ SetDir(drv_1, BACKWARD); }
                            else{ SetDir(drv_1, FORWARD); }
                            SetPose(drv_1, val);
                            //UartPuts("POSE_1 IS SETTING !\n\r"); 
                            UartPuts("OK\n\r");
                        }
                        else if(strcmp("VEL", ParsePtr2) == 0)                   
                        {
                            SetVel(drv_1, val);
                            time_drive_L = (uint)(val);
                            // sprintf(buf,"%d",time_drive_L);
                            // UartPuts(buf);
                            UartPuts("OK\n\r");
                        }
                    }
                    else 
                    { 
                        if(strcmp("POS", ParsePtr2) == 0)
                        {
                            UartPuts(GetPose(drv_1));
                        }
                        else if(strcmp("VEL", ParsePtr2) == 0)                   
                        {
                            UartPuts(GetVel(drv_1));
                        }
                    }
                    return EOK;

                }

                else if(strcmp("2", ParsePtr3) == 0)
                { 
                    char *ParsePtr4 = strtok(NULL, "\n");

                    if(strcmp("?", ParsePtr2) != 0)
                    {                    
                        val = atof(ParsePtr4);

                        if(strcmp("POS", ParsePtr2) == 0)
                        {
                            if(val < 0){ SetDir(drv_2, BACKWARD); }
                            else{ SetDir(drv_2, FORWARD); }
                            SetPose(drv_2, val);
                            UartPuts("OK\n\r");
                        }
                        else if(strcmp("VEL", ParsePtr2) == 0)                   
                        {
                            SetVel(drv_2, val);
                            time_drive_R = (uint)(val);
                            UartPuts("OK\n\r");
                        }
                    } 
                    else 
                    { 
                        if(strcmp("POS", ParsePtr2) == 0)
                        {
                            UartPuts(GetPose(drv_2));
                        }
                        else if(strcmp("VEL", ParsePtr2) == 0)                   
                        {
                            UartPuts(GetVel(drv_2));
                        }
                    }
                    return EOK;
                } 
                else 
                { 
                    return E03;
                    UartPuts("E03\n\r");
                }

            }

            // SIMPLE COMMANDS <-> BOTH DRIVERS  -------------------------------------->  
            else if(strcmp("CHECK\n", ParsePtr2) == 0)
            { 
                // set check OK 
                UartPuts("OK\n\r");
                return EOK;
            }
            else if((strcmp("ENABLE\n", ParsePtr2) == 0))
            { 
                // turn on drivers 
                UartPuts("--\n\r");
                return EOK;
            }
            else if((strcmp("DISABLE\n", ParsePtr2) == 0))
            { 
                // turn off drivers 
                UartPuts("--\n\r");
                //UartPuts("DRIVERS DISABLE!\n\r");
                return EOK;
            }
            else if((strcmp("RUN\n", ParsePtr2) == 0))
            { 
                SetState(drv_1, RUN); 
                SetState(drv_2, RUN);
                gpio_put(PICO_DEFAULT_LED_PIN, 1);
                UartPuts("OK\n\r");
                // UartPuts("SETTING STATE -> RUN\n\r");
                return EOK;
            }
            else if((strcmp("STOP\n", ParsePtr2) == 0))
            { 
                SetState(drv_1, STOP);
                SetState(drv_2, STOP);
                gpio_put(PICO_DEFAULT_LED_PIN, 0);
                UartPuts("OK\n\r");
                // UartPuts("SETTING STATE -> STOP\n\r");
                return EOK;
            }
            else if((strcmp("SAFETY_STOP\n", ParsePtr2) == 0))
            { 
               SetState(drv_1, SAFETY);
               SetState(drv_2, SAFETY);
               UartPuts("OK\n\r");
               while(1){
               gpio_put(PICO_DEFAULT_LED_PIN, 1);
               sleep_ms(100);
               gpio_put(PICO_DEFAULT_LED_PIN, 0);
               sleep_ms(300);
               }; // Ha ! pulapka skurczybyki !
               // UartPuts("SETTING STATE -> SAFETY_STOP\n\r");
               // UartPuts("TURN OFF DRIVES -> GO RESET -> AND REBOOT DEVICE !!!\n\r");
               return EOK;
            }
            else if((strcmp("FSTEP\n", ParsePtr2) == 0))
            { 
               SetDivStep(drv_1, FULLSTEP);
               SetDivStep(drv_2, FULLSTEP);
               UartPuts("OK\n\r");
               // UartPuts("SETTING DIVISION STEP -> FULLSTEP\n\r");
               return EOK;
            }
            else if((strcmp("HSTEP\n", ParsePtr2) == 0))
            { 
               SetDivStep(drv_1, HALFSTEP);
               SetDivStep(drv_2, HALFSTEP);
               UartPuts("OK\n\r");
               // UartPuts("SETTING DIVISION STEP -> HALFSTEP\n\r");
               return EOK;
            }
            // BOTH DRIVERS COMMANDS -----------------------------END-----------------> 

            else 
            { 
                return E05;
            }   
        }
    } 

}
