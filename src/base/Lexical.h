//------------------------------------------------------------------------------
// Class:  Lexical
//------------------------------------------------------------------------------
#include <fstream>

#ifndef _oe_base_Lexical_H_
#define _oe_base_Lexical_H_

#ifndef __FLEX_LEXER_H
#define yyFlexLexer baseFlexLexer
#include "../base/FlexLexer.h"
#undef yyFlexLexer
#endif

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class:  Lexical
// Base class:  FlexLexer -> baseFlexLexer -> Lexical
//
// Description:  Lexical generator for the input files.
//
//------------------------------------------------------------------------------
class Lexical : public baseFlexLexer {
public:
    Lexical(std::fstream* f) : baseFlexLexer(f) {
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


