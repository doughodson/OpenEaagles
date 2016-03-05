#include "openeaagles/base/String.h"

#include <cstdlib>
#include <cstring>

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(String, "String")
EMPTY_SLOTTABLE(String)

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
String::String()
{
   STANDARD_CONSTRUCTOR()

   str = nullptr;
   n = 0;
   nn = 0;
}

String::String(const char* s)
{
   STANDARD_CONSTRUCTOR()

   str = nullptr;
   n = 0;
   nn = 0;
   setStr(s);
}

String::String(const char* s1, const char* s2)
{
   STANDARD_CONSTRUCTOR()

   str = nullptr;
   n = 0;
   nn = 0;
   setStr(s1);
   catStr(s2);
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void String::copyData(const String& org, const bool cc)
{
   BaseClass::copyData(org);
   if (!cc && str != nullptr) delete[] str;
   str = nullptr;
   nn = 0;
   n = 0;
   setStr(org);
}


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void String::deleteData()
{
   if (str != nullptr) delete[] str;
   str = nullptr;
   nn = 0;
   n = 0;
}


//------------------------------------------------------------------------------
// setString() --
//------------------------------------------------------------------------------
void String::setString(const String& origStr, const size_t w, const Justify j)
{
   char sbuf[MAX_STRING_LENGTH+1];  // Source buffer
   char dbuf[MAX_STRING_LENGTH+1];  // Destination buffer
   const char* ss = sbuf;           // Pointer to source buffer


   // ---
   // when w is zero or origStr is empty
   // ---

   if (w == 0 || origStr.len() == 0) {
      dbuf[0] = '\0';
      *this = dbuf;
      return;
   }

   // ---
   // When justified (left, right, center), make the copy without leading
   // or trailing spaces.
   // ---

   if (j != NONE) {
      // Justified:  copy without leading or trailing spaces
      const char* p = origStr;
      char* q = sbuf;
      while (*p != '\0' && *p == ' ') { p++; }
      while (*p != '\0' && q <= &sbuf[MAX_STRING_LENGTH-1]) { *q++ = *p++; }
      *q-- = '\0';
      while (*q == ' ' && q >= sbuf) { *q-- = ' '; }
   }
   else {
      // Not justified:  change our source buffer pointer to the orig string
      ss = origStr;
   }


   // ---
   // Set this to the new string justified
   // ---

   int i1 = 0;                                         // Source index
   int i2 = 0;                                         // Destination index
   int l1 = static_cast<int>(std::strlen(ss));         // Source string length
   if (l1 > MAX_STRING_LENGTH) l1 = MAX_STRING_LENGTH;
   int l2 = static_cast<int>(w);                       // Destination string length
   if (l2 > MAX_STRING_LENGTH) l2 = MAX_STRING_LENGTH;
   int d = l2 - l1;                                    // Difference in lengths

   switch (j) {

      // NONE or LEFT justified
      case NONE :
      case LEFT :
      {
         if (d < 0) l1 += d;
         while (i1 < l1) { dbuf[i2++] = ss[i1++]; }
         while (i2 < l2) { dbuf[i2++] = ' '; }
      }
      break;

      // RIGHT justified
      case RIGHT : {
         if (d < 0) i1 = -d;
         while (i2 < d)  { dbuf[i2++] = ' '; }
         while (i1 < l1) { dbuf[i2++] = ss[i1++]; }
      }
      break;

      // CENTER justified
      case CENTER : {
         int n1 = d/2;
         if (d < 0) { i1 = -n1; l1 += (d-n1); }
         while (i2 < n1) { dbuf[i2++] = ' '; }
         while (i1 < l1) { dbuf[i2++] = ss[i1++]; }
         while (i2 < l2) { dbuf[i2++] = ' '; }
      }
      break;
   }

   dbuf[i2] = '\0';
   *this = dbuf;
}

//------------------------------------------------------------------------------
// getSubString() -- Returns in "subStr" a "numChars" character sub-string
//                   of 'this' string that starts a "startIndex"
//------------------------------------------------------------------------------
bool String::getSubString(String& subStr, const unsigned int startIndex, const size_t numChars) const
{
   if(startIndex > (n - 1))
      return false;

   size_t maxChars = 0;
   if((startIndex + numChars) > n)
      maxChars = (n - startIndex);
   else
      maxChars = numChars;

   char* subString = new char[maxChars + 1];
   lcStrncpy(subString, (maxChars + 1), &str[startIndex], maxChars);
   subString[maxChars] = '\0';
   subStr.setStr(subString);
   delete[] subString;

   return true;
}


//------------------------------------------------------------------------------
// setStr() -- sets this string to a copy of 'string'
//------------------------------------------------------------------------------
void String::setStr(const char* string)
{
   // copy the new text string
   if (string != nullptr) {
      size_t l = std::strlen(string);
      if (l >= nn || str == nullptr) {
         if (str != nullptr) delete[] str;
         nn = (l+1);
         str = new char[nn];
      }
      lcStrcpy(str,nn,string);
      n = l;
   }
   else {
      // remove the old text string
      n = 0;
   }
}


//------------------------------------------------------------------------------
// catStr() -- appends a copy of 's' to the end of this string.
//------------------------------------------------------------------------------
void String::catStr(const char* s)
{
   // early out if nothing to append
   if (s == nullptr) return;

   // if this string was empty then we're really just setStr()
   if ( isEmpty() ) {
      setStr(s);
      return;
   }

   // Have new text to append to the original text
   size_t l = n + std::strlen(s);
   if (l >= nn) {
      char* t = str;
      nn = (l+1);
      str = new char[nn];
      lcStrcpy(str,nn,t);
      delete[] t;
   }
   lcStrcat(str,nn,s);
   n = l;
}


//------------------------------------------------------------------------------
// setChar() -- Sets a specific character
//------------------------------------------------------------------------------
void String::setChar(const unsigned int index, const char c)
{
   if (index <= len())
      str[index] = c;
}


//------------------------------------------------------------------------------
// empty() -- make this an empty string
//------------------------------------------------------------------------------
void String::empty()
{
   if (str != nullptr) str[0] = 0;
   n = 0;
}


//------------------------------------------------------------------------------
// Assignment operator =
//------------------------------------------------------------------------------
String& String::operator=(const char* s)
{
   setStr(s);
   return *this;
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& String::serialize(std::ostream& sout, const int, const bool) const
{
   sout << "\"";
   if (str != nullptr) sout << str;
   sout << "\"";
   return sout;
}

} // End base namespace
} // End oe namespace
