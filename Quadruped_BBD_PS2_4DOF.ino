
#define DEFINE_HEX_GLOBALS
#if ARDUINO>99
#include <Arduino.h>
#else
#endif
#include <Wire.h>
#include <EEPROM.h>
#include <PS2X_lib.h>

#include <SoftwareSerial.h>
#include "Hex_CFG.h"
#include <Quadruped_BBD_PS2_4DOF.h>

#include <Quadruped_BBD_PS2_4DOF_Input_PS2.h>
#include <Quadruped_BBD_PS2_4DOF_Input_Sensors.h>

#include <Quadruped_BBD_PS2_4DOF_Driver_SSC32.h>
#include <Quadruped_BBD_PS2_4DOF_Code_Quad.h>
