
#ifndef __oe_terrain_factory_H__
#define __oe_terrain_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace terrain {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
