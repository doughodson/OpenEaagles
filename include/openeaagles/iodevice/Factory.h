//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_iodevice_Factory_H__
#define __oe_iodevice_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace iodevice {

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
