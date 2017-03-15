
#ifndef __oe_graphics_factory_H__
#define __oe_graphics_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace graphics {
base::Object* factory(const std::string&);
}
}

#endif
