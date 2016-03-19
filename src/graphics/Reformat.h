//
// Scanner for the Readout reformatter
//

#ifndef __oe_graphics_Reformat_H__
#define __oe_graphics_Reformat_H__

#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer rfFlexLexer
#include "../base/platform/FlexLexer.h"
#endif

#include "openeaagles/graphics/Readouts.h"
#include <sstream>

namespace oe {
namespace graphics {

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

}
}

#endif

