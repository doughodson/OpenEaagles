//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Factory_H__
#define __oe_simulation_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace simulation {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace simulation
}  // end namespace oe

#endif
