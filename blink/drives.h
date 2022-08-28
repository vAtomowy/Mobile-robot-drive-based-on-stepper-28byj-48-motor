
/* drives.h */ 

// funkcje public do zadawania pozycji pojedynczego koła 

typedef enum { 
    FULLSTEP = 0,
    HALFSTEP,
} DIV_STEP;

typedef enum { 
  STOP = 0,  
  RUN,
  SAFETY 
} DRIVE_STATE;

typedef struct {
  DIV_STEP step;
  DRIVE_STATE state;
  float velocity;
  float one_fullstep;
  float one_halfstep;
  float error;
  float threshold_pose;
  float ref_pose;
  float pose; 
  int Pin1;
  int Pin2;
  int Pin3;
  int Pin4;
} DRIVE;

//Function prototypes:
void InitStepper(DRIVE*drive_struct, int Pin_1, int Pin_2, int Pin_3, int Pin_4, int vel); 
void SetState(DRIVE*drive_struct, DRIVE_STATE state);
void SetDivStep(DRIVE*drive_struct, DIV_STEP step); 
void SetVel(DRIVE*drive_struct, float velocity); 
void SetPose(DRIVE*drive_struct, float ref_pose);
int GetVel(DRIVE*drive_struct); 
int GetPose(DRIVE*drive_struct); 

void DrivesTask(DRIVE*drive_struct); 

