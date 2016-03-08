//------------------------------------------------------------------------------
// Support functions
//------------------------------------------------------------------------------

#include "openeaagles/base/platform/support.h"
#include <cstring>
#include <cmath>
#include <fstream>

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

namespace oe {
namespace base {

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
      const double fsec = static_cast<double>(uSec) / 1000000.0;
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
      const double fsec = static_cast<double>(ntpSecF) / P32;
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
// Returns the sine and cosine of an angle (radians)
//------------
void lcSinCos(const double src, double* const dst1, double* const dst2)
{
   if (dst1 != nullptr && dst2 != nullptr) {
      *dst1 = std::sin(src);
      *dst2 = std::cos(src);
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
void lcSinArray(const double* const src, double* const dst, const int n)
{
   const double* ps = src;
   double* pd = dst;
   for (int i = 0; i < n; i++) {
      *pd++ = std::sin(*ps++);
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
void lcCosArray(const double* const src, double* const dst, const int n)
{
   const double* ps = src;
   double* pd = dst;
   for (int i = 0; i < n; i++) {
      *pd++ = std::cos(*ps++);
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
void lcSinCosArray(const double* const src, double* const dst1, double* const dst2, const int n)
{
   const double* ps = src;
   double* pd1 = dst1;
   double* pd2 = dst2;
   for (int i = 0; i < n; i++) {
      *pd1++ = std::sin(*ps);
      *pd2++ = std::cos(*ps++);
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
void lcAcosArray(const double* const src, double* const dst, const int n)
{
   double* pd = dst;
   const double* ps = src;
   for (int i = 0; i < n; i++) {
      *pd++ = std::acos(*ps++);
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
void lcAtan2Array(const double* const yValues, const double* const xValues, double* const dst, const int n)
{
   double* pd = dst;
   const double* ps1 = yValues;
   const double* ps2 = xValues;
   for (int i = 0; i < n; i++) {
      *pd++ = std::atan2(*ps1++,*ps2++);
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
void lcSqrtArray(const double* const src, double* const dst, const int n)
{
   const double* ps = src;
   double* pd = dst;
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
void lcPow10Array(const double* const src, double* const dst, const int n)
{
   double* pd = dst;
   const double* ps = src;
   for (int i = 0; i < n; i++) {
      *pd++ = std::pow(static_cast<double>(10.0), *ps++);
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
void lcMultArrayConst(const double* const src, const double c, double* const dst, const int n)
{
   double* pd = dst;
   const double* ps = src;
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

void multArrayConst(const float* const src, const float c, double* const dst, const unsigned int n)
{
   double* pd = dst;
   const float* ps = src;
   for (unsigned int i = 0; i < n; i++) {
      *pd++ = *ps++ * c;
   }
}

}
}

