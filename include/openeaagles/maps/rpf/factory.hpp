
#ifndef __oe_maps_rpf_factory_H__
#define __oe_maps_rpf_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace rpf {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
