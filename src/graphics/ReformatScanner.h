//
// Scanner for the Readout reformatter
//

#ifndef __oe_graphics_ReformatScanner_H__
#define __oe_graphics_ReformatScanner_H__

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
class ReformatScanner : public rfFlexLexer
{
public:
   enum DataType { invalid, number, octal, hex, time, dir };

   ReformatScanner() : rfFlexLexer(), dataType(invalid), postSign(false) { }
   virtual ~ReformatScanner() { }

   const char* getFormat() const { return dataType != invalid ? format : nullptr; }

   DataType convertNumber(const char* s);
   DataType convertOctal(const char* s);
   DataType convertHex(const char* s);
   TimeReadout::TimeMode convertTime(const char* s);
   DirectionReadout::DirMode convertDirection(const char* s);

   bool isPostSign() { return postSign; }

protected:
   virtual int yylex();
   virtual int yylex(const DataType dt);

   int processInteger(const char* text, const int len);
   int processFloat(const char* text, const int len);
   int processTime(const TimeReadout::TimeMode tm, const char* text, const int len);
   int processDirection(const DirectionReadout::DirMode dm, const char* text, const int len);

   int formatError(const char* text);

private:
   DataType dataType;

   char format[256];
   bool  postSign;
};

}
}

#endif
