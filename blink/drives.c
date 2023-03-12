
/* drives.c */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

#include "drives.h" 

const uint phase_sequence [8] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09};
uint actual_step = 0;
char buf [32]; 

// Public: 
void InitStepper(DRIVE*drive_struct, int Pin_1, int Pin_2, int Pin_3, int Pin_4, int vel ){ 
    drive_struct->pose = 0;
    drive_struct->ref_pose = 0;
    drive_struct->velocity = vel; 
    drive_struct->error = 10;
    drive_struct->busy = 0;
    drive_struct->state = STOP;
    drive_struct->step = HALFSTEP; 
    drive_struct->dir = FORWARD;
    drive_struct->threshold_pose = 0.5f; //*(1.0f/2048.0f);
    drive_struct->one_fullstep = 1.0f; // (1.0f/2048.0f);
    drive_struct->one_halfstep = 0.5f; // (1.0f/4096.0f);
    drive_struct->Pin1 = Pin_1; 
    drive_struct->Pin2 = Pin_2;
    drive_struct->Pin3 = Pin_3; 
    drive_struct->Pin4 = Pin_4;
    gpio_init(drive_struct->Pin1); 
    gpio_init(drive_struct->Pin2);
    gpio_init(drive_struct->Pin3); 
    gpio_init(drive_struct->Pin4);
    gpio_set_dir(drive_struct->Pin1, GPIO_OUT);
    gpio_set_dir(drive_struct->Pin2, GPIO_OUT);
    gpio_set_dir(drive_struct->Pin3, GPIO_OUT);
    gpio_set_dir(drive_struct->Pin4, GPIO_OUT);
}

void SetState(DRIVE*drive_struct, DRIVE_STATE state){ 
    drive_struct->state = state;
}

void SetDivStep(DRIVE*drive_struct, DIV_STEP step){ 
    drive_struct->step = step;
}

void SetVel(DRIVE*drive_struct, float velocity){ 
    drive_struct->velocity = velocity; 
}

void SetPose(DRIVE*drive_struct, float ref_pose){ 
    drive_struct->ref_pose = (drive_struct->ref_pose) + (ref_pose); 
    //drive_struct->ref_pose = ref_pose;
}

void SetDir(DRIVE*drive_struct, DRIVE_DIR direction){ 
    drive_struct->dir = direction;
}

DRIVE_DIR GetDir(DRIVE*drive_struct){ 
    return drive_struct->dir;
}

int GetVel(DRIVE*drive_struct){ 
    return drive_struct->velocity; 
}

int GetPose(DRIVE*drive_struct){ 
    return drive_struct->ref_pose;
}

int GetBusy(DRIVE*drive_struct){ 
    return drive_struct->busy;
}

static void SetBusy(DRIVE*drive_struct, int busy){ 
    drive_struct->busy = busy; 
}


// Private:
void DrivesTask(DRIVE*drive_struct){
    
    // jeśli stan to run     
    if(drive_struct->state == RUN)
    {   // i jeśli pozycja poza zakresem zadanym 
        if(! ((((drive_struct->ref_pose)-(drive_struct->threshold_pose)) <= (drive_struct->pose)) && (((drive_struct->ref_pose)+(drive_struct->threshold_pose)) >= (drive_struct->pose))) )
        {
            SetBusy(drive_struct,1); // Now, I'm busy    
            if(GetDir(drive_struct) == FORWARD){
                // to dokonaj ruchu dla osi 
                if((phase_sequence[actual_step] & 0x08) > 0) { gpio_put(drive_struct->Pin1, 1); } else { gpio_put(drive_struct->Pin1, 0); };
                if((phase_sequence[actual_step] & 0x04) > 0) { gpio_put(drive_struct->Pin2, 1); } else { gpio_put(drive_struct->Pin2, 0); };
                if((phase_sequence[actual_step] & 0x02) > 0) { gpio_put(drive_struct->Pin3, 1); } else { gpio_put(drive_struct->Pin3, 0); };
                if((phase_sequence[actual_step] & 0x01) > 0) { gpio_put(drive_struct->Pin4, 1); } else { gpio_put(drive_struct->Pin4, 0); };

                // aktualizuj pozycje  
                if(drive_struct->step == FULLSTEP)
                { 
                    drive_struct->pose = (drive_struct->pose) + (drive_struct->one_fullstep);
                } 
                if(drive_struct->step == HALFSTEP)
                { 
                    drive_struct->pose = (drive_struct->pose) + (drive_struct->one_halfstep);
                }
                
                if(drive_struct->step == FULLSTEP) { actual_step = actual_step+2; } else { actual_step++; }
                if(actual_step > 7) actual_step = 0;  

            }
            if(GetDir(drive_struct) == BACKWARD){
                // to dokonaj ruchu dla osi 
                if((phase_sequence[actual_step] & 0x01) > 0) { gpio_put(drive_struct->Pin1, 1); } else { gpio_put(drive_struct->Pin1, 0); };
                if((phase_sequence[actual_step] & 0x02) > 0) { gpio_put(drive_struct->Pin2, 1); } else { gpio_put(drive_struct->Pin2, 0); };
                if((phase_sequence[actual_step] & 0x04) > 0) { gpio_put(drive_struct->Pin3, 1); } else { gpio_put(drive_struct->Pin3, 0); };
                if((phase_sequence[actual_step] & 0x08) > 0) { gpio_put(drive_struct->Pin4, 1); } else { gpio_put(drive_struct->Pin4, 0); };

                // aktualizuj pozycje  
                if(drive_struct->step == FULLSTEP)
                { 
                    drive_struct->pose = (drive_struct->pose) - (drive_struct->one_fullstep);
                } 
                if(drive_struct->step == HALFSTEP)
                { 
                    drive_struct->pose = (drive_struct->pose) - (drive_struct->one_halfstep);
                }
                
                if(drive_struct->step == FULLSTEP) { actual_step = actual_step+2; } else { actual_step++; }
                if(actual_step > 7) actual_step = 0; 
            
            } 

        }
        else
        { 
            SetBusy(drive_struct,0); // Now, I'm free   
        }
        // Debug commands
        // sprintf(buf, "ident:%d, akt:krok:%f ref:krok:%f \n\r",drive_struct->Pin1, drive_struct->pose, drive_struct->ref_pose);
        // UartPuts(buf);
    }

}


