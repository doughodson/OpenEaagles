//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_BasicGL_Factory_H__
#define __oe_BasicGL_Factory_H__

namespace oe {

namespace Basic { class Object; }

namespace BasicGL {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace BasicGL
}  // end namespace Eaagles

#endif
