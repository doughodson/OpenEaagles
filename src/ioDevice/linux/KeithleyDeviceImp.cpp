//==============================================================================
// KeithleyDeviceImp -- Linux implementation
//==============================================================================

#include "./KeithleyDeviceImp.h"
#include "kpci.h"

namespace Eaagles {
namespace IoDevice {

IMPLEMENT_SUBCLASS(KeithleyDeviceImp,"KeithleyDevice")
EMPTY_SLOTTABLE(KeithleyDeviceImp)
EMPTY_SERIALIZER(KeithleyDeviceImp)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
KeithleyDeviceImp::KeithleyDeviceImp()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

//------------------------------------------------------------------------------
// Init our data
//------------------------------------------------------------------------------
void KeithleyDeviceImp::initData()
{
   stream = 0;

   driverOpen = false;
   openFailed = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void KeithleyDeviceImp::copyData(const KeithleyDeviceImp& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   // No copy -- just delete our the old stuff
   deleteData(); 
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void KeithleyDeviceImp::deleteData()
{
   if (stream != 0) {
      close(stream);
      stream = 0;
   }

   driverOpen = false;
   openFailed = false;
}

//------------------------------------------------------------------------------
// reset() -- Reset (open) device
//------------------------------------------------------------------------------
void KeithleyDeviceImp::reset()
{
   BaseClass::reset();

   if ( !(driverOpen || openFailed) ) {

      stream = ::open("/dev/kpci02x0", O_RDWR);
      if ( stream <= 0  ) {
         cout << "*** Error ***  Couldn't open device /dev/kpci02x0" << endl;
         cout << endl;
         openFailed = true;
      }
      else {
          driverOpen = true;
      }

      if (driverOpen) {

         // set the number of buttons and axes device supports
         numDI = NUM_DIGITAL_CHANNELS;
         if (numDI > MAX_DI) numDI = MAX_DI;

         numAI = NUM_ANALOG_CHANNELS;
         if (numAI > MAX_AI) numAI = MAX_AI;

      }
   }
}

//------------------------------------------------------------------------------
// read current info from Keithley device
//------------------------------------------------------------------------------
void KeithleyDeviceImp::processInputs(const Eaagles::LCreal dt, Basic::IoData* const pInData)
{
   Kpci310xcmb cmb;  // 3102 Combination Digital and Analog IO

   int n = read(stream, &cmb, sizeof(struct Kpci310xcmb));
   if (n < 1) {
      std::cerr << "*** Read Error on Keithley Card inside receive()" << std::endl;
   }

   if (n > 0) {
      unsigned int idx = 0;

      // copy button info into our arrays
      for (unsigned int j = 0; j < 8; j++) {
         if (idx < numDI) inBits[idx++] = (cmb.dio.pa[j] != 0);
      }

      for (unsigned int j = 0; j < 8; j++) {
         if (idx < numDI) inBits[idx++] = (cmb.dio.pb[j] != 0);
      }

      for (unsigned int j = 0; j < 7; j++) {
         if (idx < numDI) inBits[idx++] = (cmb.dio.pc[j] != 0);
      }

      // copy axis info into our arrays
      for (unsigned int j = 0; j < numAI; j++) {
         inData[j] = float(cmb.ad.channels[j]/4096.0f);
      }
   }

   // Update our base class, which will call our component DI handlers
   BaseClass::processInputs(dt,pInData);
}

} // IoDevice namespace
} // end Eaagles namespace
