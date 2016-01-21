//------------------------------------------------------------------------------
// Class: Parser
// Base class: <this is a base class>
//
// Description: Parser for the description files.
//
// Factory function:
//
//    Object* (*ParserFormFunc)(const char* formname)
//
//       This is a user supplied function, via our constructor, that is
//       used by the parser to create objects using their 'form' names
//       (see Object.h).  The object's form name, formname, is passed to
//       the function.  The function will construct a default object and
//       return a pointer to the new object.  If the form name is not
//       recognized then no object is created and null(0) is returned.
//
//--------------------------------------------------------------------------

%token	IDENT SLOT_ID
%token	INTEGERconstant  FLOATINGconstant  BOOLconstant STRING_LITERAL

%{

// disable all deprecation warnings for now, until we fix
// they are quite annoying to see over and over again...
#if(_MSC_VER>=1400)   // VC8+
# pragma warning(disable: 4996)
#endif

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "openeaagles/basic/Parser.h"
#include "openeaagles/basic/support.h"
#include "openeaagles/basic/Object.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Float.h"
#include "openeaagles/basic/Boolean.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/List.h"
#include "Lexical.h"

static oe::Basic::Object*  result;          // Result of all our work
static oe::Basic::Lexical* lex;             // Lex generator
static oe::Basic::ParserFormFunc formFunc;  // Factory function 
static int errCount;                        // Error count

//------------------------------------------------------------------------------
// yylex() -- user defined; used by the parser to call the lexical generator
//------------------------------------------------------------------------------
inline int yylex()
{
   return lex->yylex();
}

//------------------------------------------------------------------------------
// yyerror() -- user defined; use by the parser to report errors.
//------------------------------------------------------------------------------
inline void yyerror(const char* s)
{
   std::cerr << lex->getFilename() << ", line ";
   std::cerr << lex->getLineNumber() << ": ";
   std::cerr << s << std::endl;
   errCount++;
}

//------------------------------------------------------------------------------
// gufParse() -- Returns an object of type 'formname' with its slots set to
//                values in 'argList'.
//------------------------------------------------------------------------------
static oe::Basic::Object* gufParse(const char* formname, oe::Basic::PairStream* argList)
{
    char emsg[256];
    oe::Basic::Object* form = 0;

    if (formFunc != 0) {

        // when we have a form name, use formFunc() to construct an
        // object of the form's class type.
        form = formFunc(formname);

       // set slots in our new object
       if (form != 0 && argList != 0) {
          oe::Basic::List::Item* item = argList->getFirstItem();
          while (item != 0) {
               oe::Basic::Pair* p = static_cast<oe::Basic::Pair*>(item->getValue());
               bool ok = form->setSlotByName(*p->slot(), p->object());
               if (!ok) {
                  oe::lcStrcpy(emsg, sizeof(emsg), "error while setting slot name: ");
                  oe::lcStrcat(emsg, sizeof(emsg), *p->slot());
                  yyerror(emsg);
               }
               item = item->getNext();
          }
          bool ok = form->isValid();
          if (!ok) {
             oe::lcStrcpy(emsg, sizeof(emsg), "error: invalid form: ");
             oe::lcStrcat(emsg, sizeof(emsg), formname);
             yyerror(emsg);
          }
       }
       else if (form == 0) {
          oe::lcStrcpy(emsg, sizeof(emsg), "undefined form name: ");
          oe::lcStrcat(emsg, sizeof(emsg), formname);
          yyerror(emsg);
       }

    }
    return form;
}

%}


// Defines types that our values can be, yylval.
%union {
   double                           dval;
   long                             lval;
   bool                             bval;
   char*                            cvalp;
   oe::Basic::Object*          ovalp;
   oe::Basic::Pair*            pvalp;
   oe::Basic::PairStream*      svalp;
   oe::Basic::List*            lvalp;
   oe::Basic::Number*          nvalp;
}

// Add types to some for our tokens and rules
%type  <lval>   INTEGERconstant
%type  <bval>   BOOLconstant
%type  <dval>   FLOATINGconstant
%type  <cvalp>  STRING_LITERAL
%type  <cvalp>  IDENT
%type  <cvalp>  SLOT_ID
%type  <svalp>  arglist
%type  <ovalp>  form
%type  <ovalp>  prim
%type  <pvalp>  slot_value
%type  <lvalp>  numlist
%type  <nvalp>  number


//--------------------------------------------------------------------------
// The grammar rules ---
//--------------------------------------------------------------------------
%%
file    : form                      { result = $1; }
        | SLOT_ID form              { if ($2 != 0) { result = new oe::Basic::Pair($1, $2); delete[] $1; $2->unref(); } }
        ;

arglist :                           { $$ = new oe::Basic::PairStream(); }

        | arglist form              { if ($2 != 0) {
                                        int i = $1->entries();
                                        char cbuf[20];
                                        std::sprintf(cbuf,"%i",i+1);
                                        oe::Basic::Pair* p = new oe::Basic::Pair(cbuf, $2);
                                        $2->unref();
                                        $1->put(p);
                                        p->unref();
                                        $$ = $1;
                                      }
                                    }

        | arglist prim              {
                                    int i = $1->entries();
                                    char cbuf[20];
                                    std::sprintf(cbuf,"%i",i+1);
                                    oe::Basic::Pair* p = new oe::Basic::Pair(cbuf, $2);
                                    $2->unref();
                                    $1->put(p);
                                    p->unref();
                                    $$ = $1;
                                    }

        | arglist slot_value        { $1->put($2); $2->unref(); $$ = $1; }
        ;


form    : '(' IDENT arglist ')'     { $$ = gufParse($2, $3); delete[] $2; $3->unref(); }

        | '{' arglist '}'           { $$ = (oe::Basic::Object*) $2; }
        ;


slot_value  : SLOT_ID prim          { $$ = new oe::Basic::Pair($1, $2); delete[] $1; $2->unref(); }
        | SLOT_ID form              { $$ = new oe::Basic::Pair($1, $2); delete[] $1; $2->unref(); }
        ;

prim    : STRING_LITERAL            { $$ = new oe::Basic::String($1); delete[] $1; }
        | IDENT                     { $$ = new oe::Basic::Identifier($1); delete[] $1; }
        | BOOLconstant              { $$ = new oe::Basic::Boolean($1); }
        | '[' numlist ']'           { $$ = $2; }
        | number                    { $$ = $1; }
        ;

numlist : number                    { $$ = new oe::Basic::List(); $$->put($1); $1->unref(); }
        | numlist number            { $$ = $1; $$->put($2); $2->unref(); }
        ;

number  : INTEGERconstant       { $$ = new oe::Basic::Integer($1); }
        | FLOATINGconstant      { $$ = new oe::Basic::Float($1); }
        ;
%%


namespace oe {
namespace Basic {

//------------------------------------------------------------------------------
// parse() -- Returns an Object that was constructed from
//      parsing the input file.  Func is the name of the form
//      constructor function.  
//------------------------------------------------------------------------------

Object* lcParser(const char* filename, ParserFormFunc func, int* numErrors)
{
    formFunc = func;
    result = 0;
    errCount = 0;

    // Open the file (someone else passed it through the preprocessor)
    std::fstream fin;
    fin.open(filename,std::ios::in);
    lex = new Lexical(&fin);

    //yydebug = 1;
    Object* q = 0;
    if (yyparse() == 0) q = result;

    fin.close();
    delete lex;

    if (numErrors != 0) *numErrors = errCount;
    return q;
}

} // End Basic namespace
} // End oe namespace
