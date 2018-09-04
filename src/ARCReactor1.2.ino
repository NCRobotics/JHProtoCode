
/** Base code for arcade drive (single joystick) for ARC Reactor July 2017
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


USB Usb;
USBHub  Hub1(&Usb); // The controller has a built in hub, so this instance is needed
XBOXRECV Xbox(&Usb);

MDD10A mc;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, 17, NEO_GRB + NEO_KHZ800);
SimpleTimer timer;

int32_t StartTime;

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

int loopcnt = 0;
int curlight = 0;
float SpeedLimit = .6;
bool autoRightRunning = false;
bool autoSkillsRunning = false;
bool autoLeftRunning = false;
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
  Usb.Task();

  float DriveLeftSpeed = 0; //Max 255 sent to motor controller library//
  float DriveRightSpeed = 0; //Max 255 sent to motor controller library//
  float Clawspeed = 0;
  float Liftspeed = 0;
  float y_axis = 0;
  float x_axis = 0;
  {


    //Getting Xbox controller input from the left joystick - Map function converts Xbox joystick position to the motor controller speeds

    if (Xbox.getAnalogHat(LeftHatY) > 7500) {
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

    if (Xbox.getButtonClick(Y)) // switch to autoSkillsRunning if needed
    {
      autoRightRunning = true;
      StartTime = millis();
    }

    if (Xbox.getButtonClick(X))
    {
      autoLeftRunning = true;
      StartTime = millis();
    }

    if (Xbox.getButtonClick(UP))
    {
      SpeedLimit = SpeedLimit + .1;
    }


    if (Xbox.getButtonClick(DOWN))
    {
      SpeedLimit = SpeedLimit - .1;
    }

    if (Xbox.getButtonPress(A))
    {
      Liftspeed =   123 ;
    }

    if (Xbox.getButtonPress(B))
    {
      Liftspeed = -123;
    }

    if (Xbox.getButtonPress(R1))
    {
      Clawspeed =  100;
    }

    if (Xbox.getButtonPress(L1))
    {
      Clawspeed = -100;
    }
    if (Xbox.getButtonClick(DOWN))
    {
      autoLeftRunning = true;
      StartTime = millis();
    }
    DriveLeftSpeed = y_axis - x_axis;
    DriveRightSpeed = y_axis + x_axis;

    if (autoRightRunning)
    {
      int32_t time = millis() - StartTime;
      if (time < 1000) //forward
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
      else if (time < 2000) //drop lift by going backwards
      {
        DriveLeftSpeed = 150;
        DriveRightSpeed = 150;
      }
      else if (time < 2700) // lower lift
      {
        Liftspeed = -123;
      }
      else if (time < 4300) // drive forward to unload preload
      {
        DriveLeftSpeed = -200;
        DriveRightSpeed = -200;
      }
      else if (time < 5200) // lower "cone lift"
      {
        Clawspeed = 100;
      }
/*      else if (time < 5800) // drive forward to pick up mobile goal
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }  */
      else if (time < 5600) // raise lift
      {
        Liftspeed = 123;
      }

      else {
        autoRightRunning = false;
      }
    }
    if (autoLeftRunning)
    {
      int32_t time = millis() - StartTime;
      if (time < 1000) //forward
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
      else if (time < 2000) //drop lift by going backwards
      {
        DriveLeftSpeed = 150;
        DriveRightSpeed = 150;
      }
      else if (time < 2700) // lower lift
      {
        Liftspeed = -123;
      }
      else if (time < 4300) // drive forward to unload preload
      {
        DriveLeftSpeed = -200;
        DriveRightSpeed = -200;
      }
      else if (time < 5200) // lower "cone lift"
      {
        Clawspeed = 100;
      }
/*      else if (time < 5800) // drive forward to pick up mobile goal
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }  */
      else if (time < 5500) // raise lift
      {
        Liftspeed = 123;
      }
      else if (time < 5800) // raise lift
      {
        Liftspeed = 123;
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
/*      if (time < 1000) //forward
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
      else if (time < 1500) //drop lift by going backwards
      {
        DriveLeftSpeed = 255;
        DriveRightSpeed = 255;
      }
      else if (time < 2400) // lower lift
      {
        Liftspeed = -123;
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
      else if (time < 3900) // drive forward to unload preload
      {
        DriveLeftSpeed = -125;//was 110
        DriveRightSpeed = -125;//was 110
      }
      else if (time < 4800) // lower "cone lift"
      {
        Clawspeed = 100;
      }

      else if (time < 6400) // raise lift
      {
        Liftspeed = 123;
      } */

      else if (time < 7000) // drive backwards
        {
          DriveLeftSpeed = 195;
          DriveRightSpeed = 255;
        }
        else if (time < 7940) // spin left, 900
        {
          DriveLeftSpeed = -255;
          DriveRightSpeed = 255;
        }
        else if (time < 89040) // go forwards
        {
          DriveLeftSpeed = -255;
          DriveRightSpeed = -255;
        }
        else if (time < 10540)
        {
          Liftspeed = -123;
        }
        else if (time < 10040)//back up and park
        {
          DriveLeftSpeed = 255;
          DriveRightSpeed = 255;
        }

        else{
          autoLeftRunning = false;

        }
      }


    if (autoSkillsRunning)
    {
      int32_t time = millis() - StartTime;
      if (time < 1000)
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
      else if (time < 1500)
      {
        DriveLeftSpeed = 255;
        DriveRightSpeed = 255;
      }
      else if (time < 2300)
      {
        Liftspeed = -123;
      }
      else if (time < 7700)
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
      else if (time < 8600)
      {
        Clawspeed = 100;
      }
      else if (time < 9500)
      {
        Liftspeed = 123;
      }
      else if (time < 13100)
      {
        DriveLeftSpeed = -255;
        DriveRightSpeed = -255;
      }
      else if (time < 11400)
      {
        Liftspeed = -123;
      }
      else if (time < 14900)
      {
        DriveLeftSpeed = 255;
        DriveRightSpeed = 255;
      }

      else {
        autoSkillsRunning = false;
      }
    }

    loopcnt++;
    if (loopcnt >= 20)
    {
      loopcnt = 0;

      DriveLeftSpeed = DriveLeftSpeed * abs(DriveLeftSpeed) / 255;
      DriveRightSpeed = DriveRightSpeed * abs(DriveRightSpeed) / 255;

      DriveLeftSpeed = SpeedLimit * DriveLeftSpeed;
      DriveRightSpeed = SpeedLimit * DriveRightSpeed;

      //write to motor controllers
      mc.setMotorSpeed(2, DriveLeftSpeed);
      mc.setMotorSpeed(1, DriveRightSpeed);
      mc.setMotorSpeed(3, Liftspeed * -1);
      mc.setMotorSpeed(4, Clawspeed);
      mc.setMotorSpeed(5, Clawspeed * -1);

    }


  }
}
