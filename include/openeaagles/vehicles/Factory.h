//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Vehicle_Factory_H__
#define __Eaagles_Vehicle_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Vehicle {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Vehicle
}  // end namespace Eaagles

#endif
