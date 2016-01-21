//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_Otw_Factory_H__
#define __oe_Otw_Factory_H__

namespace oe {

namespace Basic { class Object; }

namespace Otw {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Otw
}  // end namespace Eaagles

#endif
