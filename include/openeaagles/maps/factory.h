
#ifndef __oe_maps_factory_H__
#define __oe_maps_factory_H__

namespace oe {

namespace base { class Object; }

namespace maps {

// factory function
base::Object* factory(const char* name);

}
}

#endif
