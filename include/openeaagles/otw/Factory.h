//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_otw_Factory_H__
#define __oe_otw_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace otw {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace otw
}  // end namespace oe

#endif
