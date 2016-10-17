%{
//------------------------------------------------------------------------------
// Description: Scans EDL syntax and generates tokens for parser
//------------------------------------------------------------------------------

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
#pragma warning(disable: 4996)
#pragma warning(disable: 4005)
#endif

%}


%{
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "openeaagles/base/Object.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/Integer.hpp"
#include "openeaagles/base/Float.hpp"
#include "openeaagles/base/Boolean.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/List.hpp"
#include "openeaagles/base/util/str_utils.hpp"
#include "EdlScanner.hpp"
#include "EdlParser.hpp"

#define YY_BREAK  /* We'll put in the break commands to stop the warnings */

%}

/*** Flex declarations and Options ***/

/* generate C++ scanner class */
%option c++

/* no calls to yywrap */
%option noyywrap

/* no calls to yywrap */
%option nounistd

/* change the name of the scanner class - results in "edlFlexLexer" */
%option prefix="edl"

/* derived 'oe::base::EdlScanner' is a subclass of edlFlexLexer */
%option yyclass="oe::base::EdlScanner"

D           [0-9]
L           [a-zA-Z0-9~!@#$%^&*\-_+=<>?/]
H           [a-fA-F0-9]
A           [a-zA-Z0-9_]
E           [Ee][+-]?{D}+
FS          (f|F|l|L)
IS          (u|U|l|L)*
LN          line

%%

[#]{LN}?[ ]+{D}+[ ]+\"(\\.|[^\\"])*\".*$ {
           //-----------------------------------------------------------------------------
           // Use C preprocessor inserted text to cue filename and line number information
           // # linenum filename flags
           // after filename, there comes zero or more flags '1', '2', '3' or '4'
           // Meaning of flags:
           //      '1' = indicates the start of new file
           //      '2' = indicates the returning to a file (after having included another file)
           //      '3' = indicates the following text comes from a system header file
           //      '4' = indicates the following text should be treated as being wrapped
           //            in an inplicit extern "C" block
           //
           // For examples  # 21 "test.cf"    -- means line 21 of test.cf
           //-----------------------------------------------------------------------------
           char* i1 = std::strchr(yytext,'\"');     // looking for first quote (")
           char* i2 = std::strrchr(yytext,'\"');    // looking for last quote (")
           char* j1 = std::strchr(yytext,' ');      // looking for first space
           *i1 = '\0';                              // replace both quotes with 0
           *i2 = '\0';
           line = std::atoi(j1+1) - 1;              // determine line number
           utStrcpy(fname, FNAME_SIZE, i1+1);
           break;
        }

[/][/].*$  {
           //-----------------------------------------------------------------------------
           // ignore C++ like comments
           //-----------------------------------------------------------------------------
           break;
        }

\\\n    {
           //-----------------------------------------------------------------------------
           // ignore backslashes at end of lines
           //-----------------------------------------------------------------------------
           line++;
           break;
        }

\n      {
           //-----------------------------------------------------------------------------
           // count line numbers
           //-----------------------------------------------------------------------------
           line++;
           break;
        }

[ ,\t\v\f]  {
           //-----------------------------------------------------------------------------
           // ignore white characters
           //-----------------------------------------------------------------------------
           break;
        }

[(){}\[\]]  {
           //-----------------------------------------------------------------------------
           // special characters
           //-----------------------------------------------------------------------------
           return *yytext;
        }

true    {
           //-----------------------------------------------------------------------------
           // Boolean constant: true
           //-----------------------------------------------------------------------------
           yylval.bval = true;
           return BOOLconstant;
        }

TRUE    {
           //-----------------------------------------------------------------------------
           // Boolean constant: true
           //-----------------------------------------------------------------------------
           yylval.bval = true;
           return BOOLconstant;
        }

false   {
           //-----------------------------------------------------------------------------
           // Boolean constant: false
           //-----------------------------------------------------------------------------
           yylval.bval = false;
           return BOOLconstant;
        }

FALSE   {
           //-----------------------------------------------------------------------------
           // Boolean constant: false
           //-----------------------------------------------------------------------------
           yylval.bval = false;
           return BOOLconstant;
        }

0[xX]{H}+{IS}? {
           //-----------------------------------------------------------------------------
           // decode hex into integer: 0Xnnn
           //-----------------------------------------------------------------------------
           bool nflg = false;
           char cbuf[64];
           utStrcpy(cbuf, sizeof(cbuf), yytext);
           if (std::strlen(cbuf) == 10) {
              if (cbuf[2] == '8') { cbuf[2] = '0'; nflg = true; }
              if (cbuf[2] == '9') { cbuf[2] = '1'; nflg = true; }
              if (cbuf[2] == 'a' || cbuf[2] == 'A') { cbuf[2] = '2'; nflg = true; }
              if (cbuf[2] == 'b' || cbuf[2] == 'B') { cbuf[2] = '3'; nflg = true; }
              if (cbuf[2] == 'c' || cbuf[2] == 'C') { cbuf[2] = '4'; nflg = true; }
              if (cbuf[2] == 'd' || cbuf[2] == 'D') { cbuf[2] = '5'; nflg = true; }
              if (cbuf[2] == 'e' || cbuf[2] == 'E') { cbuf[2] = '6'; nflg = true; }
              if (cbuf[2] == 'f' || cbuf[2] == 'F') { cbuf[2] = '7'; nflg = true; }
           }
           long ii = std::strtol(cbuf, 0, 16);
           if (nflg) ii = (ii | 0x80000000);
           yylval.lval = ii;
           return INTEGERconstant;
        }

0{D}+{IS}?  {
           //-----------------------------------------------------------------------------
           // decode octal into integer: 0nnn
           //-----------------------------------------------------------------------------
           bool nflg = false;
           char cbuf[64];
           utStrcpy(cbuf, sizeof(cbuf), yytext);
           if (std::strlen(cbuf) == 12) {
              if (cbuf[1] == '2') { cbuf[1] = '0'; nflg = true; }
              if (cbuf[1] == '3') { cbuf[1] = '1'; nflg = true; }
           }
           long ii = std::strtol(cbuf, 0, 8);
           if (nflg) ii = (ii | 0x80000000);
           yylval.lval = ii;
           return INTEGERconstant;
        }

[+-]?{D}+{IS}? {
           //-----------------------------------------------------------------------------
           // standard integer
           //-----------------------------------------------------------------------------
           yylval.lval = std::atoi(yytext);
           return INTEGERconstant;
        }

[+-]?{D}+{E}{FS}? {
           //-----------------------------------------------------------------------------
           // floating point format
           //-----------------------------------------------------------------------------
           yylval.dval = std::atof(yytext);
           return FLOATINGconstant;
        }

[+-]?{D}*"."{D}+({E})?{FS}? {
           //-----------------------------------------------------------------------------
           // floating point format
           //---------------------------------------------------------------------------*/
           yylval.dval = std::atof(yytext);
           return FLOATINGconstant;
        }

[+-]?{D}+"."{D}*({E})?{FS}? {
           //-----------------------------------------------------------------------------
           // floating point format
           //---------------------------------------------------------------------------*/
           yylval.dval = std::atof(yytext);
           return FLOATINGconstant;
        }

\"(\\.|[^\\"])*\"   {
           //-----------------------------------------------------------------------------
           // literal string (i.e., in double quotes): "hi there"
           // return STRING without quotes
           //-----------------------------------------------------------------------------
           size_t slen = std::strlen(yytext)+1;
           yylval.cvalp = new char[slen];
           utStrcpy(yylval.cvalp,slen,yytext+1);
           yylval.cvalp[strlen(yylval.cvalp)-1] = 0;
           return STRING_LITERAL;
        }

