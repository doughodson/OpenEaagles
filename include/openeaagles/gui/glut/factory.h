
#ifndef __oe_glut_factory_H__
#define __oe_glut_factory_H__

namespace oe {

namespace basic { class Object; }

namespace glut {

// factory function
basic::Object* factory(const char* name);

}
}

#endif
