//==============================================================================
// UsbJoystick -- Linux implementation
//==============================================================================
#include <errno.h>

#include <iostream>
#include <cstdio>

#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/joystick.h>

#include "UsbJoystick_linux.h"
#include "openeaagles/base/util/str_utils.hpp"
#include "openeaagles/base/util/system.hpp"

namespace oe {
namespace iodevice {

IMPLEMENT_SUBCLASS(UsbJoystick, "UsbJoystick")
EMPTY_SLOTTABLE(UsbJoystick)
EMPTY_SERIALIZER(UsbJoystick)
EMPTY_DELETEDATA(UsbJoystick)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
UsbJoystick::UsbJoystick()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// Init our data
//------------------------------------------------------------------------------
void UsbJoystick::initData()
{
   stream = 0;
   deviceName[0] = '\0';
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void UsbJoystick::copyData(const UsbJoystick& org, const bool)
{
   BaseClass::copyData(org);

   initData();
}

//------------------------------------------------------------------------------
// reset() -- reset (open) device
//------------------------------------------------------------------------------
void UsbJoystick::reset()
{
   BaseClass::reset();

   if (deviceName[0] == '\0') {

      // create the device name based on the device index
      {
         char cbuff[128];
         // search for device at "/dev/jsX" first
         std::sprintf(cbuff, "/dev/js%d", getDeviceIndex());
         if (base::doesFileExist(cbuff)) {
           base::utStrcpy(deviceName, sizeof(deviceName), cbuff);
         } else {
            // search for device at "/dev/input/jsX" next
            std::sprintf(cbuff, "/dev/input/js%d", getDeviceIndex());
            if (base::doesFileExist(cbuff)) {
               base::utStrcpy(deviceName, sizeof(deviceName), cbuff);
            }
         }
      }

      // if no device found, print message and leave
      if (deviceName[0] == '\0') {
         stream = -1;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "UsbJoystick::reset(): Valid joystick device not found" << std::endl;
         }
         return;
      }

      // try opening device
      stream = open(deviceName, O_RDONLY | O_NONBLOCK );
      if ((stream == -1) && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "UsbJoystick::reset(): Error opening device at : " << deviceName << std::endl;
      }

      // determine characteristics and print out info message
      if (stream != -1) {

         int driverVersion;
         ioctl(stream, JSIOCGVERSION, &driverVersion);

         char modelName[128];
         ioctl(stream, JSIOCGNAME(128), modelName);

         {
            unsigned char numOfAxes(0);
            ioctl(stream, JSIOCGAXES, &numOfAxes);
            unsigned short tmp = static_cast<unsigned short>(numOfAxes);
            if (tmp > MAX_AI)
                tmp = MAX_AI;
            numAI = tmp;
         }

         {
            unsigned char numOfBtns(0);
            ioctl(stream, JSIOCGBUTTONS, &numOfBtns);
            unsigned short tmp = static_cast<unsigned short>(numOfBtns);
            if (tmp > MAX_DI)
                tmp = MAX_DI;
            numDI = tmp;
         }

         std::printf("\n");
         std::printf("---------------------------\n");
         std::printf("USB Joystick Configuration:\n");
         std::printf("---------------------------\n");
         std::printf("  Port          : %s\n", deviceName);
         std::printf("  Name          : %s\n", modelName);
         std::printf("  NumAIs        : %d\n", numAI);
         std::printf("  NumDIs        : %d\n", numDI);
         std::printf("  Driver version: %d.%d.%d\n", driverVersion >> 16,
                       (driverVersion >> 8) & 0xff, driverVersion & 0xff);
         std::printf("\n");
      }
   }
}

//------------------------------------------------------------------------------
// Go get our AIs and DIs here
//------------------------------------------------------------------------------
void UsbJoystick::processInputs(const double dt, base::IoData* const pInData)
{
   js_event js; // joystick event structure

   // Loop reading joystick events ..
   while (true) {

      // read the next joystick event (if any)
      int status = read(stream, &js, sizeof(js));

      // Break out of the loop when there are no more events
      if (status != sizeof(js)) break;

      // decode the event
      switch ( js.type & ~JS_EVENT_INIT ) {

         // button event
         case JS_EVENT_BUTTON:
            {
            int n = js.number;
            if (n < numDI)
                inBits[n] = static_cast<bool>(js.value);
            }
         break;

         // axis event
         case JS_EVENT_AXIS:
            {
            int n = js.number;
            if (n < numAI)
                inData[n] = (js.value / 32767.0f);
            }
            break;

      }
   }

   // Update our base class, which will call our component DI handlers
   BaseClass::processInputs(dt,pInData);
}

}
}

