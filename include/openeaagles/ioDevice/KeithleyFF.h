//------------------------------------------------------------------------------
// Form function for the Keithley I/O device
//------------------------------------------------------------------------------
#ifndef __Eaagles_IoDevice_Keithley_FormFunc_H__
#define __Eaagles_IoDevice_Keithley_FormFunc_H__

namespace Eaagles {
   namespace Basic { class Object; }
   namespace IoDevice {
      namespace Keithley {
         extern Eaagles::Basic::Object* keithleyFormFunc(const char* formname);
      }
   }
}

#endif
