//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Glut_Factory_H__
#define __Eaagles_Glut_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Glut {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Glut
}  // end namespace Eaagles

#endif
