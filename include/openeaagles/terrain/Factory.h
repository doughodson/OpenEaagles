//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Terrain_Factory_H__
#define __Eaagles_Terrain_Factory_H__

namespace Eaagles {

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
}  // end namespace Eaagles

#endif
