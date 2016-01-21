//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_IoDevice_Factory_H__
#define __oe_IoDevice_Factory_H__

namespace oe {

namespace Basic { class Object; }

namespace IoDevice {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace IoDevice
}  // end namespace Eaagles

#endif
