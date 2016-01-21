//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_Terrain_Factory_H__
#define __oe_Terrain_Factory_H__

namespace oe {

namespace Basic { class Object; }

namespace Terrain {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Terrain
}  // end namespace oe

#endif
