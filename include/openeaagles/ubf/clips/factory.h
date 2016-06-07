
#ifndef __oe_clips_factory_H__
#define __oe_clips_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace clips {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
