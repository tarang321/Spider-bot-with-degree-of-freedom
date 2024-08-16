//#define DEBUG
#define DEBUGSENSORS

#ifdef __AVR__
#if defined(UBRR1H)
#define SSCSerial         Serial1
#define XBeeSerial        Serial3 //Serial2
#else
#define XBeeSerial        Serial
#define DontAllowDebug
#endif
#else  // For My Pic32 Mega shield...
#define SSCSerial         Serial1
#define XBeeSerial        Serial3
#endif

//==================================================================================================================================
// Define which input classes we will use. If we wish to use more than one we need to define USEMULTI - This will define a forwarder
//    type implementation, that the Inputcontroller will need to call.  There will be some negotion for which one is in contol.
//
//  If this is not defined, The included Controller should simply implement the InputController Class...
//==================================================================================================================================
#define QUADMODE            // We are building for quad support...
#define c4DOF   
#define AVERAGER

//#define USEMULTI
//#define USEXBEE            // only allow to be defined on Megas...
#define USEPS2
//#define USECOMMANDER
//#define USESERIAL

// Do we want Debug Serial Output?
//#define DBGSerial Serial

// Some configurations will not allow this so if one of them undefine it
#if (defined USEXBEE) || (defined USECOMMANDER)
#ifdef DontAllowDebug
#undef DBGSerial
#endif
#endif

#ifdef USESERIAL
#undef DBGSerial
#endif

#ifdef DBGSerial
#define OPT_TERMINAL_MONITOR  // Only allow this to be defined if we have a debug serial port
#endif

#ifdef OPT_TERMINAL_MONITOR
//#define OPT_SSC_FORWARDER  // only useful if terminal monitor is enabled
#define OPT_FIND_SERVO_OFFSETS    // Only useful if terminal monitor is enabled
#endif

//#define OPT_GPPLAYER

#define USE_SSC32
#define	cSSC_BINARYMODE	1			// Define if your SSC-32 card supports binary mode.
#define cSSC_BAUD       115200   //SSC32 BAUD rate

// Debug options
//#define DEBUG_IOPINS    // used to control if we are going to use IO pins for debug support

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
// CHR-3
//==================================================================================================================================

//[Assuming Botboarduino for this one
#define SOUND_PIN    5        // Botboarduino JR pin number

// PS2 definitions
#define PS2_DAT      6        
#define PS2_CMD      7
#define PS2_SEL      8
#define PS2_CLK      9

#define cSSC_OUT     4      	//Output pin for (SSC32 RX) on BotBoard (Yellow)
#define cSSC_IN      5      	//Input pin for (SSC32 TX) on BotBoard (Blue)
// XBee was defined to use a hardware Serial port
#define XBEE_BAUD      38400
#define SERIAL_BAUD    38400

// Define Analog pin and minimum voltage that we will allow the servos to run
#define cVoltagePin  0      // Use our Analog pin jumper here...
#define cTurnOffVol  670     // 4.7v
#define cTurnOnVol   700     // 5.5V - optional part to say if voltage goes back up, turn it back on...

//====================================================================
// Warning I reversed my legs as I put left legs where right should be...
//[SSC32 Pin Numbers]
#define cRRCoxaPin      3   //Rear Right leg Hip Horizontal
#define cRRFemurPin     2   //Rear Right leg Hip Vertical
#define cRRTibiaPin     1   //Rear Right leg Knee
#define cRRTarsPin      0   // Tar

#define cRFCoxaPin      12   //Front Right leg Hip Horizontal
#define cRFFemurPin     13   //Front Right leg Hip Vertical
#define cRFTibiaPin     14   //Front Right leg Knee
#define cRFTarsPin      15   // Tar

#define cLRCoxaPin      19    //Rear Left leg Hip Horizontal
#define cLRFemurPin     18    //Rear Left leg Hip Vertical
#define cLRTibiaPin     17    //Rear Left leg Knee
#define cLRTarsPin      16    // Tar

#define cLFCoxaPin      28   //Front Left leg Hip Horizontal
#define cLFFemurPin     29   //Front Left leg Hip Vertical
#define cLFTibiaPin     30   //Front Left leg Knee
#define cLFTarsPin      31   // Tar

