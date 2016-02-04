
#ifndef __oe_simulation_factory_H__
#define __oe_simulation_factory_H__

namespace oe {

namespace basic { class Object; }

namespace simulation {

// factory function
basic::Object* factory(const char* name);

}
}

#endif
