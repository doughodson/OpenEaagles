//------------------------------------------------------------------------------
// Class: String
//------------------------------------------------------------------------------
#ifndef __oe_base_String_H__
#define __oe_base_String_H__

#include "openeaagles/base/Object.h"
#include "openeaagles/base/util/str_utils.h"

#include <cstdlib>
#include <cctype>
#include <cstring>

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: String
// Description: Character string class
//
//      Assignment Operators:     =
//      Concatenation Operators:  +  +=
//      Comparison Operators:     <  <=  ==  >=  >  !=
//      iostream Operations:      <<  >>
//
// Factory name: String
//------------------------------------------------------------------------------
class String : public Object {
    DECLARE_SUBCLASS(String,Object)

public:
    enum Justify { NONE, LEFT, RIGHT, CENTER };
    static const unsigned int MAX_STRING_LENGTH = 512; /* used only by setString() */

public:
    String(const char* string);
    String(const char* s1, const char* s2);
    String();

    operator char*();
    operator const char*() const;
    String& operator=(const char*);
    void operator+=(const char*);

    size_t len() const;                            // Returns the length of this string.
    bool isEmpty() const;                          // Returns true if this string is empty (null).

    const char* getString() const;                 // Returns a pointer to the text string.
    char* getCopyString() const;                   // Returns a copy of the text string.
    char getChar(const unsigned int index) const;  // Gets a specific character

    virtual void empty();                          // Empty (null) this string.
    virtual void setStr(const char* string);       // Sets us to a copy of 'string'
    virtual void catStr(const char* string);       // Append a copy of 'string' to our text string
    virtual void setChar(const unsigned int index, const char c);    // Sets a specific character

    // Returns true if this string is the ascii form of a number
    bool isNumber() const;

    // Returns the value of this string converted to a number or zero if isNumber() is false.
    double getNumber() const;

    // Returns true if this string is the ascii form of an integer number.
    bool isInteger() const;

    // Returns the value of this string converted to an integer value or zero if isInteger() is false.
    int getInteger() const;

    // Returns in "subStr" a "numChars" character sub-string of 'this' string that starts a "startIndex"
    bool getSubString(String& subStr, const unsigned int startIndex, const size_t numChars) const;

    // Set to the first 'w' characters of 'str'.
    // If the length of 'str' is less than 'w', than justify as 'j'.
    virtual void setString(const String& str, const size_t w, const Justify j = NONE);

private:
    char* str;      // the character string
    size_t n;       // length of this string
    size_t nn;      // length of the memory allocated for this string
};

//------------------------------------------------------------------------------
// String append operators: + and +=
//------------------------------------------------------------------------------
inline void String::operator+=(const char* s)
{
    catStr(s);
}

inline String operator+(const String& s1, const String& s2)
{
    return String(s1, s2);
}

inline String operator+(const char* s1, const String& s2)
{
    return String(s1, s2);
}

inline String operator+(const String& s1, const char* s2)
{
    return String(s1, s2);
}


//------------------------------------------------------------------------------
// conversion function -- returns a const pointer to the text string.
//------------------------------------------------------------------------------
inline String::operator char*()
{
    return str;
}

inline String::operator const char*() const
{
    return str;
}


//------------------------------------------------------------------------------
// len() -- returns the length of this string.
//------------------------------------------------------------------------------
inline size_t String::len() const
{
    return n;
}


//------------------------------------------------------------------------------
// isEmpty() -- returns true if this is an empty string.
//------------------------------------------------------------------------------
inline bool String::isEmpty() const
{
    return len() == 0;
}


//------------------------------------------------------------------------------
// getChar() -- returns a character from the string.
//------------------------------------------------------------------------------
inline char String::getChar(const unsigned int index) const
{
   if (str != nullptr && index <= len())
      return str[index];
   else
      return '\0';
}


//------------------------------------------------------------------------------
// getString() -- returns a const pointer of the string
//------------------------------------------------------------------------------
inline const char* String::getString() const
{
    return str;
}


//------------------------------------------------------------------------------
// getCopyString() -- returns a copy of the string
//------------------------------------------------------------------------------
inline char* String::getCopyString() const
{
    char* p = 0;
    size_t l = len();
    if (l > 0) {
        p = new char[l+1];
        lcStrcpy(p,(l+1),str);
    }
    return p;
}

//------------------------------------------------------------------------------
// isInteger() -- is this an a ascii integer
//------------------------------------------------------------------------------
inline bool String::isInteger() const
{
    if (isEmpty()) return false;
    for (int i = 0; str[i] != '\0'; i++) {
        if ( !std::isdigit(str[i]) ) return false;
    }
    return true;
}


//------------------------------------------------------------------------------
// isNumber() -- is this an a ascii number
//------------------------------------------------------------------------------
inline bool String::isNumber() const
{
    if (isEmpty()) return false;
    for (int i = 0; str[i] != '\0'; i++) {
        if ( !std::isdigit(str[i]) && str[i] != '.' ) return false;
    }
    return true;
}


//------------------------------------------------------------------------------
// getInteger() -- convert a ascii number string to an integer
//------------------------------------------------------------------------------
inline int String::getInteger() const
{
    if (isInteger()) return std::atoi(str);
    else return 0;
}


//------------------------------------------------------------------------------
// getNumber() -- convert a ascii number string to a number
//------------------------------------------------------------------------------
inline double String::getNumber() const
{
    if (isNumber()) return  std::atof(str);
    else return 0.0;
}

