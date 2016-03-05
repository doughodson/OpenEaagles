
#ifndef __oe_simulation_factory_H__
#define __oe_simulation_factory_H__

namespace oe {

namespace base { class Object; }

namespace simulation {

// factory function
base::Object* factory(const char* name);

}
}

#endif
