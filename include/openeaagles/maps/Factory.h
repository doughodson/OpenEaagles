//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_Maps_Factory_H__
#define __oe_Maps_Factory_H__

namespace oe {

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
}  // end namespace oe

#endif
