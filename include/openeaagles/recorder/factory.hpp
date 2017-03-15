
#ifndef __oe_recorder_factory_H__
#define __oe_recorder_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace recorder {
base::Object* factory(const std::string&);
}
}

#endif
