//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Maps_Factory_H__
#define __Eaagles_Maps_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Maps {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Maps
}  // end namespace Eaagles

#endif
