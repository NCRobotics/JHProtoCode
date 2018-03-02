
/*
 *  JLI Robotics - Ben Johnson
 *
 */

// Required libraries include USB Host Shield, MDD10A motor controller //

#include "XBOXRECV.h" //Allows XBox 360 controller//
#include "adk.h"
#include "usbhub.h" //Activates Arduino ADK USB port //
#include "MDD10A.h" //Motor controller library for Cytron MDD10A motor controller
#include "Adafruit_NeoPixel.h"
#include "SimpleTimer.h"
#include "a_Robot.h"//Brought over from previous code
USB Usb;
USBHub  Hub1(&Usb); // The controller has a built in hub, so this instance is needed
XBOXRECV Xbox(&Usb);

MDD10A mc;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 17, NEO_GRB + NEO_KHZ800);
SimpleTimer timer;
//Expect -400 through 400
float DriveRightFrontSpeed;
float DriveRightRearSpeed;
float DriveLeftFrontSpeed;
float DriveLeftRearSpeed;
float LiftSpeed;
float ClawSpeed;
float prevDriveRightFrontSpeed;
float prevDriveRightRearSpeed;
float prevDriveLeftFrontSpeed;
float prevDriveLeftRearSpeed;
float prevLiftSpeed;
//unnecessary torqueLimit
//int TorqueLimitLift;
//torqueLimit();
//controller variables

float TriggerAggregate;
int LR2Aggregate;
int YPress;
int32_t StartTime;
bool _autoRunning;
int loopcnt = 0;
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect on ADK using built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // halt
  }
  Serial.print(F("\r\nXBOX Library Started"));
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(50);
 for(int i=0;i<12;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(74,154,211)); // GeoramaBlue.

    pixels.show(); // This sends the updated pixel color to the hardware.

  }
  timer.setInterval(1, RunRobot);
  timer.setInterval(20, Breathe);
}

void loop() {

     timer.run();

  }

int curlight = 0;
float SpeedLimit = .6;


void Breathe() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.


  for(int i=0;i<12;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if (curlight==i)
      pixels.setPixelColor(i, pixels.Color(74,154,211)); // GeoramaBlue.
    else
      pixels.setPixelColor(i, pixels.Color(1,1,1)); // off

}
    pixels.show(); // This sends the updated pixel color to the hardware.
  curlight++;

  if (curlight>=12)
    curlight=0;
}

void RunRobot() {
// put your main code here, to run repeatedly:

////////////////////////////////Stock code//////////////////////////////////////

//Getting Xbox controller input

  /*if (Xbox.getAnalogHat(LeftHatY) > 7500) {
     //Going backwards
     y_axis = -1 * map(Xbox.getAnalogHat(LeftHatY), 7500, 32767, 0, 255);
   }
   else if (Xbox.getAnalogHat(LeftHatY) < -7500)
   {
     //going forwards
     y_axis = map(Xbox.getAnalogHat(LeftHatY), -7500, -32767, 0, 255);
   }



   if (Xbox.getAnalogHat(LeftHatX) > 7500) {
     x_axis = map(Xbox.getAnalogHat(LeftHatX), 7500, 32767, 0, -255);
   }
   else if (Xbox.getAnalogHat(LeftHatX) < -7500)
   {
     x_axis = map(Xbox.getAnalogHat(LeftHatX), -7500, -32767, 0, 255);
   }

 if (Xbox.getButtonClick(UP))
 {
   SpeedLimit = SpeedLimit + .1;
   }


 if (Xbox.getButtonClick(DOWN))
 {
   SpeedLimit = SpeedLimit - .1;
   }

DriveLeftSpeed = y_axis - x_axis;
DriveRightSpeed = y_axis + x_axis;


     loopcnt++;
if (loopcnt >= 20)
{
 loopcnt = 0;

DriveLeftSpeed = DriveLeftSpeed * abs(DriveLeftSpeed) / 255;
DriveRightSpeed = DriveRightSpeed * abs(DriveRightSpeed) / 255;

 DriveLeftSpeed = SpeedLimit * DriveLeftSpeed;
 DriveRightSpeed = SpeedLimit * DriveRightSpeed;

 //write to motor controllers
 mc.setMotorSpeed(2, DriveLeftSpeed * -1);
 mc.setMotorSpeed(1, DriveRightSpeed * -1);
}


}
}*/
Usb.Task();

float DriveRightFrontSpeed = 0;
float DriveRightRearSpeed = 0;
float DriveLeftFrontSpeed = 0;
float DriveLeftRearSpeed =0;
float LeftJoystickY = 0;
float LeftJoystickX = 0;
float RightJoystickY = 0;
float RightJoystickX = 0;
float LiftSpeed = 0;
//float ClawSpeed;
//float y_axis = 0;
//float x_axis = 0;
/* float DriveLeftSpeed = 0; //Max 255 sent to motor controller library//
  float DriveRightSpeed = 0; //Max 255 sent to motor controller library//
  */

{




for (uint8_t i = 0; i < 4; i++)
{


  if (Xbox.Xbox360Connected[i])
  {



    if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500)
    {
      LeftJoystickY = 400.0 / 32767 * Xbox.getAnalogHat(LeftHatY, i);
    }

    if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500)
    {
      LeftJoystickX = 400.0 / 32767 * Xbox.getAnalogHat(LeftHatX, i);
    }


    if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500)
    {
      RightJoystickY = 400.0 / 32767 * Xbox.getAnalogHat(RightHatY, i);
    }

    if (Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500)
    {
      RightJoystickX = 400.0 / 32767 * Xbox.getAnalogHat(RightHatX, i);
    }

    //L2 Trigger
    if (Xbox.getButtonPress(R2, i))
    {
      TriggerAggregate = 400.0 / 255 * Xbox.getButtonPress(R2, i) * 1;
    }
    //R2 Trigger
    else if (Xbox.getButtonPress(L2, i))
    {
      TriggerAggregate = 400.0 / 255 * Xbox.getButtonPress(L2, i) * -1;
    }



    if (Xbox.getButtonPress(L1, i))
    {
      LR2Aggregate = 1;

    }
    //R1 Button
    else if (Xbox.getButtonPress(R1, i))
    {
      LR2Aggregate = -1;

    }



    if (Xbox.getButtonPress(Y, i))
    {
      YPress = 1;
    }

    if (Xbox.getButtonPress(A, i)) {


    }
    if (Xbox.getButtonClick(A, i))
    {
      //APress = 1;
  _autoRunning = true;
    }
    if (Xbox.getButtonClick(START, i))
    {
      //StartButton = 1;
    }


    if (Xbox.getButtonClick(LEFT, i))
    {
      //DPadLeftRight = -1;
    }
    else if (Xbox.getButtonPress(RIGHT, i))
    {
      //DPadLeftRight = 1;
    }
    else if (Xbox.getButtonClick(UP, i))
    {
      //DPadLeftRight = 2;
    }
    else if (Xbox.getButtonPress(DOWN, i))
    {
      //DPadLeftRight = -2;
    }


  }
}
}

