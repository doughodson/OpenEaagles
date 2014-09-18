//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dynamics_Factory_H__
#define __Eaagles_Dynamics_Factory_H__

namespace Eaagles {

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
}  // end namespace Eaagles

#endif
