//------------------------------------------------------------------------------
// CVector and RVector
//------------------------------------------------------------------------------
#include "openeaagles/base/Vectors.h"
#include <iostream>
#include <iomanip>

namespace oe {
namespace base {

//==============================================================================
// RVector
//==============================================================================
IMPLEMENT_SUBCLASS(RVector,"RVector")
EMPTY_SLOTTABLE(RVector)
EMPTY_SERIALIZER(RVector)
EMPTY_COPYDATA(RVector)
EMPTY_DELETEDATA(RVector)


//------------------------------------------------------------------------------
// isZeroVector
//------------------------------------------------------------------------------
bool RVector::isZeroVector() const
{
   bool ok = true;
   const unsigned int SIZE = getSize();
   unsigned int k = 0;
   while (ok && k < SIZE) {
      ok = ((*this)[k++] == 0.0);
   }
   return ok;
}

//------------------------------------------------------------------------------
// getNorm
//------------------------------------------------------------------------------
double RVector::getNorm() const
{
   double x = 0.0;
   const unsigned SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      x += (*this)[i] * (*this)[i];
   }
   return std::sqrt(x);
}

//------------------------------------------------------------------------------
// normalize this vector
//------------------------------------------------------------------------------
bool RVector::normalize()
{
   if (isZeroVector()) return false;

   const double sf = 1.0 / getNorm();
   multiply(sf);
   return true;
}

//------------------------------------------------------------------------------
// fill with the value of 'x'
//------------------------------------------------------------------------------
bool RVector::fillWith(const double x)
{
   if (!isGoodVector())
      return false;

   const unsigned int SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      (*this)[i] = x;
   }
   return true;
}

//------------------------------------------------------------------------------
// Get the maximum magnitude element of this vector
//------------------------------------------------------------------------------
double RVector::getMaxMag() const
{
   double val = (*this)[0];
   double max = std::fabs(val);
   const unsigned int SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      double t = (*this)[i];
      double x = std::fabs(t);
      if (max < x) {
         max = x;
         val = t;
      }
   }
   return val;
}

//------------------------------------------------------------------------------
// Get the minimum magnitude element of this vector
//------------------------------------------------------------------------------
double RVector::getMinMag() const
{
   double val = (*this)[0];
   double min = std::fabs(val);
   const unsigned int SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      double t = (*this)[i];
      double x = std::fabs(t);
      if (min > x) {
         min = x;
         val = t;
      }
   }
   return val;
}

//------------------------------------------------------------------------------
// output the row vector
//------------------------------------------------------------------------------
void RVector::showVector(const unsigned int DECPNT,
                         const unsigned int FLDWTH) const
{
   static_cast<const Matrix*>(this)->showMatrix(DECPNT, FLDWTH);
   //std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& sout, const RVector& V)
{
   sout << *(static_cast<const Matrix*>(&V)) << std::endl;
   return sout;
}

bool RVector::setDecPoint(const unsigned int DP)
{
   return static_cast<Matrix*>(this)->setDecPoint(DP);
}

bool RVector::setFldWidth(const unsigned int FW)
{
   return static_cast<Matrix*>(this)->setFldWidth(FW);
}


//==============================================================================
// CVector
//==============================================================================
IMPLEMENT_SUBCLASS(CVector,"CVector")
EMPTY_SLOTTABLE(CVector)
EMPTY_SERIALIZER(CVector)
EMPTY_COPYDATA(CVector)
EMPTY_DELETEDATA(CVector)


//------------------------------------------------------------------------------
// isZeroVector
//------------------------------------------------------------------------------
bool CVector::isZeroVector() const
{
   bool ok = true;
   const unsigned int SIZE = getSize();
   unsigned int k = 0;
   while (ok && k < SIZE) {
      ok = ((*this)[k++] == 0.0);
   }
   return ok;
}

//------------------------------------------------------------------------------
// getNorm
//------------------------------------------------------------------------------
double CVector::getNorm() const
{
   double x = 0.0;
   const unsigned int SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      x += (*this)[i] * (*this)[i];
   }
   return std::sqrt(x);
}

//------------------------------------------------------------------------------
// normalize this vector
//------------------------------------------------------------------------------
bool CVector::normalize()
{
   if (isZeroVector()) return false;

   double sf = 1.0 / getNorm();
   multiply(sf);
   return true;
}

//------------------------------------------------------------------------------
// fill with the value of 'x'
//------------------------------------------------------------------------------
bool CVector::fillWith(const double x)
{
   if (!isGoodVector())
      return false;

   const unsigned int SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      (*this)[i] = x;
   }
   return true;
}

//------------------------------------------------------------------------------
// Get the maximum magnitude element of this vector
//------------------------------------------------------------------------------
double CVector::getMaxMag() const
{
   double val = (*this)[0];
   double max = std::fabs(val);
   const unsigned int SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      double t = (*this)[i];
      double x = std::fabs(t);
      if (max < x) {
         max = x;
         val = t;
      }
   }
   return val;
}

//------------------------------------------------------------------------------
// Get the minimum magnitude element of this vector
//------------------------------------------------------------------------------
double CVector::getMinMag() const
{
   double val = (*this)[0];
   double min = std::fabs(val);
   const unsigned int SIZE = getSize();
   for (unsigned int i = 0; i < SIZE; i++) {
      double t = (*this)[i];
      double x = std::fabs(t);
      if (min > x) {
         min = x;
         val = t;
      }
   }
   return val;
}


//------------------------------------------------------------------------------
// output the column vector
//------------------------------------------------------------------------------
void CVector::showVector(const unsigned int DECPNT,
                         const unsigned int FLDWTH) const
{
   static_cast<const Matrix*>(this)->showMatrix(DECPNT, FLDWTH);
   //std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& sout, const CVector& V)
{
   sout << *static_cast<const Matrix*>(&V) << std::endl;
   return sout;
}

bool CVector::setDecPoint(const unsigned int DP)
{
   return static_cast<Matrix*>(this)->setDecPoint(DP);
}

bool CVector::setFldWidth(const unsigned int FW)
{
   return static_cast<Matrix*>(this)->setFldWidth(FW);
}


} // End base namespace
} // End oe namespace

