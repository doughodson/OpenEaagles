
#ifndef __oe_base_factory_H__
#define __oe_base_factory_H__

#include <string>

namespace oe {
namespace base {

class Object;
Object* factory(const std::string&);
}
}

#endif

