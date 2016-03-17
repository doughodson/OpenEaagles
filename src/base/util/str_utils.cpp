
#include "openeaagles/base/util/str_utils.h"

#include <cstring>
#include <cctype>
#include <cstddef>

namespace oe {
namespace base {

//------------
// Converts the integer number, 'num', to an ascii string.
//------------
void utInteger2Str(const int num, char* const str, int width)
{
   const int MAX_CHARS = 32;

   // check sign
   const bool isNeg = (num < 0);

   // Abs value of the number
   int tnum = num;
   if (isNeg) tnum = -num;

   // Convert to a string (reverse order)
   char temp[MAX_CHARS];
   int nc = 0;
   do {
      temp[nc++] = char(tnum % 10 + static_cast<int>('0'));
      tnum = tnum / 10;
   } while( tnum > 0 && nc < MAX_CHARS);

   // Add the negative character (if needed)
   if(isNeg && nc < MAX_CHARS) {
      temp[nc++] = '-';
   }

   // pad with blanks
   int nBlanks = (width - nc);
   if (nBlanks < 0) nBlanks = 0;
   for (int i = 0; i < nBlanks; i++) {
      str[i] = ' ';
   }

   // move chars reversed
   for (int i = nBlanks; i < width; i++) {
      str[i] = temp[--nc];
   }
   str[width] = '\0';
}

//------------
// String copy function: Copies a string from the source, 'strSource', to the
// destination buffer, 'strDest'.  The destination string is null terminated.
// The 'strSource' string will be truncated if the 'strDest' string is too small.
//------------
bool utStrcpy(char* const strDest, const size_t sizeOfDest, const char* const strSource)
{
   if ((strDest == nullptr) || (strSource == nullptr) || sizeOfDest == 0) { // NULL ptr's or zero dest size
      return false;
   }

   // Max characters to copy
   const size_t maxToCopy = sizeOfDest - 1;

   // Set our working pointers
   char* q = strDest;
   const char* p = strSource;

   // Everything looks good, copy...
   size_t idx = 0;
   while (idx++ < maxToCopy && (*q++ = *p++) != '\0') {}
   *q = '\0'; // null terminate

   return true;   // Good result
}

//------------
// String N copy function: Copies up to 'count' characters from the source string,
// 'strSource', to the destination buffer, 'strDest'.  The new destination string
// is null terminated.  The 'strSource' string will be truncated if the 'strDest'
// string is too small.
//------------
bool utStrncpy(char* const strDest, const size_t sizeOfDest, const char* const strSource, const size_t count)
{
   if ((strDest == nullptr) || (strSource == nullptr) || sizeOfDest == 0) { // NULL ptr's or zero dest size
      return false;
   }

   // Max characters to copy
   size_t maxToCopy = sizeOfDest - 1;
   if (count < maxToCopy) maxToCopy = count;

   // Set our working pointers
   char* q = strDest;
   const char* p = strSource;

   // Everything looks good, copy...
   size_t idx = 0;
   while (idx++ < maxToCopy && (*q++ = *p++) != '\0') {}
   *q = '\0'; // null terminate

   return true;   // Good result
}

//------------
// String cat function: Appends the 'strSource' string to the end of the 'strDest'
// string.  The new destination string is null terminated.  The 'strSource' string
// will be truncated if the 'strDest' string is too small.  If 'strDest' is a null
// pointer, or is not null-terminated, or if 'strSource' is a null pointer then false
// is returned and the destination buffer is unchanged.
//------------
bool utStrcat(char* const strDest, const size_t sizeOfDest, const char* const strSource)
{
   if ((strDest == nullptr) || (strSource == nullptr) || sizeOfDest == 0) { // NULL ptr's or zero dest size
      return false;
   }

   // Set our working pointers
   char* q = strDest;
   const char* p = strSource;

   // Max characters to the destination buffer
   const size_t maxToCopy = sizeOfDest - 1;

   // Forward to the end of the current destination string
   size_t idx = 0;
   while (*q++ != '\0' && idx++ < maxToCopy) {}

   // if we have a terminated string ...
   bool ok = (idx <= maxToCopy);
   if (ok) {
      // Back space for the original string's null character
      --q;

      // Everything looks good, copy...
      while (idx++ < maxToCopy && (*q++ = *p++) != '\0') {}
      *q = '\0'; // null terminate
   }

   return ok;
}

//------------
// Full string copy function: Copies a string from the source, 'strSource', to the
// destination buffer, 'strDest'.  The new destination string is null terminated.
// If the destination buffer is too small then false is returned and the destination buffer
// is unchanged.
//------------
bool utStrcpyFull(char* const strDest, const size_t sizeOfDest, const char* const strSource)
{
   if ((strDest == nullptr) || (strSource == nullptr) || sizeOfDest == 0) { // NULL ptr's or zero dest size
      return false;
   }

   // do we have space for the full string plus termination?
   const size_t srcSize = std::strlen(strSource);
   if (sizeOfDest < (srcSize+1)) return false;

   // Max characters to copy
   const size_t maxToCopy = sizeOfDest - 1;

   // Set our working pointers
   char* q = strDest;
   const char* p = strSource;

   // Everything looks good, copy...
   size_t idx = 0;
   while (idx++ < maxToCopy && (*q++ = *p++) != '\0') {}
   *q = '\0'; // null terminate

   return true;   // Good result
}

//------------
// Full string N copy function: Copies up to 'count' characters from the source string,
// 'strSource', to the destination buffer, 'strDest'.  The new destination string is null
// terminated.   If the destination buffer is too small then false is returned and the
// destination buffer is unchanged.
//------------
bool utStrncpyFull(char* const strDest, const size_t sizeOfDest, const char* const strSource, const size_t count)
{
   if ((strDest == nullptr) || (strSource == nullptr) || sizeOfDest == 0) { // NULL ptr's or zero dest size
      return false;
   }

   // do we have space for the full character count plus termination?
   if (sizeOfDest < (count+1)) return false;

   // Max characters to copy
   size_t maxToCopy = sizeOfDest - 1;
   if (count < maxToCopy) maxToCopy = count;

   // Set our working pointers
   char* q = strDest;
   const char* p = strSource;

   // Everything looks good, copy...
   size_t idx = 0;
   while (idx++ < maxToCopy && (*q++ = *p++) != '\0') {}
   *q = '\0'; // null terminate

   return true;   // Good result
}

//------------
// Full string cat function: Appends the source string, 'strSource', to the end of the
// destination string, 'strDest'.  The new destination string is null terminated.  If
// the destination buffer is too small, or if 'strDest' is a null pointer, or is not
// null-terminated, or if 'strSource' is a null pointer then false is returned and the
// destination buffer is unchanged.
//------------
bool utStrcatFull(char* const strDest, const size_t sizeOfDest, const char* const strSource)
{
   if ((strDest == nullptr) || (strSource == nullptr) || sizeOfDest == 0) { // NULL ptr's or zero dest size
      return false;
   }

   // Set our working pointers
   char* q = strDest;
   const char* p = strSource;

   // Max characters to the destination buffer
   const size_t maxToCopy = sizeOfDest - 1;

   // Forward to the end of the current destination string
   size_t idx = 0;
   while (*q++ != '\0' && idx++ < maxToCopy) {}

   // do we have space for the full string plus termination?
   const size_t srcSize = std::strlen(strSource);

   // if we have room for both strings ...
   bool ok = ((idx+srcSize) <= maxToCopy);
   if (ok) {

      // Back space for the original string's null character
      --q;

      // Everything looks good, copy...
      while (idx++ < maxToCopy && (*q++ = *p++) != '\0') {}
      *q = '\0'; // null terminate
   }

   return ok;
}

//------------
// Compare two strings ignoring case (using lower case characters)
//------------
int utStrcasecmp(const char* const s1, const char* const s2)
{
   if (s1 == nullptr && s2 == nullptr) return  0;   // They're the same if they both don't exist
   if (s1 != nullptr && s2 == nullptr) return  1;  // S1 is greater than S2 if S2 doesn't exist and S1 does
   if (s1 == nullptr && s2 != nullptr) return -1;  // S1 is less than S2 if S1 doesn't exist and S2 does

   const char* p1 = s1;
   const char* p2 = s2;
   while (*p1 != '\0' && *p2 != '\0') {
      int c1 = std::tolower(static_cast<int>(*p1++));
      int c2 = std::tolower(static_cast<int>(*p2++));
      if (c1 < c2) return -1;
      if (c1 > c2) return  1;
   }

   if (*p1 != '\0') return  1;
   if (*p2 != '\0') return -1;
   return 0;
}

//------------
// Compare the first 'n' characters of two strings ignoring case (using lower case characters)
//------------
int utStrncasecmp(const char* const s1, const char* const s2, const size_t n)
{
   if ( (s1 == nullptr && s2 == nullptr) || n == 0) return 0;  // They're the same if they both don't exist
   if (s1 != nullptr && s2 == nullptr) return  1;  // S1 is greater than S2 if S2 doesn't exist and S1 does
   if (s1 == nullptr && s2 != nullptr) return -1;  // S1 is less than S2 if S1 doesn't exist and S2 does

   const char* p1 = s1;
   const char* p2 = s2;
   size_t icnt = 0;
   while (*p1 != '\0' && *p2 != '\0' && icnt++ < n) {
      int c1 = std::tolower(static_cast<int>(*p1++));
      int c2 = std::tolower(static_cast<int>(*p2++));
      if (c1 < c2) return -1;
      if (c1 > c2) return  1;
   }

   if (icnt < n && *p1 != '\0') return  1;
   if (icnt < n && *p2 != '\0') return - 1;
   return 0;
}

}
}

