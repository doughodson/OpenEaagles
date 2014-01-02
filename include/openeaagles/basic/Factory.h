//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory for the basic library
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Factory_H__
#define __Eaagles_Basic_Factory_H__

namespace Eaagles {
namespace Basic {

class Object;

class Factory
{
public:
   static Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Basic
}  // end namespace Eaagles

#endif
