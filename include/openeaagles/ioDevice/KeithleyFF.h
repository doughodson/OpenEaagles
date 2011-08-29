//------------------------------------------------------------------------------
// Form function for the Keithley I/O device
//------------------------------------------------------------------------------
#ifndef __OpenEaagles_IoDevice_Keithley_FormFunc_H__
#define __OpenEaagles_IoDevice_Keithley_FormFunc_H__

namespace Eaagles {
   namespace Basic { class Object; }
   namespace IoDevice {
      namespace Keithley {
         extern Eaagles::Basic::Object* keithleyFormFunc(const char* formname);
      }
   }
}

#endif
