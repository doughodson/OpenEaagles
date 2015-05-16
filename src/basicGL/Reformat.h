//
// Lexical generator for the Readout reformatter
//

#ifndef _Eaagles_BasicGL_Reformat_H_
#define _Eaagles_BasicGL_Reformat_H_

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer rfFlexLexer
#include "../basic/FlexLexer.h"
#endif

#include "openeaagles/basicGL/Readouts.h"
#include <sstream>

namespace Eaagles {
namespace BasicGL {

// Reformat converts example strings into C/C++ format specifiers for
// sprintf, printf, etc.  These examples will be later used as templates
// for data entry.
class Reformat : public rfFlexLexer
{
public:
   enum DataType { invalid, number, octal, hex, time, dir };

   Reformat() : rfFlexLexer(), dataType(invalid), postSign(false) { }
   virtual ~Reformat() { }

   const char* getFormat() const { return dataType != invalid ? format : nullptr; }

   Reformat::DataType convertNumber(const char* s);
   Reformat::DataType convertOctal(const char* s);
   Reformat::DataType convertHex(const char* s);
   TimeReadout::TimeMode   convertTime(const char* s);
   DirectionReadout::DirMode     convertDirection(const char* s);

   bool isPostSign() { return postSign; }

protected:
   virtual int yylex();
   virtual int yylex(const DataType dt);

   int processInteger(const char* text, const int len);
   int processFloat(const char* text, const int len);
   int processTime(const TimeReadout::TimeMode tm, const char* text, const int len);
   int processDirection(const DirectionReadout::DirMode dm, const char* text, const int len);

   int formatError(const char* text) const;

private:
   DataType dataType;

   char format[256];
   bool  postSign;
};

} // End BasicGL namespace
} // End Eaagles namespace

#endif

