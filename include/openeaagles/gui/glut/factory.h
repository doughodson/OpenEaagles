
#ifndef __oe_glut_factory_H__
#define __oe_glut_factory_H__

namespace oe {

namespace base { class Object; }

namespace glut {

// factory function
base::Object* factory(const char* name);

}
}

#endif
