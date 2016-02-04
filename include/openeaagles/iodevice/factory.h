
#ifndef __oe_iodevice_factory_H__
#define __oe_iodevice_factory_H__

namespace oe {

namespace basic { class Object; }

namespace iodevice {

// factory function
basic::Object* factory(const char* name);

}
}

#endif
