//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __Eaagles_Network_Dis_Factory_H__
#define __Eaagles_Network_Dis_Factory_H__

namespace Eaagles {

namespace Basic { class Object; }

namespace Network {
namespace Dis {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace

#endif
