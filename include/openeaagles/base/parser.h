
#ifndef __oe_base_parser_H__
#define __oe_base_parser_H__

#include <string>

namespace oe {

namespace base {

class Object;
typedef Object* (*FactoryFunc)(const std::string& name);
extern Object* edlParser(const std::string& filename, FactoryFunc factory, int* numErrors = nullptr);

}
}

#endif

