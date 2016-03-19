//------------------------------------------------------------------------------
// Description: EDL parser
//
// Factory function:
//
//    Object* (*FactoryFunc)(const char* name)
//
//       This is a user supplied function, via our constructor, that is
//       used by the parser to create objects using their 'factory' names
//       (see Object.h).  The object's factory name is passed to
//       the function.  The function will construct a default object and
//       return a pointer to the new object.  If the name is not
//       recognized then no object is created and null(0) is returned.
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
#include "openeaagles/base/edl_parser.h"
#include "openeaagles/base/util/str_utils.h"
#include "openeaagles/base/Object.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/Identifier.h"
#include "openeaagles/base/Integer.h"
#include "openeaagles/base/Float.h"
#include "openeaagles/base/Boolean.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/List.h"
#include "EdlScanner.h"

static oe::base::Object* result;               // result of all our work
static oe::base::EdlScanner* scanner;          // edl scanner
static oe::base::FactoryFunc factoryFunc;      // factory function 
static int errCount;                           // error count

//------------------------------------------------------------------------------
// yylex() -- user defined; used by the parser to call the lexical generator
//------------------------------------------------------------------------------
inline int yylex()
{
   return scanner->yylex();
}

//------------------------------------------------------------------------------
// yyerror() -- user defined; use by the parser to report errors.
//------------------------------------------------------------------------------
inline void yyerror(const char* s)
{
   std::cerr << scanner->getFilename() << ", line ";
   std::cerr << scanner->getLineNumber() << ": ";
   std::cerr << s << std::endl;
   errCount++;
}

//------------------------------------------------------------------------------
// gufParse() -- returns an object with factory 'name' with its slots set to
//               values in 'argList'
//------------------------------------------------------------------------------
static oe::base::Object* gufParse(const char* name, oe::base::PairStream* argList)
{
    char emsg[256];
    oe::base::Object* obj = nullptr;

    if (factoryFunc != nullptr) {

        // when we have a factory name, use factoryFunc() to construct an
        // object of the object's class type.
        obj = factoryFunc(name);

       // set slots in our new object
       if (obj != nullptr && argList != nullptr) {
          oe::base::List::Item* item = argList->getFirstItem();
          while (item != nullptr) {
               oe::base::Pair* p = static_cast<oe::base::Pair*>(item->getValue());
               bool ok = obj->setSlotByName(*p->slot(), p->object());
               if (!ok) {
                  oe::base::utStrcpy(emsg, sizeof(emsg), "error while setting slot name: ");
                  oe::base::utStrcat(emsg, sizeof(emsg), *p->slot());
                  yyerror(emsg);
               }
               item = item->getNext();
          }
          bool ok = obj->isValid();
          if (!ok) {
             oe::base::utStrcpy(emsg, sizeof(emsg), "error: invalid object: ");
             oe::base::utStrcat(emsg, sizeof(emsg), name);
             yyerror(emsg);
          }
       }
       else if (obj == nullptr) {
          oe::base::utStrcpy(emsg, sizeof(emsg), "undefined factory name: ");
          oe::base::utStrcat(emsg, sizeof(emsg), name);
          yyerror(emsg);
       }

    }
    return obj;
}

%}

// Defines types that our values can be, yylval.
%union {
   double                     dval;
   long                       lval;
   bool                       bval;
   char*                      cvalp;
   oe::base::Object*          ovalp;
   oe::base::Pair*            pvalp;
   oe::base::PairStream*      svalp;
   oe::base::List*            lvalp;
   oe::base::Number*          nvalp;
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
        | SLOT_ID form              { if ($2 != 0) { result = new oe::base::Pair($1, $2); delete[] $1; $2->unref(); } }
        ;

arglist :                           { $$ = new oe::base::PairStream(); }

        | arglist form              { if ($2 != 0) {
                                        int i = $1->entries();
                                        char cbuf[20];
                                        std::sprintf(cbuf,"%i",i+1);
                                        oe::base::Pair* p = new oe::base::Pair(cbuf, $2);
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
                                    oe::base::Pair* p = new oe::base::Pair(cbuf, $2);
                                    $2->unref();
                                    $1->put(p);
                                    p->unref();
                                    $$ = $1;
                                    }

        | arglist slot_value        { $1->put($2); $2->unref(); $$ = $1; }
        ;


form    : '(' IDENT arglist ')'     { $$ = gufParse($2, $3); delete[] $2; $3->unref(); }

        | '{' arglist '}'           { $$ = (oe::base::Object*) $2; }
        ;


slot_value  : SLOT_ID prim          { $$ = new oe::base::Pair($1, $2); delete[] $1; $2->unref(); }
        | SLOT_ID form              { $$ = new oe::base::Pair($1, $2); delete[] $1; $2->unref(); }
        ;

prim    : STRING_LITERAL            { $$ = new oe::base::String($1); delete[] $1; }
        | IDENT                     { $$ = new oe::base::Identifier($1); delete[] $1; }
        | BOOLconstant              { $$ = new oe::base::Boolean($1); }
        | '[' numlist ']'           { $$ = $2; }
        | number                    { $$ = $1; }
        ;

numlist : number                    { $$ = new oe::base::List(); $$->put($1); $1->unref(); }
        | numlist number            { $$ = $1; $$->put($2); $2->unref(); }
        ;

number  : INTEGERconstant       { $$ = new oe::base::Integer($1); }
        | FLOATINGconstant      { $$ = new oe::base::Float($1); }
        ;
%%

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Returns an Object* that was constructed from parsing an EDL file.
// factory is the name of the Object creation function  
//------------------------------------------------------------------------------
Object* edlParser(const char* filename, FactoryFunc factory, int* numErrors)
{
    factoryFunc = factory;
    result = 0;
    errCount = 0;

    // Open the file (someone else passed it through the preprocessor)
    std::fstream fin;
    fin.open(filename,std::ios::in);
    scanner = new EdlScanner(&fin);

    //yydebug = 1;
    Object* q = 0;
    if (yyparse() == 0) q = result;

    fin.close();
    delete scanner;

    if (numErrors != 0) *numErrors = errCount;
    return q;
}

}
}

