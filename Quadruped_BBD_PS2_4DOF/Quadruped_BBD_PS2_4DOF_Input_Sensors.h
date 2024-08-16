
bool LegUp;

int SensorReactSpeed = 10;
int SensorSensitivity = 2;

int	SensorDeviationX;
int	SensorDeviationY;
int	SensorDeviationZ;
int SensorShiftY;

int XaxisValue = 0;
int ZaxisValue = 0;

const int numReadings = 150;

int Counter = 0;                  // the Counter of the current reading

int readingsX[numReadings];      // the readings from the analog input
int totalX = 0;                  // the running total

int readingsZ[numReadings];      // the readings from the analog input
int totalZ = 0;                  // the running total

int IRsensorValue1;
int IRsensorValue2;

long IRcm=80;
long IRcm1=80;
long IRcm2=80;

int RFcoxa;
int LFcoxa;

void ScanFootSensor(void) {	

	int LegRF = digitalRead(3);
    int LegLR = digitalRead(10);
    int LegLF = digitalRead(11);
    int LegRR = digitalRead(12);

	if (FootSensorMode)
		if (LegLF + LegRF + LegLR + LegRR > 2) {
			SensorShiftY = SensorShiftY + SensorReactSpeed;
		}
		else {
			if (SensorShiftY > 0)
				SensorShiftY = SensorShiftY - SensorReactSpeed;
		}
	else
		SensorShiftY = 0;

#ifdef DEBUGSENSORS
	Serial.begin(38400);
	Serial.print("LegLF:");
	Serial.print(LegLF, DEC);
	Serial.print(" LegRF:");
	Serial.print(LegRF, DEC);
	Serial.print(" LegLR:");
	Serial.print(LegLR, DEC);
	Serial.print(" LegRR:");
	Serial.print(LegRR, DEC);
	Serial.print(" SensorShiftY:");
	Serial.print(SensorShiftY, DEC);
	Serial.print(" FootSensorMode:");
	Serial.print(FootSensorMode, DEC);
#endif

	  //experimental!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //[Footsensor functions]
      if (FootSensorMode) {
		if ((ControlMode == TRANSLATEMODE) || (ControlMode == ROTATEMODE)) {
			if (g_BodyYOffset > 0) {
			g_BodyYShift = g_BodyYShift + SensorShiftY;
			}
		}
		else {
			if (g_BodyYOffset > 0) {
			g_BodyYShift = SensorShiftY;
			}
		}
      g_InControlState.BodyPos.y = min(max(g_BodyYOffset + g_BodyYShift,  0), MAX_BODY_Y);
	  }

	  //experimental!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void Accelerometer(void) {

	totalX = totalX - readingsX[Counter]; 
	totalZ = totalZ - readingsZ[Counter]; 

	readingsX[Counter] = analogRead(A2) - 415;
	readingsZ[Counter] = analogRead(A1) - 520;

	totalX= totalX + readingsX[Counter];
	totalZ= totalZ + readingsZ[Counter];

	Counter = Counter + 1; 

	if (Counter >= numReadings)
		Counter = 0;

	XaxisValue = (totalX / numReadings);
	ZaxisValue = (totalZ / numReadings);

	if (abs(XaxisValue) > SensorSensitivity) { 
		SensorDeviationX = XaxisValue * SensorReactSpeed;
	}

	if (abs(ZaxisValue) > SensorSensitivity) { 
		SensorDeviationZ = ZaxisValue * SensorReactSpeed;
	}

	if (abs(XaxisValue) <= SensorSensitivity)
		SensorDeviationX = 0;

	if (abs(ZaxisValue) <= SensorSensitivity)
		SensorDeviationZ = 0;

      //[Accelerator functions]
	  //experimental!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if (AcceleratorMode) {
		if ((ControlMode == TRANSLATEMODE) || (ControlMode == ROTATEMODE)) {
			if (g_BodyYOffset > 0) {
			g_InControlState.BodyRot1.x = g_InControlState.BodyRot1.x - SensorDeviationX; 
			g_InControlState.BodyRot1.z = g_InControlState.BodyRot1.z + SensorDeviationZ; 
			}
		}
		else {
			if (g_BodyYOffset > 0) {
			g_InControlState.BodyRot1.x = -SensorDeviationX; 
			g_InControlState.BodyRot1.z = SensorDeviationZ; 
			}
		}
      }
	  //experimental!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifdef DEBUGSENSORS
	Serial.print(" XaxisValue:");
	Serial.print(XaxisValue, DEC);
	Serial.print(" ZaxisValue:");
	Serial.print(ZaxisValue, DEC);
	Serial.print(" AcceleratorMode:");
	Serial.print(AcceleratorMode, DEC);
#endif
}

void IRSensor(void) {

  IRsensorValue1 = analogRead(A3);
  delay(1);
  IRsensorValue2 = analogRead(A4);

  if  (RFcoxa >= -200)
	IRcm1 = 4800/(analogRead(A3)-20);

  if  (LFcoxa >= -300)
	IRcm2 = 4800/(analogRead(A4)-20);
 
  if ((IRcm1 > 80) || (IRcm1 < 0)) 
	  IRcm1 = 80;

  if ((IRcm2 > 80) || (IRcm2 < 0)) 
	  IRcm2 = 80;

  IRcm = (IRcm1 + IRcm2)/3.2;
	  //experimental!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //[IRsensor functions]
      if (IRSensorMode == 1) {
		if ((ControlMode == TRANSLATEMODE) || (ControlMode == ROTATEMODE)) {
			if (g_BodyYOffset > 0) {
				g_InControlState.BodyPos.z = g_InControlState.BodyPos.z-(50-IRcm);
				g_InControlState.BodyRot1.y = g_InControlState.BodyRot1.y + (2*(IRcm1 - IRcm2));

			}
		}
      }
	  //experimental!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifdef DEBUGSENSORS
	Serial.print(" IRcm1:");
	Serial.print(IRcm1, DEC);
	Serial.print(" IRcm2:");
	Serial.print(IRcm2, DEC);
	Serial.print(" LFcoxa:");
	Serial.print(LFcoxa, DEC);
	Serial.print(" RFcoxa:");
	Serial.print(RFcoxa, DEC);
	Serial.print(" IRSensorMode:");
	Serial.println(IRSensorMode, DEC);
#endif
}