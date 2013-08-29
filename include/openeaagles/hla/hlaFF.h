//------------------------------------------------------------------------------
// hlaFormFunc() -- parser form function for HLA networks
//------------------------------------------------------------------------------
#ifndef __Eaagles_Network_Hla_FormFunc_H__
#define __Eaagles_Network_Hla_FormFunc_H__

//#include "eaagles/basic/support.h"

namespace Eaagles {
   namespace Network {
      namespace Basic { class Object; }
      namespace Hla {
         extern Basic::Object* hlaFormFunc(const char* const formname);
      }
   }
}

#endif
