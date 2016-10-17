
#ifndef __oe_base_factory_H__
#define __oe_base_factory_H__

#include <string>

namespace oe {
namespace base {

class Object;

// factory function
Object* factory(const std::string& name);

}
}

#endif

