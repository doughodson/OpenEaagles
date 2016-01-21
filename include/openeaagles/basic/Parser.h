//------------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------------
#ifndef __oe_basic_Parser_H__
#define __oe_basic_Parser_H__

// parser and form functions
#include "openeaagles/basic/support.h"

namespace oe {
   namespace basic {

      class Object;
      typedef Object* (*ParserFormFunc)(const char* formname);
      extern Object* lcParser(const char* filename, ParserFormFunc func, int* numErrors = 0);
   }
}


#endif

