//------------------------------------------------------------------------------
// Support functions
//------------------------------------------------------------------------------

#include "openeaagles/basic/support.h"
#include <fstream>
#include <cstring>
#include <cctype>

//------------------------------------------------------------------------------
// Window/Linux specific code
//------------------------------------------------------------------------------
#if defined(WIN32)
  #if defined(__MINGW32__)
    // MinGW support functions
    #include "windows/mingwSupport.cxx"
  #else
    // Visual Studio support functions
    #include "windows/vsSupport.cxx"
  #endif
#else
  // Linux GCC support functions
  #include "linux/support.cxx"
#endif

namespace Eaagles {

//------------
// UTC time of day (seconds since midnight) and optional date (day/month/year)
//------------
double getTimeUtc(unsigned int* const day, unsigned int* const month, unsigned int* const year)
{
   unsigned long seconds = 0;
   unsigned long uSec = 0;
   getTime(&seconds, &uSec);

   unsigned int hour = 0;
   unsigned int min = 0;
   unsigned int sec = 0;
   convertSec2Ymdhms(seconds, year, month, day, &hour, &min, &sec);

   // Return time since midnight (sec)
   return ( hour * 3600.0 + min * 60.0 + sec + static_cast<double>(uSec) / 1000000.0 );
}

//------------
// Get UTC time since midnight (00:00:00), January 1, 1970
//------------
double getTime()
{
   unsigned long sec(0), uSec(0);
   getTime(&sec, &uSec);
   return static_cast<double>(sec) + static_cast<double>(uSec)/1000000.0;
}

// Convert seconds since midnight (00:00:00), January 1, 1970 to NTP time stamp
bool convertSec2Ntp(
      const unsigned long seconds,  // (IN) whole seconds since midnight (00:00:00), January 1, 1970
      const unsigned long uSec,     // (IN) microseconds seconds
      unsigned long* const ntpSec,  // (OUT) NTP seconds
      unsigned long* const ntpSecF  // (OUT) NTP fraction of seconds
   )
{
   static const double P32 = 4294967296.0; // 2^32

   // Adjust by 2208988800 seconds (1900 to 1970)
   if (ntpSec != nullptr) {
      static const unsigned long ADJ = 0x83AA7E80;   // 2208988800 seconds
      *ntpSec = seconds + ADJ;
   }

   // Microseconds to fractional second
   if (ntpSecF != nullptr) {
      double fsec = static_cast<double>(uSec) / 1000000.0;
      *ntpSecF = static_cast<unsigned long>((fsec * P32) + 0.5);
   }
   return true;
}

// Convert NTP time stamp to seconds since midnight (00:00:00), January 1, 1970
bool convertNtp2Sec(
      const unsigned long ntpSec,   // (IN) NTP seconds
      const unsigned long ntpSecF,  // (IN) NTP fraction of seconds
      unsigned long* const seconds, // (OUT) whole seconds since midnight (00:00:00), January 1, 1970
      unsigned long* const uSec     // (OUT) microseconds seconds
   )
{
   static const double P32 = 4294967296.0; // 2^32

   // Adjust by 2208988800L seconds (1900 to 1970)
   if (seconds != nullptr) {
      static const unsigned long ADJ = 0x83AA7E80;   // 2208988800 seconds
      *seconds = ntpSec - ADJ;
   }

   // fractional second to microseconds
   if (uSec != nullptr) {
      double fsec = static_cast<double>(ntpSecF) / P32;
      *uSec = static_cast<unsigned long>((fsec * 1000000.0) + 0.5);
   }
   return true;
}

//------------
// doesFileExist() -- Returns true if the file already exists
//------------
bool doesFileExist(const char* const fullname)
{
   bool result = false;
   if (fullname != nullptr) {
      // Test open the file
      std::ifstream t;
      t.open(fullname);
      if (t.is_open()) {
         // It's open, it exists!
         result = true;
         t.close();
      }
   }
   return result;
}


//------------
// Pre-multiply an array of 4D vectors with a 4x4 matrix
//------------
void lcPreMultVec4Array(
      const osg::Vec4 orig[],
      const osg::Matrix& matrix,
      osg::Vec4 result[],
      const int n
   )
{
   for (int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec4Array(
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

//------------
// Pre-multiply an array of 3D vectors with a 4x4 matrix
//------------
void lcPreMultVec3Array(
      const osg::Vec3 orig[],
      const osg::Matrix& matrix,
      osg::Vec3 result[],
      const int n
   )
{
   for (int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec3Array(
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

void preMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.preMult(orig[i]);
   }
}

//------------
// Post-multiply an array of 4D vectors with a 4x4 matrix
//------------
void lcPostMultVec4Array(
      const osg::Vec4 orig[],
      const osg::Matrix& matrix,
      osg::Vec4 result[],
      const int n
   )
{
   for (int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec4Array(
      const osg::Vec4d orig[],
      const osg::Matrixd& matrix,
      osg::Vec4d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec4Array(
      const osg::Vec4f orig[],
      const osg::Matrixf& matrix,
      osg::Vec4f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

//------------
// Post-multiply an array of 3D vectors with a 4x4 matrix
//------------
void lcPostMultVec3Array(
      const osg::Vec3 orig[],
      const osg::Matrix& matrix,
      osg::Vec3 result[],
      const int n
   )
{
   for (int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec3Array(
      const osg::Vec3d orig[],
      const osg::Matrixd& matrix,
      osg::Vec3d result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}

void postMultVec3Array(
      const osg::Vec3f orig[],
      const osg::Matrixf& matrix,
      osg::Vec3f result[],
      const unsigned int n
   )
{
   for (unsigned int i = 0; i < n; i++) {
      result[i] = matrix.postMult(orig[i]);
   }
}


//------------
// Returns the sine and cosine of an angle (radians)
//------------
void lcSinCos(const LCreal src, LCreal* const dst1, LCreal* const dst2)
{
   if (dst1 != nullptr && dst2 != nullptr) {
      *dst1 = lcSin(src);
      *dst2 = lcCos(src);
   }
}

void sinCos(const double src, double* const dst1, double* const dst2)
{
   if (dst1 != nullptr && dst2 != nullptr) {
      *dst1 = std::sin(src);
      *dst2 = std::cos(src);
   }
}

void sinCos(const float src, float* const dst1, float* const dst2)
{
   if (dst1 != nullptr && dst2 != nullptr) {
      *dst1 = std::sin(src);
      *dst2 = std::cos(src);
   }
}

//------------
// Computes the sines of an array of 'n' angles (radians)
//------------
void lcSinArray(const LCreal* const src, LCreal* const dst, const int n)
{
   const LCreal* ps = src;
   LCreal* pd = dst;
   for (int i = 0; i < n; i++) {
      *pd++ = lcSin(*ps++);
   }
}

void sinArray(const double* const src, double* const dst, const unsigned int n)
{
   const double* ps = src;
   double* pd = dst;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = std::sin(*ps++);
   }
}

void sinArray(const float* const src, float* const dst, const unsigned int n)
{
   const float* ps = src;
   float* pd = dst;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = sinf(*ps++);
   }
}

//------------
// Computes the cosines of an array of 'n' angles (radians)
//------------
void lcCosArray(const LCreal* const src, LCreal* const dst, const int n)
{
   const LCreal* ps = src;
   LCreal* pd = dst;
   for (int i = 0; i < n; i++) {
      *pd++ = lcCos(*ps++);
   }
}

void cosArray(const double* const src, double* const dst, const unsigned int n)
{
   const double* ps = src;
   double* pd = dst;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = std::cos(*ps++);
   }
}

void cosArray(const float* const src, float* const dst, const unsigned int n)
{
   const float* ps = src;
   float* pd = dst;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = cosf(*ps++);
   }
}

//------------
// Computes the sines and cosines of an array of 'n' angles (radians)
//------------
void lcSinCosArray(const LCreal* const src, LCreal* const dst1, LCreal* const dst2, const int n)
{
   const LCreal* ps = src;
   LCreal* pd1 = dst1;
   LCreal* pd2 = dst2;
   for (int i = 0; i < n; i++) {
      *pd1++ = lcSin(*ps);
      *pd2++ = lcCos(*ps++);
   }
}

void sinCosArray(const double* const src, double* const dst1, double* const dst2, const unsigned int n)
{
   const double* ps = src;
   double* pd1 = dst1;
   double* pd2 = dst2;
   for (unsigned int i = 0; i < n; i++) {
      *pd1++ = std::sin(*ps);
      *pd2++ = std::cos(*ps++);
   }
}

void sinCosArray(const float* const src, float* const dst1, float* const dst2, const unsigned int n)
{
   const float* ps = src;
   float* pd1 = dst1;
   float* pd2 = dst2;
   for (unsigned int i = 0; i < n; i++) {
      *pd1++ = sinf(*ps);
      *pd2++ = cosf(*ps++);
   }
}

//------------
// Computes the arc-cosines of an array of 'n' angles (radians)
//------------
void lcAcosArray(const LCreal* const src, LCreal* const dst, const int n)
{
   LCreal* pd = dst;
   const LCreal* ps = src;
   for (int i = 0; i < n; i++) {
      *pd++ = lcAcos(*ps++);
   }
}

void acosArray(const double* const src, double* const dst, const unsigned int n)
{
   double* pd = dst;
   const double* ps = src;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = std::acos(*ps++);
   }
}

void acosArray(const float* const src, float* const dst, const unsigned int n)
{
   float* pd = dst;
   const float* ps = src;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = acosf(*ps++);
   }
}

//------------
// Computes the arc-tangants for two, 'n' element arrays (yValues and xValues)
//------------
void lcAtan2Array(const LCreal* const yValues, const LCreal* const xValues, LCreal* const dst, const int n)
{
   LCreal* pd = dst;
   const LCreal* ps1 = yValues;
   const LCreal* ps2 = xValues;
   for (int i = 0; i < n; i++) {
      *pd++ = lcAtan2(*ps1++,*ps2++);
   }
}

void atan2Array(const double* const yValues, const double* const xValues, double* const dst, const unsigned int n)
{
   double* pd = dst;
   const double* ps1 = yValues;
   const double* ps2 = xValues;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = std::atan2(*ps1++,*ps2++);
   }
}

void atan2Array(const float* const yValues, const float* const xValues, float* const dst, const unsigned int n)
{
   float* pd = dst;
   const float* ps1 = yValues;
   const float* ps2 = xValues;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = atan2f(*ps1++,*ps2++);
   }
}

//------------
// Computes the square roots of 'n' real values
//------------
void lcSqrtArray(const LCreal* const src, LCreal* const dst, const int n)
{
   const LCreal* ps = src;
   LCreal* pd = dst;
   for (int i = 0; i < n; i++) {
      *pd++ = std::sqrt(*ps++);
   }
}

void sqrtArray(const double* const src, double* const dst, const unsigned int n)
{
   const double* ps = src;
   double* pd = dst;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = std::sqrt(*ps++);
   }
}

void sqrtArray(const float* const src, float* const dst, const unsigned int n)
{
   const float* ps = src;
   float* pd = dst;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = sqrtf(*ps++);
   }
}

//------------
// Raises 10 to the power of 'n' src values and returns the results in 'dst'
//------------
void lcPow10Array(const LCreal* const src, LCreal* const dst, const int n)
{
   LCreal* pd = dst;
   const LCreal* ps = src;
   for (int i = 0; i < n; i++) {
      *pd++ = lcPow( static_cast<LCreal>(10.0), *ps++);
   }
}

void pow10Array(const double* const src, double* const dst, const unsigned int n)
{
   double* pd = dst;
   const double* ps = src;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = std::pow(10.0,*ps++);
   }
}

