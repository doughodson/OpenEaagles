
#ifndef __oe_simulation_factory_H__
#define __oe_simulation_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace simulation {
base::Object* factory(const std::string&);
}
}

#endif
