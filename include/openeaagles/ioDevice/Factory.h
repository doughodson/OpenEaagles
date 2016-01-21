//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_IoDevice_Factory_H__
#define __oe_IoDevice_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace IoDevice {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace IoDevice
}  // end namespace oe

#endif
