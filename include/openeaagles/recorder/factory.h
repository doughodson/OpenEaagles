
#ifndef __oe_recorder_factory_H__
#define __oe_recorder_factory_H__

namespace oe {

namespace base { class Object; }

namespace recorder {

// factory function
base::Object* factory(const char* name);

}
}

#endif
