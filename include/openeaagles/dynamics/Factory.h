//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_Dynamics_Factory_H__
#define __oe_Dynamics_Factory_H__

namespace oe {

namespace Basic { class Object; }

namespace Dynamics {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Dynamics
}  // end namespace oe

#endif
