
#ifndef __oe_behaviors_factory_H__
#define __oe_behaviors_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace behaviors {

base::Object* factory(const std::string& name);

}
}

#endif
