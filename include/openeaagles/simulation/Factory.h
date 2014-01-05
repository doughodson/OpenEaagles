//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Factory_H__
#define __Eaagles_Simulation_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Simulation {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Simulation
}  // end namespace Eaagles

#endif