\<(\\.|[^\\"])*\>   {
           //-----------------------------------------------------------------------------
           // literal string (i.e., in brackets): <hi there>
           // return STRING without quotes
           //-----------------------------------------------------------------------------
           size_t slen = std::strlen(yytext)+1;
           yylval.cvalp = new char[slen];
           utStrcpy(yylval.cvalp,slen,yytext+1);
           yylval.cvalp[strlen(yylval.cvalp)-1] = 0;
           return STRING_LITERAL;
        }

{L}+[:] {
           //-----------------------------------------------------------------------------
           // slot-id is one or more legal characters followed by a ':'
           //-----------------------------------------------------------------------------
           size_t slen = std::strlen(yytext)+1;
           yylval.cvalp = new char[slen];
           utStrcpy(yylval.cvalp,slen,yytext);
           yylval.cvalp[strlen(yylval.cvalp)-1] = 0; // remove the :
           return SLOT_ID;
        }

{L}+    {
           //-----------------------------------------------------------------------------
           // ident: one or more legal characters
           //-----------------------------------------------------------------------------
           size_t slen = std::strlen(yytext)+1;
           yylval.cvalp = new char[slen];
           utStrcpy(yylval.cvalp,slen,yytext);
           return IDENT;
        }

.       {
           //-----------------------------------------------------------------------------
           // ignore all other characters
           //-----------------------------------------------------------------------------
           break;
        }

%%

