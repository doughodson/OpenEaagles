//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_sensor_Factory_H__
#define __oe_sensor_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace sensor {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace sensor
}  // end namespace oe

#endif
