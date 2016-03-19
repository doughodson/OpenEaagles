
#ifndef __oe_base_edlParser_H__
#define __oe_base_edlParser_H__

namespace oe {

namespace base {

class Object;
typedef Object* (*FactoryFunc)(const char* name);
extern Object* edlParser(const char* filename, FactoryFunc factory, int* numErrors = 0);

}
}

#endif

