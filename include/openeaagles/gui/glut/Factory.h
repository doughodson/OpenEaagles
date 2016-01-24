//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_glut_Factory_H__
#define __oe_glut_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace glut {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace glut
}  // end namespace oe

#endif
