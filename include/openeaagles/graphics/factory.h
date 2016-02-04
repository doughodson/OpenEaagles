
#ifndef __oe_graphics_factory_H__
#define __oe_graphics_factory_H__

namespace oe {

namespace basic { class Object; }

namespace graphics {

// factory function
basic::Object* factory(const char* name);

}
}

#endif
