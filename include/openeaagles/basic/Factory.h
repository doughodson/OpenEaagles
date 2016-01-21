//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_Basic_Factory_H__
#define __oe_Basic_Factory_H__

namespace oe {
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
