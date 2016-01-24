//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_dafif_Factory_H__
#define __oe_dafif_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace dafif {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace dafif
}  // end namespace oe

#endif
