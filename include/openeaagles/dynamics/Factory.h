//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_dynamics_Factory_H__
#define __oe_dynamics_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace dynamics {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace dynamics
}  // end namespace oe

#endif
