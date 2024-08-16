#if ARDUINO>99
#include <Arduino.h> // Arduino 1.0
#else
#include <Wprogram.h> // Arduino 0022
#endif
#include <PS2X_lib.h>

//[CONSTANTS]
#define WALKMODE          0
#define TRANSLATEMODE     1
#define ROTATEMODE        2
#define SINGLELEGMODE     3
#define GPPLAYERMODE      4


#define cTravelDeadZone  1      //The deadzone for the analog input from the remote
#define  MAXPS2ERRORCNT  5     // How many times through the loop will we go before shutting off robot?

#ifndef MAX_BODY_Y
#define MAX_BODY_Y 200
#endif

//=============================================================================
// Global - Local to this file only...
//=============================================================================
PS2X ps2x; // create PS2 Controller Class


// Define an instance of the Input Controller...
InputController  g_InputController;       // Our Input controller 


static short       g_BodyYOffset; 
static short       g_sPS2ErrorCnt;
static short       g_BodyYShift;
static byte        ControlMode;
static bool        DoubleHeightOn;
static bool        DoubleTravelOn;
static bool        WalkMethod;
byte               GPSeq;             //Number of the sequence
short              g_sGPSMController;    // What GPSM value have we calculated. 0xff - Not used yet

static byte        FootSensorMode;
static byte        AcceleratorMode;
static byte        IRSensorMode;


//Averager
#ifdef AVERAGER
	int ValuesCounter;
	const int valuesNumber = 10;
	int totalTravelLengthx;
	int totalTravelLengthy;
	int totalTravelLengthz;
	int valuesTravelLengthx[valuesNumber];
	int valuesTravelLengthy[valuesNumber];
	int valuesTravelLengthz[valuesNumber];
#endif

// some external or forward function references.
extern void PS2TurnRobotOff(void);

//==============================================================================
// This is The function that is called by the Main program to initialize
//the input controller, which in this case is the PS2 controller
//process any commands.
//==============================================================================

// If both PS2 and XBee are defined then we will become secondary to the xbee
void InputController::Init(void)
{
  int error;

  //error = ps2x.config_gamepad(57, 55, 56, 54);  // Setup gamepad (clock, command, attention, data) pins
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT);  // Setup gamepad (clock, command, attention, data) pins

#ifdef DBGSerial
	DBGSerial.print("PS2 Init: ");
	DBGSerial.println(error, DEC);
#endif
  g_BodyYOffset = 0;    
  g_BodyYShift = 0;
  g_sPS2ErrorCnt = 0;  // error count

  ControlMode = WALKMODE;
  DoubleHeightOn = false;
  DoubleTravelOn = false;
  WalkMethod = false;

  g_InControlState.SpeedControl = 100;    // Sort of migrate stuff in from Devon.
}

//==============================================================================
// This function is called by the main code to tell us when it is about to
// do a lot of bit-bang outputs and it would like us to minimize any interrupts
// that we do while it is active...
//==============================================================================
void InputController::AllowControllerInterrupts(boolean fAllow)
{
  // We don't need to do anything...
}