//--------------------------------------------------------------------
//[SERVO PULSE INVERSE]
#define cRRCoxaInv      0
#define cRRFemurInv     0
#define cRRTibiaInv     0
#define cRRTarsInv      0

#define cRFCoxaInv      1
#define cRFFemurInv     1
#define cRFTibiaInv     1
#define cRFTarsInv      1

#define cLRCoxaInv      1
#define cLRFemurInv     1
#define cLRTibiaInv     1
#define cLRTarsInv      1

#define cLFCoxaInv      0
#define cLFFemurInv     0
#define cLFTibiaInv     0
#define cLFTarsInv      0

//--------------------------------------------------------------------
//[MIN/MAX ANGLES]
#define cRRCoxaMin1     -650      //Mechanical limits of the Right Rear Leg
#define cRRCoxaMax1     650
#define cRRFemurMin1    -1050
#define cRRFemurMax1    750
#define cRRTibiaMin1    -530
#define cRRTibiaMax1    900
#define cRRTarsMin1     -1300	//4DOF ONLY - In theory the kinematics can reach about -160 deg
#define cRRTarsMax1	500	//4DOF ONLY - The kinematics will never exceed 23 deg though..

#define cRFCoxaMin1     -650      //Mechanical limits of the Right Front Leg
#define cRFCoxaMax1     650
#define cRFFemurMin1    -1050
#define cRFFemurMax1    750
#define cRFTibiaMin1    -530
#define cRFTibiaMax1    900
#define cRFTarsMin1     -1300	//4DOF ONLY - In theory the kinematics can reach about -160 deg
#define cRFTarsMax1	500	//4DOF ONLY - The kinematics will never exceed 23 deg though..

#define cLRCoxaMin1     -650      //Mechanical limits of the Left Rear Leg
#define cLRCoxaMax1     650
#define cLRFemurMin1    -1050
#define cLRFemurMax1    750
#define cLRTibiaMin1    -530
#define cLRTibiaMax1    900
#define cLRTarsMin1     -1300	//4DOF ONLY - In theory the kinematics can reach about -160 deg
#define cLRTarsMax1	500	//4DOF ONLY - The kinematics will never exceed 23 deg though..

#define cLFCoxaMin1     -650      //Mechanical limits of the Left Front Leg
#define cLFCoxaMax1     650
#define cLFFemurMin1    -1050
#define cLFFemurMax1    750
#define cLFTibiaMin1    -530
#define cLFTibiaMax1    900
#define cLFTarsMin1     -1300	//4DOF ONLY - In theory the kinematics can reach about -160 deg
#define cLFTarsMax1	500	//4DOF ONLY - The kinematics will never exceed 23 deg though..



#define cRMCoxaMin1	-530	//Mechanical limits of the Right Middle Leg, decimals = 1
#define cRMCoxaMax1	530
#define cRMFemurMin1	-1010
#define cRMFemurMax1	950
#define cRMTibiaMin1	-1060
#define cRMTibiaMax1	770
#define cRMTarsMin1     -1300	//4DOF ONLY - In theory the kinematics can reach about -160 deg
#define cRMTarsMax1	500	//4DOF ONLY - The kinematics will never exceed 23 deg though..

#define cLMCoxaMin1	-530	//Mechanical limits of the Left Middle Leg, decimals = 1
#define cLMCoxaMax1	530
#define cLMFemurMin1	-950
#define cLMFemurMax1	1010
#define cLMTibiaMin1	-770
#define cLMTibiaMax1	1060
#define cLMTarsMin1     -1300	//4DOF ONLY - In theory the kinematics can reach about -160 deg
#define cLMTarsMax1	500	//4DOF ONLY - The kinematics will never exceed 23 deg though..
//--------------------------------------------------------------------
//[Joint offsets]
// This allows us to calibrate servos to some fixed position, and then adjust them by moving theim
// one or more servo horn clicks.  This requires us to adjust the value for those servos by 15 degrees
// per click.  This is used with the T-Hex 4DOF legs
//First calibrate the servos in the 0 deg position using the SSC-32 reg offsets, then:
#define cFemurHornOffset1	150
#define cTibiaHornOffset1	-50
#define cTarsHornOffset1	50	

