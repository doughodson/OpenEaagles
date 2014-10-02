//------------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Parser_H__
#define __Eaagles_Basic_Parser_H__

// parser and form functions
#include "openeaagles/basic/support.h"

namespace Eaagles {
   namespace Basic {

      class Object;
      typedef Object* (*ParserFormFunc)(const char* formname);
      extern Object* lcParser(const char* filename, ParserFormFunc func, int* numErrors = 0);
   }
}


#endif

