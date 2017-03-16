
#ifndef __oe_rprfom_factory_H__
#define __oe_rprfom_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace rprfom {
base::Object* factory(const std::string&);
}
}

#endif

