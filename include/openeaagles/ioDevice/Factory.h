//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory for the ioDevice library
//------------------------------------------------------------------------------
#ifndef __Eaagles_IoDevice_Factory_H__
#define __Eaagles_IoDevice_Factory_H__

namespace Eaagles {

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
