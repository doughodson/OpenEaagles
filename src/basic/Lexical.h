//------------------------------------------------------------------------------
// Class:  Lexical
//------------------------------------------------------------------------------
#include <fstream>

#ifndef _Eaagles_Basic_Lexical_H_
#define _Eaagles_Basic_Lexical_H_

#ifndef __FLEX_LEXER_H
#define yyFlexLexer basicFlexLexer
#include "../basic/FlexLexer.h"
#undef yyFlexLexer
#endif

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class:  Lexical
// Base class:  FlexLexer -> basicFlexLexer -> Lexical
//
// Description:  Lexical generator for the input files.
//
//------------------------------------------------------------------------------
class Lexical : public basicFlexLexer {
public:
    Lexical(std::fstream* f) : basicFlexLexer(f) {
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

} // End Basic namespace
} // End Eaagles namespace

#endif


