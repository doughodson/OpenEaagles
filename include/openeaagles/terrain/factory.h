
#ifndef __oe_terrain_factory_H__
#define __oe_terrain_factory_H__

namespace oe {

namespace base { class Object; }

namespace terrain {

// factory function
base::Object* factory(const char* name);

}
}

#endif
