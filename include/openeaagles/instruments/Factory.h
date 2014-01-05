//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Instruments_Factory_H__
#define __Eaagles_Instruments_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Instruments {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Instruments
}  // end namespace Eaagles

#endif
