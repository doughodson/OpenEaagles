
#ifndef __oe_network_dis_factory_H__
#define __oe_network_dis_factory_H__

namespace oe {

namespace basic { class Object; }

namespace network {
namespace dis {

// factory function
basic::Object* factory(const char* name);

}
}
}

#endif
