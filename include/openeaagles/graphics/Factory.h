//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_graphics_Factory_H__
#define __oe_graphics_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace graphics {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace graphics
}  // end namespace oe

#endif