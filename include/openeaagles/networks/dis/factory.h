
#ifndef __oe_dis_factory_H__
#define __oe_dis_factory_H__

namespace oe {

namespace base { class Object; }

namespace dis {

// factory function
base::Object* factory(const char* name);

}
}

#endif
