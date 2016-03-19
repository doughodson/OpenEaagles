
#include <fstream>

#ifndef _oe_base_EdlScanner_H_
#define _oe_base_EdlScanner_H_

#ifndef __FLEX_LEXER_H
#define yyFlexLexer edlFlexLexer
#include "./platform/FlexLexer.h"
#undef yyFlexLexer
#endif

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: EdlScanner
// Base class:  FlexLexer -> edlFlexLexer -> EdlScanner
//
// Description: Scanner for the input files.
//
//------------------------------------------------------------------------------
class EdlScanner : public edlFlexLexer {
public:
    EdlScanner(std::fstream* f) : edlFlexLexer(f) {
        fname[0] = 0;
        line = 1;
    }

    virtual int yylex();
    const char* getFilename()    { return fname; }
    int getLineNumber()          { return line; }

private:
    static const unsigned int FNAME_SIZE = 256;
    char fname[FNAME_SIZE];   // Current file name
    int  line;                // Current line number

};

}
}

#endif


