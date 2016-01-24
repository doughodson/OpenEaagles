//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_network_dis_Factory_H__
#define __oe_network_dis_Factory_H__

namespace oe {

namespace basic { class Object; }

namespace network {
namespace dis {

class Factory
{
public:
   static basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

} // End dis namespace
} // End network namespace
} // End oe namespace

#endif
