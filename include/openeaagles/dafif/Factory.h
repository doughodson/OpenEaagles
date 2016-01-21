//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_Dafif_Factory_H__
#define __oe_Dafif_Factory_H__

namespace oe {

namespace Basic { class Object; }

namespace Dafif {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Dafif
}  // end namespace oe

#endif