if (_autoRunning)
{
  int32_t time = millis() - StartTime;

    if (time < 4000) //forward
    {
      DriveLeftFrontSpeed = -225;
      DriveRightFrontSpeed = -255;
      DriveRightRearSpeed = -255;
      DriveLeftRearSpeed = -255;
    }
    else {

      _autoRunning = false;
    }
}
///////////////////////    Map the Controller to wheel speeds  /////////////////
DriveRightFrontSpeed = LeftJoystickY - RightJoystickX - LeftJoystickX;
DriveRightRearSpeed = LeftJoystickY - RightJoystickX + LeftJoystickX;
DriveLeftFrontSpeed = LeftJoystickY + RightJoystickX + LeftJoystickX;
DriveLeftRearSpeed = LeftJoystickY + RightJoystickX - LeftJoystickX;


///////////////////////////////////////Drive////////////////////////////////////

loopcnt++;
if (loopcnt >= 20)
{
loopcnt = 0;

  	//DriveRightSpeed
  	if (DriveRightFrontSpeed < -400)
  		DriveRightFrontSpeed = -400;
  	if (DriveRightFrontSpeed > 400)
  		DriveRightFrontSpeed = 400;

  	DriveRightFrontSpeed = map(DriveRightFrontSpeed, -400, 400, -255, 255);

  	if (DriveRightRearSpeed < -400)
  		DriveRightRearSpeed = -400;
  	if (DriveRightRearSpeed > 400)
  		DriveRightRearSpeed = 400;

  	DriveRightRearSpeed = map(DriveRightRearSpeed, -400, 400, -255, 255);

  	//prevDriveRightFrontSpeed = torqueLimit(prevDriveRightFrontSpeed, DriveRightFrontSpeed, TorqueLimitDrive);

  	mc.setMotorSpeed(0, DriveRightFrontSpeed);

  	//prevDriveRightRearSpeed = torqueLimit(prevDriveRightRearSpeed, DriveRightRearSpeed, TorqueLimitDrive);

  	mc.setMotorSpeed(1, DriveRightRearSpeed * -1);

  	//Serial.print(DriveRightSpeed);
  	//Serial.print("\t");

  	//DriveLeftSpeed
  	if (DriveLeftFrontSpeed < -400)
  		DriveLeftFrontSpeed = -400;
  	if (DriveLeftFrontSpeed > 400)
  		DriveLeftFrontSpeed = 400;

  	DriveLeftFrontSpeed = map(DriveLeftFrontSpeed, -400, 400, -255, 255);

  	if (DriveLeftRearSpeed < -400)
  		DriveLeftRearSpeed = -400;
  	if (DriveLeftRearSpeed > 400)
  		DriveLeftRearSpeed = 400;

  	DriveLeftRearSpeed = map(DriveLeftRearSpeed, -400, 400, -255, 255);

  	//prevDriveLeftFrontSpeed = torqueLimit(prevDriveLeftFrontSpeed, DriveLeftFrontSpeed, TorqueLimitDrive);

  	mc.setMotorSpeed(4, DriveLeftFrontSpeed * -1);

  	//prevDriveLeftRearSpeed = torqueLimit(prevDriveLeftRearSpeed, DriveLeftRearSpeed, TorqueLimitDrive);

  	mc.setMotorSpeed(5, DriveLeftRearSpeed * -1);

  	//Serial.print(DriveLeftSpeed);
  	//Serial.print("\t");


  	if (LiftSpeed < -400)
  		LiftSpeed = -400;
  	if (LiftSpeed > 400)
  		LiftSpeed = 400;

  	LiftSpeed = map(LiftSpeed, -400, 400, -255, 255);

  	if (LiftSpeed < 0) {
  		mc.setMotorSpeed(3, -1 * LiftSpeed);
  		mc.setMotorSpeed(2, -1 * LiftSpeed);
  	}
  	else {
  		mc.setMotorSpeed(3, -1 * LiftSpeed);
  		mc.setMotorSpeed(2, -1 * LiftSpeed);
  	}


  	//IntakeSpeed

  	if (ClawSpeed < -400)
  		ClawSpeed = -400;
  	if (ClawSpeed > 400)
  		ClawSpeed = 400;

  	ClawSpeed = map(ClawSpeed, -400, 400, -255, 255);

  	mc.setMotorSpeed(2, ClawSpeed);

  	mc.setMotorSpeed(6, 0);

  }
}
