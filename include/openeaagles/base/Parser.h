//------------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------------
#ifndef __oe_base_Parser_H__
#define __oe_base_Parser_H__

namespace oe {
   namespace base {

      class Object;
      typedef Object* (*ParserFormFunc)(const char* formname);
      extern Object* lcParser(const char* filename, ParserFormFunc func, int* numErrors = 0);
   }
}


#endif

