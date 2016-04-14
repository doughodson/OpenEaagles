
#ifndef __oe_dis_factory_H__
#define __oe_dis_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace dis {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
