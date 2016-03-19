%{
//
// Scanner for reading EDL files
//

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

%}


%{
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "openeaagles/base/Object.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Identifier.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/Boolean.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/List.h"
#include "openeaagles/base/util/str_utils.h"
#include "EdlScanner.h"
#include "Parser.hpp"

#define YY_BREAK  /* We'll put in the break commands to stop the warnings */

%}

/*** Flex declarations and Options ***/

/* generate C++ scanner class */
%option c++

/* no calls to yywrap */
%option noyywrap

/* no calls to yywrap */
%option nounistd

/* change the name of the scanner class - results in "baseFlexLexer" */
%option prefix="base"

/* derived 'oe::base::EdlScanner' is a subclass of yyFlexLexer */
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

        // cpp commands that change line number and file name
        // for examples:  # 21 "test.cf"
        //                #line 21 "test.cf"
[#]{LN}?[ ]+{D}+[ ]+\"(\\.|[^\\"])*\".*$    {
            char* i1 = std::strchr(yytext,'\"');
            char* i2 = std::strrchr(yytext,'\"');
            char* j1 = std::strchr(yytext,' ');
            *i1 = '\0';
            *i2 = '\0';
            line = std::atoi(j1+1) - 1;
            utStrcpy(fname,FNAME_SIZE,i1+1);
            break;
        }


[/][/].*$   { // ignore C++ like comments
            line++;
            break;
        }

\\\n    { // ignore backslashes at end of lines
            line++;
            break;
        }

\n      { // count line numbers
            line++;
            break;
        }

[ ,\t\v\f]  { // ignore white characters
        break;
    }

[(){}\[\]]  { // Special characters
            return *yytext;
        }

true    { // Boolean Constants; true
            yylval.bval = true;
            return BOOLconstant;
        }

TRUE    { // Boolean Constants; true
            yylval.bval = true;
            return BOOLconstant;
        }

false   { // Boolean Constants; false
            yylval.bval = false;
            return BOOLconstant;
        }

FALSE   { // Boolean Constants; false
            yylval.bval = false;
            return BOOLconstant;
        }

0[xX]{H}+{IS}? { // Decode hex into integer; 0Xnnn
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

0{D}+{IS}?  { // Decode octal into integer; 0nnn
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


[+-]?{D}+{IS}? { // Standard integer
            yylval.lval = std::atoi(yytext);
            return INTEGERconstant;
        }


[+-]?{D}+{E}{FS}? { // floating point format
            yylval.dval = std::atof(yytext);
            return FLOATINGconstant;
        }


[+-]?{D}*"."{D}+({E})?{FS}? { // floating point format
            yylval.dval = std::atof(yytext);
            return FLOATINGconstant;
        }


[+-]?{D}+"."{D}*({E})?{FS}? { // floating point format
            yylval.dval = std::atof(yytext);
            return FLOATINGconstant;
        }


\"(\\.|[^\\"])*\"   { // Literal string (i.e., in double quotes); "hi there"
            /* return STRING without quotes */
            size_t slen = std::strlen(yytext)+1;
            yylval.cvalp = new char[slen];
            utStrcpy(yylval.cvalp,slen,yytext+1);
            yylval.cvalp[strlen(yylval.cvalp)-1] = 0;
            return STRING_LITERAL;
        }

\<(\\.|[^\\"])*\>   { // Literal string (i.e., in brackets); <hi there>
            /* return STRING without quotes */
            size_t slen = std::strlen(yytext)+1;
            yylval.cvalp = new char[slen];
            utStrcpy(yylval.cvalp,slen,yytext+1);
            yylval.cvalp[strlen(yylval.cvalp)-1] = 0;
            return STRING_LITERAL;
        }


{L}+[:] { // slot-id is one or more legal characters followed by a ':'
            size_t slen = std::strlen(yytext)+1;
            yylval.cvalp = new char[slen];
            utStrcpy(yylval.cvalp,slen,yytext);
            yylval.cvalp[strlen(yylval.cvalp)-1] = 0; // remove the :
            return SLOT_ID;
        }


{L}+    { // ident: one or more legal characters
            size_t slen = std::strlen(yytext)+1;
            yylval.cvalp = new char[slen];
            utStrcpy(yylval.cvalp,slen,yytext);
            return IDENT;
        }


.       { // ignore all other characters
            break;
        }

%%

