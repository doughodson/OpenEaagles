
#ifndef __oe_lua_factory_H__
#define __oe_lua_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace lua {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
