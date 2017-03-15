
#ifndef __oe_iodevice_factory_H__
#define __oe_iodevice_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace iodevice {
base::Object* factory(const std::string&);
}
}

#endif