//==============================================================================
// This is The main code to input function to read inputs from the PS2 and then
//process any commands.
//==============================================================================
void InputController::ControlInput(void)
{
  boolean fAdjustLegPositions = false;
  // Then try to receive a packet of information from the PS2.
  // Then try to receive a packet of information from the PS2.
  ps2x.read_gamepad();          //read controller and set large motor to spin at 'vibrate' speed

    // Wish the library had a valid way to verify that the read_gamepad succeeded... Will hack for now
  if ((ps2x.Analog(1) & 0xf0) == 0x70) {
#ifdef DBGSerial
#ifdef DEBUG_PS2_INPUT
	if (g_fDebugOutput) {
		DBGSerial.print("PS2 Input: ");
		DBGSerial.print(ps2x.ButtonDataByte(), HEX);
		DBGSerial.print(":");
		DBGSerial.print(ps2x.Analog(PSS_LX), DEC);
		DBGSerial.print(" ");
		DBGSerial.print(ps2x.Analog(PSS_LY), DEC);
		DBGSerial.print(" ");
		DBGSerial.print(ps2x.Analog(PSS_RX), DEC);
		DBGSerial.print(" ");
		DBGSerial.println(ps2x.Analog(PSS_RY), DEC);
	}
#endif
#endif
    // In an analog mode so should be OK...
    g_sPS2ErrorCnt = 0;    // clear out error count...

    if (ps2x.ButtonPressed(PSB_START)) {// OK lets try "0" button for Start. 
      if (g_InControlState.fRobotOn) {
        PS2TurnRobotOff();
      } 
      else {
        //Turn on
        g_InControlState.fRobotOn = 1;
        fAdjustLegPositions = true;
      }
    }

    if (g_InControlState.fRobotOn) {
      // [SWITCH MODES]

      //Translate mode
      if (ps2x.ButtonPressed(PSB_L1)) {// L1 Button Test
        MSound( 1, 50, 2000);  
        if (ControlMode != TRANSLATEMODE )
          ControlMode = TRANSLATEMODE;
        else {
          if (g_InControlState.SelectedLeg==255) 
            ControlMode = WALKMODE;
          else
            ControlMode = SINGLELEGMODE;
        }
      }

      //Rotate mode
      if (ps2x.ButtonPressed(PSB_L2)) {    // L2 Button Test
        MSound( 1, 50, 2000);
        if (ControlMode != ROTATEMODE)
          ControlMode = ROTATEMODE;
        else {
          if (g_InControlState.SelectedLeg == 255) 
            ControlMode = WALKMODE;
          else
            ControlMode = SINGLELEGMODE;
        }
      }

      //Switch IRsensor on/of
      if (ps2x.ButtonPressed(PSB_CIRCLE)) {// O - Circle Button Test
        IRSensorMode = !IRSensorMode;
        if (IRSensorMode) {
          MSound(1, 250, 1500); 
        } 
        else {
          MSound( 2, 100, 2000, 50, 4000);
		  g_InControlState.BodyPos.z=0;
		  g_InControlState.BodyRot1.y=0;
        }
      }      

      //Switch accelerometer on/of
      if (ps2x.ButtonPressed(PSB_CROSS)) { // X - Cross Button Test
        AcceleratorMode = !AcceleratorMode;
        if (AcceleratorMode) {
          MSound(1, 250, 1500); 
        } 
        else {
          MSound( 2, 100, 2000, 50, 4000);
	      g_InControlState.BodyRot1.x = 0;
		  g_InControlState.BodyRot1.y = 0;
		  g_InControlState.BodyRot1.z = 0;
        }
      }


      //Switch footsensors on/of
      if (ps2x.ButtonPressed(PSB_SQUARE)) { // Square Button Test
        FootSensorMode = !FootSensorMode;
        if (FootSensorMode) {
          MSound(1, 250, 1500); 
        } 
        else {
          MSound( 2, 100, 2000, 50, 4000);
		  g_BodyYShift = 0;
        }
      }

      //Stand up, sit down  
      if (ps2x.ButtonPressed(PSB_TRIANGLE)) { // Triangle - Button Test
        if (g_BodyYOffset>0) 
          g_BodyYOffset = 0;
        else
          g_BodyYOffset = 50;
        fAdjustLegPositions = true;
      }

      if (ps2x.ButtonPressed(PSB_PAD_UP)) {// D-Up - Button Test
        g_BodyYOffset += 10;

        // And see if the legs should adjust...
        fAdjustLegPositions = true;
        if (g_BodyYOffset > MAX_BODY_Y)
          g_BodyYOffset = MAX_BODY_Y;
      }

      if (ps2x.ButtonPressed(PSB_PAD_DOWN) && g_BodyYOffset) {// D-Down - Button Test
        if (g_BodyYOffset > 10)
          g_BodyYOffset -= 10;
        else
          g_BodyYOffset = 0;      // constrain don't go less than zero.

        // And see if the legs should adjust...
        fAdjustLegPositions = true;
      }

      if (ps2x.ButtonPressed(PSB_PAD_RIGHT)) { // D-Right - Button Test
        if (g_InControlState.SpeedControl>0) {
          g_InControlState.SpeedControl = g_InControlState.SpeedControl - 50;
          MSound( 1, 50, 2000);  
        }
      }

      if (ps2x.ButtonPressed(PSB_PAD_LEFT)) { // D-Left - Button Test
        if (g_InControlState.SpeedControl<2000 ) {
          g_InControlState.SpeedControl = g_InControlState.SpeedControl + 50;
          MSound( 1, 50, 2000); 
        }
      }

      //[Walk functions]
      if (ControlMode == WALKMODE) {
        //Switch gates
        if (ps2x.ButtonPressed(PSB_SELECT)            // Select Button Test
        && abs(g_InControlState.TravelLength.x)<cTravelDeadZone //No movement
        && abs(g_InControlState.TravelLength.z)<cTravelDeadZone 
          && abs(g_InControlState.TravelLength.y*2)<cTravelDeadZone  ) {
          g_InControlState.GaitType = g_InControlState.GaitType+1;                    // Go to the next gait...
          if (g_InControlState.GaitType<NUM_GAITS) {                 // Make sure we did not exceed number of gaits...
            MSound( 1, 50, 2000); 
          } 
          else {
            MSound(2, 50, 2000, 50, 2250); 
            g_InControlState.GaitType = 0;
          }
          GaitSelect();
        }

        //Double leg lift height
        if (ps2x.ButtonPressed(PSB_R1)) { // R1 Button Test
          MSound( 1, 50, 2000); 
          DoubleHeightOn = !DoubleHeightOn;
          if (DoubleHeightOn)
            g_InControlState.LegLiftHeight = 120;
          else
            g_InControlState.LegLiftHeight = 60;
        }

        //Double Travel Length
        if (ps2x.ButtonPressed(PSB_R2)) {// R2 Button Test
          MSound(1, 50, 2000); 
          DoubleTravelOn = !DoubleTravelOn;
        }

        // Switch between Walk method 1 && Walk method 2
        if (ps2x.ButtonPressed(PSB_R3)) { // R3 Button Test
          MSound(1, 50, 2000); 
          WalkMethod = !WalkMethod;
        }

        //Walking
        if (WalkMethod)  //(Walk Methode) 
          g_InControlState.TravelLength.z = (ps2x.Analog(PSS_RY)-128)/1.2; //Right Stick Up/Down  

        else {
          g_InControlState.TravelLength.x = -(ps2x.Analog(PSS_LX) - 128)/1.2;
          g_InControlState.TravelLength.z = (ps2x.Analog(PSS_LY) - 128)/1.2;
        }

        if (!DoubleTravelOn) {  //(Double travel length)
          g_InControlState.TravelLength.x = g_InControlState.TravelLength.x/1.5;
          g_InControlState.TravelLength.z = g_InControlState.TravelLength.z/1.5;
        }

		g_InControlState.TravelLength.y = -(ps2x.Analog(PSS_RX) - 128)/4; //Right Stick Left/Right 

#ifdef AVERAGER
			totalTravelLengthx = totalTravelLengthx - valuesTravelLengthx[ValuesCounter]; 
			totalTravelLengthy = totalTravelLengthy - valuesTravelLengthy[ValuesCounter]; 
			totalTravelLengthz = totalTravelLengthz - valuesTravelLengthz[ValuesCounter]; 

			valuesTravelLengthx[ValuesCounter] = g_InControlState.TravelLength.x;
			valuesTravelLengthy[ValuesCounter] = g_InControlState.TravelLength.y;
			valuesTravelLengthz[ValuesCounter] = g_InControlState.TravelLength.z;

			totalTravelLengthx = totalTravelLengthx + valuesTravelLengthx[ValuesCounter];
			totalTravelLengthy = totalTravelLengthy + valuesTravelLengthy[ValuesCounter];
			totalTravelLengthz = totalTravelLengthz + valuesTravelLengthz[ValuesCounter];

			ValuesCounter = ValuesCounter + 1; 

			if (ValuesCounter >= valuesNumber)
				ValuesCounter = 0;

			g_InControlState.TravelLength.x = (totalTravelLengthx / valuesNumber);
			g_InControlState.TravelLength.y = (totalTravelLengthy / valuesNumber);
			g_InControlState.TravelLength.z = (totalTravelLengthz / valuesNumber);
#endif

      }

	  if ((ControlMode == TRANSLATEMODE) || (ControlMode == ROTATEMODE)) {
		  g_InControlState.BodyRot1.x = 0;
		  g_InControlState.BodyRot1.z = 0;
	  }

      //[Translate functions]
      if (ControlMode == TRANSLATEMODE) {
        g_InControlState.BodyPos.x = (ps2x.Analog(PSS_LX) - 128)/2;
        g_InControlState.BodyPos.z = -(ps2x.Analog(PSS_LY) - 128)/2;
        g_InControlState.BodyRot1.y = (ps2x.Analog(PSS_RX) - 128)*2;
        if (-(ps2x.Analog(PSS_RY) - 128) < 0)
			g_BodyYShift = (-(ps2x.Analog(PSS_RY) - 128)/3.5);
		else
			g_BodyYShift = -(ps2x.Analog(PSS_RY) - 128);
      }

      //[Rotate functions]
      if (ControlMode == ROTATEMODE) {
        g_InControlState.BodyRot1.x = (ps2x.Analog(PSS_LY) - 128)*2;
        g_InControlState.BodyRot1.y = (ps2x.Analog(PSS_RX) - 128)*2;
        g_InControlState.BodyRot1.z = (ps2x.Analog(PSS_LX) - 128)*2;
        if (-(ps2x.Analog(PSS_RY) - 128) < 0)
			g_BodyYShift = (-(ps2x.Analog(PSS_RY) - 128)/3.5);
		else
			g_BodyYShift = -(ps2x.Analog(PSS_RY) - 128);
      }


      //Calculate walking time delay
      g_InControlState.InputTimeDelay = 128 - max(max(abs(ps2x.Analog(PSS_LX) - 128), abs(ps2x.Analog(PSS_LY) - 128)), abs(ps2x.Analog(PSS_RX) - 128));
    }

    //Calculate g_InControlState.BodyPos.y
    g_InControlState.BodyPos.y = min(max(g_BodyYOffset + g_BodyYShift,  0), MAX_BODY_Y);
    if (fAdjustLegPositions)
      AdjustLegPositionsToBodyHeight();    // Put main workings into main program file
  } 
  else {
    // We may have lost the PS2... See what we can do to recover...
    if (g_sPS2ErrorCnt < MAXPS2ERRORCNT)
      g_sPS2ErrorCnt++;    // Increment the error count and if to many errors, turn off the robot.
    else if (g_InControlState.fRobotOn)
      PS2TurnRobotOff();
    ps2x.reconfig_gamepad();
  }
}

//==============================================================================
// PS2TurnRobotOff - code used couple of places so save a little room...
//==============================================================================
void PS2TurnRobotOff(void)
{
  //Turn off
  g_InControlState.BodyPos.x = 0;
  g_InControlState.BodyPos.y = 0;
  g_InControlState.BodyPos.z = 0;
  g_InControlState.BodyRot1.x = 0;
  g_InControlState.BodyRot1.y = 0;
  g_InControlState.BodyRot1.z = 0;
  g_InControlState.TravelLength.x = 0;
  g_InControlState.TravelLength.z = 0;
  g_InControlState.TravelLength.y = 0;
  g_BodyYOffset = 0;
  g_BodyYShift = 0;
  g_InControlState.SelectedLeg = 255;
  g_InControlState.fRobotOn = 0;
  AdjustLegPositionsToBodyHeight();    // Put main workings into main program file
}




