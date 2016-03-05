
#ifndef __oe_graphics_factory_H__
#define __oe_graphics_factory_H__

namespace oe {

namespace base { class Object; }

namespace graphics {

// factory function
base::Object* factory(const char* name);

}
}

#endif
