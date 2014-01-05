//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_BasicGL_Factory_H__
#define __Eaagles_BasicGL_Factory_H__

namespace Eaagles {

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
