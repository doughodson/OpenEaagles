
#ifndef __oe_hla_factory_H__
#define __oe_hla_factory_H__

namespace oe {

namespace base { class Object; }

namespace hla {

// factory function
base::Object* factory(const char* const name);

}
}

#endif