//--------------------------------------------------------------------
//[LEG DIMENSIONS]
//Universal dimensions for each leg in mm
#define cXXCoxaLength     29    // This is for CH3-R with Type 3 legs
#define cXXFemurLength    75
#define cXXTibiaLength    70
#define cXXTarsLength     105   // 4DOF only...

#define cRRCoxaLength     cXXCoxaLength	    //Right Rear leg
#define cRRFemurLength    cXXFemurLength
#define cRRTibiaLength    cXXTibiaLength
#define cRRTarsLength	  cXXTarsLength	    //4DOF ONLY

#define cRFCoxaLength     cXXCoxaLength	    //Rigth front leg
#define cRFFemurLength    cXXFemurLength
#define cRFTibiaLength    cXXTibiaLength
#define cRFTarsLength	  cXXTarsLength    //4DOF ONLY

#define cLRCoxaLength     cXXCoxaLength	    //Left Rear leg
#define cLRFemurLength    cXXFemurLength
#define cLRTibiaLength    cXXTibiaLength
#define cLRTarsLength	  cXXTarsLength    //4DOF ONLY

#define cLFCoxaLength     cXXCoxaLength	    //Left front leg
#define cLFFemurLength    cXXFemurLength
#define cLFTibiaLength    cXXTibiaLength
#define cLFTarsLength	  cXXTarsLength	    //4DOF ONLY


//--------------------------------------------------------------------
//[BODY DIMENSIONS]
#define cRRCoxaAngle1   -450   //Default Coxa setup angle, decimals = 1
#define cRFCoxaAngle1    450   //Default Coxa setup angle, decimals = 1
#define cLRCoxaAngle1   -450   //Default Coxa setup angle, decimals = 1
#define cLFCoxaAngle1    450   //Default Coxa setup angle, decimals = 1

#define cRROffsetX      -54    //Distance X from center of the body to the Right Rear coxa
#define cRROffsetZ       54    //Distance Z from center of the body to the Right Rear coxa
#define cRFOffsetX      -54    //Distance X from center of the body to the Right Front coxa
#define cRFOffsetZ      -54    //Distance Z from center of the body to the Right Front coxa

#define cLROffsetX       54    //Distance X from center of the body to the Left Rear coxa
#define cLROffsetZ       54    //Distance Z from center of the body to the Left Rear coxa
#define cLFOffsetX       54     //Distance X from center of the body to the Left Front coxa
#define cLFOffsetZ      -54    //Distance Z from center of the body to the Left Front coxa

//--------------------------------------------------------------------
//[START POSITIONS FEET]
#define cHexInitXZ	 102 
#define CHexInitXZ45     72        // Sin and cos(45) .7071
#define CHexInitY	 35        //30

#define cRRInitPosX     CHexInitXZ45      //Start positions of the Right Rear leg
#define cRRInitPosY     CHexInitY
#define cRRInitPosZ     CHexInitXZ45

#define cRFInitPosX     CHexInitXZ45      //Start positions of the Right Front leg
#define cRFInitPosY     CHexInitY
#define cRFInitPosZ     -CHexInitXZ45

#define cLRInitPosX     CHexInitXZ45      //Start positions of the Left Rear leg
#define cLRInitPosY     CHexInitY
#define cLRInitPosZ     CHexInitXZ45

#define cLFInitPosX     CHexInitXZ45      //Start positions of the Left Front leg
#define cLFInitPosY     CHexInitY
#define cLFInitPosZ     -CHexInitXZ45



//--------------------------------------------------------------------
//[Tars factors used in formula to calc Tarsus angle relative to the ground]
#define cTarsConst	720	//4DOF ONLY
#define cTarsMulti	2	//4DOF ONLY
#define cTarsFactorA	70	//4DOF ONLY
#define cTarsFactorB	60	//4DOF ONLY
#define cTarsFactorC	50	//4DOF ONLY
