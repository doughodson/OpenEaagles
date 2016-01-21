//------------------------------------------------------------------------------
// Form function for the 'basicGL' library
//
//    The form functions are class factories that will create an object
//    by its 'form' name and will return a pre-ref() pointer to the new
//    object or zero if the form name wasn't found. 
//
//    (see Object.h, parser.y, and various Openoe examples)
//------------------------------------------------------------------------------
#ifndef __oe_BasicGL_FormFunc_H__
#define __oe_BasicGL_FormFunc_H__

namespace oe {
   namespace Basic { class Object; }
   namespace BasicGL {
      extern Basic::Object* basicGLFormFunc(const char* name);
   }
}

#endif
