//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_maps_Factory_H__
#define __oe_maps_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace maps {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace maps
}  // end namespace oe

#endif
