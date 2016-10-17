
#ifndef __oe_recorder_factory_H__
#define __oe_recorder_factory_H__

#include <string>

namespace oe {

namespace base { class Object; }

namespace recorder {

// factory function
base::Object* factory(const std::string& name);

}
}

#endif
