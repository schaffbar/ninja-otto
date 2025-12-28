
// LED constants
#define LEDS_COUNT  3
#define LEDS_PIN	  2
#define CHANNEL		  0

#define ServoLeftFootPin    0 
#define ServoLeftLegPin     1 
#define ServoRightFootPin  20
#define ServoRightLegPin   21

#define UltrasonicPin  5

//CALIBRATION SETTINGS:

////////// 2:

// Left foot forward walking rotation Speed
int LFFWRS= 20;    // 0 = Slowest   90 = Fastest  Default = 12

// Right foot forward walking rotation Speed
int RFFWRS= 15;  //20; // 0 = Slowest   90 = Fastest  Default = 12 // <--

////////// 3:

// Left foot Backward walking rotation Speed
int LFBWRS= 20;   // 0 = Slowest   90 = Fastest  Default = 12

// Right foot Backward walking rotation Speed
int RFBWRS= 15; //20;   // 0 = Slowest   90 = Fastest  Default = 12 // <--

////////// 4:

// Left Leg standing Position
int LA0= 60;    // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 60

// Right Leg standing position
int RA0= 135; // 120;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 120 // <--

////////// 5:

// Left Leg tilt left walking position
int LATL= 100;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 85   Default HUMANOID = 80

// Right Leg tilt left walking position
int RATL= 175;  // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 175   Default HUMANOID = 150

// Left Leg tilt right walking position
int LATR= 5;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 5   Default HUMANOID = 30

// Right Leg tilt right walking position
int RATR= 80;  // 0 = Full Tilt Right   180 = Full Tilt Left   Default BASIC = 95  Default HUMANOID = 100

////////// 6:

// Left Leg roll Position
int LA1= 160; // 180;   // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 170 // <--

// Right Leg roll position
int RA1= 25; // 0;    // 0 = Full Tilt Right   180 = Full Tilt Left   Default = 10  // <--