//------------------------------------------------------------------------------
// Comparison operators: ==, !=, <, <=, >, and >=
//------------------------------------------------------------------------------
inline bool operator==(const String& s1, const String& s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then both must be null to be equal
      if (s1.isEmpty() && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) == 0);
   return ok;
}

inline bool operator==(const char* s1, const String& s2)
{
   bool ok = false;
   if (s1 == 0 || s2.isEmpty()) {
      // if a string is null, then both must be null to be equal
      if (s1 == 0 && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) == 0);
   return ok;
}

inline bool operator==(const String& s1, const char* s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2 == 0) {
      // if a string is null, then both must be null to be equal
      if (s1.isEmpty() && s2 == 0) ok = true;
   }
   else ok = (std::strcmp(s1, s2) == 0);
   return ok;
}

inline bool operator!=(const String& s1, const String& s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then only one must be null to be not equal
      if (!s1.isEmpty() || !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) != 0);
   return ok;
}

inline bool operator!=(const char* s1, const String& s2)
{
   bool ok = false;
   if (s1 == 0 || s2.isEmpty()) {
      // if a string is null, then only one must be null to be not equal
      if (s1 != 0 || !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) != 0);
   return ok;
}

inline bool operator!=(const String& s1, const char* s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2 == 0) {
      // if a string is null, then only one must be null to be not equal
      if (!s1.isEmpty() || s2 != 0) ok = true;
   }
   else ok = (std::strcmp(s1, s2) != 0);
   return ok;
}

inline bool operator<(const String& s1, const String& s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then only s1 must be null to be less than s2
      if (s1.isEmpty() && !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) < 0);
   return ok;
}

inline bool operator<(const char* s1, const String& s2)
{
   bool ok = false;
   if (s1 == 0 || s2.isEmpty()) {
      // if a string is null, then only s1 must be null to be less than s2
      if (s1 == 0 && !s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) < 0);
   return ok;
}

inline bool operator<(const String& s1, const char* s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2 == 0) {
      // if a string is null, then only s1 must be null to be less than s2
      if (s1.isEmpty() && s2 != 0) ok = true;
   }
   else ok = (std::strcmp(s1, s2) < 0);
   return ok;
}

inline bool operator<=(const String& s1, const String& s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then s1 must be null to be less than or equal s2.
      if (s1.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) <= 0);
   return ok;
}

inline bool operator<=(const char* s1, const String& s2)
{
   bool ok = false;
   if (s1 == 0 || s2.isEmpty()) {
      // if a string is null, then s1 must be null to be less than or equal s2.
      if (s1 == 0) ok = true;
   }
   else ok = (std::strcmp(s1, s2) <= 0);
   return ok;
}

inline bool operator<=(const String& s1, const char* s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2 == 0) {
      // if a string is null, then s1 must be null to be less than or equal s2.
      if (s1.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) <= 0);
   return ok;
}

inline bool operator>(const String& s1, const String& s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then only s2 must be null to be less than s1
      if (!s1.isEmpty() && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) > 0);
   return ok;
}

inline bool operator>(const char* s1, const String& s2)
{
   bool ok = false;
   if (s1 == 0 || s2.isEmpty()) {
      // if a string is null, then only s2 must be null to be less than s1
      if (s1 != 0 && s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) > 0);
   return ok;
}

inline bool operator>(const String& s1, const char* s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2 == 0) {
      // if a string is null, then only s2 must be null to be less than s1
      if (!s1.isEmpty() && s2 == 0) ok = true;
   }
   else ok = (std::strcmp(s1, s2) > 0);
   return ok;
}

inline bool operator>=(const String& s1, const String& s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2.isEmpty()) {
      // if a string is null, then s2 must be null to be less than or equal s1
      if (s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) >= 0);
   return ok;
}

inline bool operator>=(const char* s1, const String& s2)
{
   bool ok = false;
   if (s1 == 0 || s2.isEmpty()) {
      // if a string is null, then s2 must be null to be less than or equal s1
      if (s2.isEmpty()) ok = true;
   }
   else ok = (std::strcmp(s1, s2) >= 0);
   return ok;
}

inline bool operator>=(const String& s1, const char* s2)
{
   bool ok = false;
   if (s1.isEmpty() || s2 == 0) {
      // if a string is null, then s2 must be null to be less than or equal s1
      if (s2 == 0) ok = true;
   }
   else ok = (std::strcmp(s1, s2) >= 0);
   return ok;
}

//------------------------------------------------------------------------------
// output stream operator <<
//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& sout, const String& s)
{
    sout << "\"";
    if (!s.isEmpty()) {
        const char* str = s;
        sout << str;
    }
    sout << "\"";
    return sout;
}

//------------------------------------------------------------------------------
// input stream operator >>
//------------------------------------------------------------------------------
inline std::istream& operator>>(std::istream& sin, String& s)
{
    const int a = 256;
    char* buf = new char[a+1];
    int   n1   = 0;
    char  c;

    sin.get(c);
    while ( (c != '\n') && !sin.eof() && n1 < a) {
        buf[n1++] = c;
        sin.get(c);
    }

    buf[n1] = '\0';

    s.setStr(buf);

    delete[] buf;

    return sin;
}

} // End base namespace
} // End oe namespace

#endif
