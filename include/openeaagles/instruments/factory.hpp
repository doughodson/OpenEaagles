
#ifndef __oe_instruments_factory_H__
#define __oe_instruments_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace instruments {
base::Object* factory(const std::string&);
}
}

#endif
