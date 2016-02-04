
#ifndef __oe_instruments_factory_H__
#define __oe_instruments_factory_H__

namespace oe {

namespace basic { class Object; }

namespace instruments {

// factory function
basic::Object* factory(const char* name);

}
}

#endif