void pow10Array(const float* const src, float* const dst, const unsigned int n)
{
   float* pd = dst;
   const float* ps = src;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = powf(10.0f,*ps++);
   }
}

//------------
// Multiply an array of reals with a constant
//------------
void lcMultArrayConst(const LCreal* const src, const LCreal c, LCreal* const dst, const int n)
{
   LCreal* pd = dst;
   const LCreal* ps = src;
   for (int i = 0; i < n; i++) {
      *pd++ = *ps++ * c;
   }
}

void multArrayConst(const double* const src, const double c, double* const dst, const unsigned int n)
{
   double* pd = dst;
   const double* ps = src;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = *ps++ * c;
   }
}

void multArrayConst(const float* const src, const float c, LCreal* const dst, const unsigned int n)
{
   LCreal* pd = dst;
   const float* ps = src;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = *ps++ * c;
   }
}

//------------
// Converts the integer number, 'num', to an ascii string.
//------------
void lcInteger2Str(const int num, char* const str, int width)
{
   const int MAX_CHARS = 32;

   // check sign
   bool isNeg = (num < 0);

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
bool lcStrcpy(char* const strDest, const size_t sizeOfDest, const char* const strSource)
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
bool lcStrncpy(char* const strDest, const size_t sizeOfDest, const char* const strSource, const size_t count)
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
bool lcStrcat(char* const strDest, const size_t sizeOfDest, const char* const strSource)
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
bool lcStrcpyFull(char* const strDest, const size_t sizeOfDest, const char* const strSource)
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
bool lcStrncpyFull(char* const strDest, const size_t sizeOfDest, const char* const strSource, const size_t count)
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
bool lcStrcatFull(char* const strDest, const size_t sizeOfDest, const char* const strSource)
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
int lcStrcasecmp(const char* const s1, const char* const s2)
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
int lcStrncasecmp(const char* const s1, const char* const s2, const size_t n)
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

//------------
// returns number of digits in the whole number part (i.e. left of decimal)
// of a floating point number
//------------
unsigned int getDigits(const double x)
{
   unsigned int digits = 2;
   double z = std::fabs(x);
   while (z >= 10.0) {
      digits++;
      z /= 10.0;
   }
   return digits;
}

} // End Eaagles namespace
