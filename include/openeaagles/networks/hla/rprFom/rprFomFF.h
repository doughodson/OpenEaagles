//------------------------------------------------------------------------------
// rprFormFunc() -- parser form function for HLA RPR FOM
//------------------------------------------------------------------------------
#ifndef __Eaagles_Network_Hla_RprFom_FormFunc_H__
#define __Eaagles_Network_Hla_RprFom_FormFunc_H__

namespace Eaagles {
   namespace Basic { class Object; }
   namespace Network {
      namespace Hla {
         namespace RprFom {
            extern Basic::Object* formFunc(const char* const formname);
         }
      }
   }
}

#endif
