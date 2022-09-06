
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#include "uart.h"
#include "circ_buffer.h"  //TODO: wywalic !!!!!!!!!
#include "parse.h"

char message[32];

PARSE_ERRORS ParseData(CIRC_BUFF *circ_buf) 
{ 
    char rcv_data;
    char tab[32] = {0};
    char value[32] = {' '};
    uint inx = 0; 
    int size = 0;
    float val =0;
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
            // simplest sort -> sort by white list
            char *ParsePtr2 = strtok(NULL, "=");
            // sprintf(message, "%s\n\r", ParsePtr2);
            // UartPuts(message);

            if((strcmp("POS", ParsePtr2) == 0) || (strcmp("VEL", ParsePtr2) == 0 ))
            { 
                char *ParsePtr3 = strtok(NULL, ",");

                if(strcmp("1", ParsePtr3) == 0)
                { 
                    char *ParsePtr4 = strtok(NULL, "\n");
                    val = atof(ParsePtr4);
                    
                    if(strcmp("POS", ParsePtr2) == 0)
                    {
                        // set pose TODO:
                        UartPuts("POSE_1 IS SETTING !\n\r");
                    }
                    else if(strcmp("VEL", ParsePtr2) == 0)                   
                    {
                        // set velocity TODO:
                        UartPuts("VELOCITY_1 IS SETTING !\n\r");
                    }

                    return EOK;

                }
                else if(strcmp("2", ParsePtr3) == 0)
                { 
                    char *ParsePtr4 = strtok(NULL, "\n");
                    val = atof(ParsePtr4);

                    if(strcmp("POS", ParsePtr2) == 0)
                    {
                        // set pose TODO:
                        UartPuts("POSE_2 IS SETTING !\n\r");
                    }
                    else if(strcmp("VEL", ParsePtr2) == 0)                   
                    {
                        // set velocity TODO:
                        UartPuts("VELOCITY_2 IS SETTING !\n\r");
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
                UartPuts("CONNECTION OK!\n\r");
                return EOK;
            }
            else if((strcmp("ENABLE\n", ParsePtr2) == 0))
            { 
                // turn on drivers 
                UartPuts("DRIVERS ENABLE!\n\r");
                return EOK;
            }
            else if((strcmp("DISABLE\n", ParsePtr2) == 0))
            { 
                // turn off drivers 
                UartPuts("DRIVERS DISABLE!\n\r");
                return EOK;
            }
            else if((strcmp("RUN\n", ParsePtr2) == 0))
            { 
                UartPuts("SETTING STATE -> RUN\n\r");
                return EOK;
            }
            else if((strcmp("STOP\n", ParsePtr2) == 0))
            { 
                UartPuts("SETTING STATE -> STOP\n\r");
                return EOK;
            }
            else if((strcmp("SAFETY_STOP\n", ParsePtr2) == 0))
            { 
               UartPuts("SETTING STATE -> SAFETY_STOP\n\r");
               UartPuts("TURN OFF DRIVES -> GO RESET -> AND REBOOT DEVICE !!!\n\r");
               return EOK;
            }
            else if((strcmp("FSTOP\n", ParsePtr2) == 0))
            { 
               UartPuts("SETTING DIVISION STEP -> FULLSTEP\n\r");
               return EOK;
            }
            else if((strcmp("HSTOP\n", ParsePtr2) == 0))
            { 
               UartPuts("SETTING DIVISION STEP -> HALFSTEP\n\r");
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
