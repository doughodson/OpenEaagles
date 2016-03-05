
#ifndef __oe_iodevice_factory_H__
#define __oe_iodevice_factory_H__

namespace oe {

namespace base { class Object; }

namespace iodevice {

// factory function
base::Object* factory(const char* name);

}
}

#endif
