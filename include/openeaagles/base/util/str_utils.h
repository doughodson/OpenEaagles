
#ifndef __oe_base_util_str_utils_H__
#define __oe_base_util_str_utils_H__

//------------------------------------------------------------------------------
// General purpose functions to manipulate c-style strings
//------------------------------------------------------------------------------

#include <cstddef>

namespace oe {
namespace base {

// Converts the integer number, 'num', to a 'width' character ascii string
// (this size of 'str' must be at least width+1 bytes to include the null termination)
void utInteger2Str(const int num, char* const str, int width);

// String copy function: Copies a string from the source, 'strSource', to the
// destination buffer, 'strDest'.  The destination string is null terminated.
// The 'strSource' string will be truncated if the 'strDest' string is too small.
bool utStrcpy(char* const strDest, const size_t sizeOfDest, const char* const strSource);

// String N copy function: Copies up to 'count' characters from the source string,
// 'strSource', to the destination buffer, 'strDest'.  The new destination string
// is null terminated.  The 'strSource' string will be truncated if the 'strDest'
// string is too small.
bool utStrncpy(char* const strDest, const size_t sizeOfDest, const char* const strSource, const size_t count);

// String cat function: Appends the 'strSource' string to the end of the 'strDest'
// string.  The new destination string is null terminated.  The 'strSource' string
// will be truncated if the 'strDest' string is too small.  If 'strDest' is a null
// pointer, or is not null-terminated, or if 'strSource' is a null pointer then false
// is returned and the destination buffer is unchanged.
bool utStrcat(char* const strDest, const size_t sizeOfDest, const char* const strSource);

// Full string copy function: Copies a string from the source, 'strSource', to the
// destination buffer, 'strDest'.  The new destination string is null terminated.
// If the destination buffer is too small then false is returned and the destination buffer
// is unchanged.
bool utStrcpyFull(char* const strDest, const size_t sizeOfDest, const char* const strSource);

// Full string N copy function: Copies up to 'count' characters from the source string,
// 'strSource', to the destination buffer, 'strDest'.  The new destination string is null
// terminated.   If the destination buffer is too small then false is returned and the
// destination buffer is unchanged.
bool utStrncpyFull(char* const strDest, const size_t sizeOfDest, const char* const strSource, const size_t count);

// Full string cat function: Appends the source string, 'strSource', to the end of the
// destination string, 'strDest'.  The new destination string is null terminated.  If
// the destination buffer is too small, or if 'strDest' is a null pointer, or is not
// null-terminated, or if 'strSource' is a null pointer then false is returned and the
// destination buffer is unchanged.
bool utStrcatFull(char* const strDest, const size_t sizeOfDest, const char* const strSource);

// Compare two strings ignoring case --
// Returns an integer less than, equal to or greater than zero if 's1' is less
// than, equal to or greater than 's2', respectively.
// (using lower case characters)
int utStrcasecmp(const char* const s1, const char* const s2);

// Compare the first 'n' characters of two strings ignoring case --
// Returns an integer less than, equal to or greater than zero if 's1' is less
// than, equal to or greater than 's2', respectively.
// (using lower case characters)
int utStrncasecmp(const char* const s1, const char* const s2, const size_t n);

}
}

#endif

