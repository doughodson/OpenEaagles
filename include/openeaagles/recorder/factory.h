
#ifndef __oe_recorder_factory_H__
#define __oe_recorder_factory_H__

namespace oe {

namespace basic { class Object; }

namespace recorder {

// factory function
basic::Object* factory(const char* name);

}
}

#endif
