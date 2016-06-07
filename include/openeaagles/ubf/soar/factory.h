
#ifndef __oe_soar_factory_H__
#define __oe_soar_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace soar {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
