//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Sensor_Factory_H__
#define __Eaagles_Sensor_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Sensor {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Sensor
}  // end namespace Eaagles

#endif
