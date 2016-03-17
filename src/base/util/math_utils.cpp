
#include <cmath>

namespace oe {
namespace base {

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

//------------
// Returns the sine and cosine of an angle (radians)
//------------

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
