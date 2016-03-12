//------------------------------------------------------------------------------
// Classes:
//    RVector  -- Row Vector
//    CVector  -- Column Vector
//------------------------------------------------------------------------------
// References:
// "Applied Numerical Analysis Using Matlab", 1ed, Laurene V. Fausett, Prentice Hall (1999)
// "Introduction to Numerical Analysis", 2ed, Kendall E. Atkinson, Wiley (1989)
// "Numerical Methods", 1ed, Robert W. Hornbeck, Quantum (1975)
// "Applied Numerical Methods for Digital Computation", 4ed, James-Smith-Wolford, Harper Collins (1993)
//------------------------------------------------------------------------------
#ifndef __oe_base_Vectors_H__
#define __oe_base_Vectors_H__

#include "openeaagles/base/Matrix.h"

#include "openeaagles/base/osg/Vec2"
#include "openeaagles/base/osg/Vec3"
#include "openeaagles/base/osg/Vec4"
#include "openeaagles/base/osg/Matrix"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: RVector
// Description: General purpose row vector
//
// Factory name: RVector
//------------------------------------------------------------------------------
class RVector : private Matrix {
   DECLARE_SUBCLASS(RVector, Matrix)

public:
   RVector() : Matrix() { STANDARD_CONSTRUCTOR() }

   // ---
   // 'N' size vector filled with zeros.
   // ---
   RVector(const unsigned int N) : Matrix(1,N) { STANDARD_CONSTRUCTOR() }

   // ---
   // 'N' size vector filled with 'dataSize' values from the 'data' array.
   // ---
   RVector(const unsigned int N, const double* const data, const unsigned int dataSize)
      : Matrix(1,N,data,dataSize) { STANDARD_CONSTRUCTOR() }

   // ---
   // construct vectors from OSG vectors
   // ---
   RVector(const osg::Vec2d& v) : Matrix()   { setMatrix(1,2,v.ptr(),2); STANDARD_CONSTRUCTOR() }
   RVector(const osg::Vec2f& v) : Matrix()   { setMatrix(1,2,v.ptr(),2); STANDARD_CONSTRUCTOR() }
   RVector(const osg::Vec3d& v) : Matrix()   { setMatrix(1,3,v.ptr(),3); STANDARD_CONSTRUCTOR() }
   RVector(const osg::Vec3f& v) : Matrix()   { setMatrix(1,3,v.ptr(),3); STANDARD_CONSTRUCTOR() }
   RVector(const osg::Vec4d& v) : Matrix()   { setMatrix(1,4,v.ptr(),4); STANDARD_CONSTRUCTOR() }
   RVector(const osg::Vec4f& v) : Matrix()   { setMatrix(1,4,v.ptr(),4); STANDARD_CONSTRUCTOR() }

   // ---
   // Assigned from OSG vectors
   // ---
   RVector& operator=(const osg::Vec2d& v)   { setMatrix(1,2,v.ptr(),2); return *this; }
   RVector& operator=(const osg::Vec2f& v)   { setMatrix(1,2,v.ptr(),2); return *this; }
   RVector& operator=(const osg::Vec3d& v)   { setMatrix(1,3,v.ptr(),3); return *this; }
   RVector& operator=(const osg::Vec3f& v)   { setMatrix(1,3,v.ptr(),3); return *this; }
   RVector& operator=(const osg::Vec4d& v)   { setMatrix(1,4,v.ptr(),4); return *this; }
   RVector& operator=(const osg::Vec4f& v)   { setMatrix(1,4,v.ptr(),4); return *this; }

   // Size of the vector
   unsigned int getSize() const     { return getCols(); }

   // Returns the i'th value; zero based
   double& operator[](int i)        { return (*this)(0,i); }
   double operator[](int i) const   { return (*this)(0,i); }

   // Is this a good vector
   bool isGoodVector() const        { return (getCols() > 0); }

   // isZeroVector
   bool isZeroVector() const;

   // Returns the norm (or magnitude)
   double getNorm() const;

   // Normalize the vector
   bool normalize();

   // Fill with the value of 'x'
   bool fillWith(const double x);

   // Get the maximum magnitude element of this vector
   double getMaxMag() const;

   // Get the minimum magnitude element of this vector
   double getMinMag() const;

   // ---
   // Adds 'v' to 'this' vector and returns true;
   // if 'v' is not compatible then 'this' vector is unchanged and false is returned.
   // ---
   bool add(const RVector& v) {
      return Matrix::add( v );
   }

   // ---
   // Subtracts 'v' from 'this' vector and returns true;
   // if 'v' is not compatible then, 'this' is unchanged and false is returned.
   // ---
   bool subtract(const RVector& v) {
      return Matrix::subtract( v );
   }

   // ---
   // Multiplies 'this' vector by scalar 's'
   // ---
   bool multiply(const double s) {
      return Matrix::multiply(s);
   }

   // ---
   // Returns a pre-ref'()d pointer the transpose of this vector
   // ---
   CVector* getTranspose() const {
      return (CVector*) Matrix::getTranspose();
   }

   // showVector
   void showVector(const unsigned int DP=2, const unsigned int FW=8) const;

   bool setDecPoint(const unsigned int DP);
   bool setFldWidth(const unsigned int FW);

   int getDecPoint() const {
      return Matrix::getDecPoint();
   }
   int getFldWidth() const {
      return Matrix::getFldWidth();
   }

   friend std::ostream& operator<<(std::ostream& sout, const RVector& V);

   void ref() { BaseClass::ref(); }
   void unref() { BaseClass::unref(); }
};


// ---
// equal
// ---
inline bool operator==(const RVector& v1, const RVector& v2)
{
   return (*((const Matrix*)&v1) == *((const Matrix*)&v2));
}

// ---
// not equal
// ---
inline bool operator!=(const RVector& v1, const RVector& v2)
{
   return (*((const Matrix*)&v1) != *((const Matrix*)&v2));
}

// ---
// Returns a pre-ref()'d pointer to a new RVector that is the sum of 'v1'
// and 'v2', or returns zero if they are not compatible for addition
// ---
inline RVector* add(const RVector& v1, const RVector& v2)
{
   Matrix* r = add( *((const Matrix*)&v1), *((const Matrix*)&v2) );
   return (RVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new RVector that is the result of 'v1'
// minus 'v2', or returns zero if they are not compatible for subtraction
// ---
inline RVector* subtract(const RVector& v1, const RVector& v2) {
   Matrix* r = subtract( *((const Matrix*)&v1), *((const Matrix*)&v2) );
   return (RVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new RVector that is the negation of 'v'
// ---
inline RVector* minus(const RVector& v)
{
   Matrix* r = minus( *((const Matrix*)&v) );
   return (RVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new RVector that is the result of (v * m)
// or returns zero if they are not compatible for multiplication
// ---
inline RVector* multiply(const RVector& v, const Matrix& m)
{
   Matrix* r = multiply( *((const Matrix*)&v), m );
   return (RVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new RVector that is the result of v * s
// ---
inline RVector* multiply(const RVector& v, const double s)
{
   Matrix* r = multiply( *((const Matrix*)&v), s);
   return (RVector*) r;
}

// ---
// Returns a pre-ref'()d pointer to a RVector copy of the matrix A row vector
// indicated by the index row
// ---
inline RVector* getRowVector(const Matrix& A, const unsigned int r)
{
   bool B1 = A.isGoodMatrix();
   bool B2 = r < A.getRows();
   if (!B1 || !B2) return 0;

   const unsigned int N = A.getCols();

   RVector* pV = new RVector(N);
   if (pV == 0) return 0;

   for (unsigned int c = 0; c < N; c++)
   {
      (*pV)[c] = A(r,c);
   }
   return pV;
}


//------------------------------------------------------------------------------
// Class: CVector
// Description: General purpose column vector
//
// Factory name: CVector
//------------------------------------------------------------------------------
class CVector : private Matrix {
   DECLARE_SUBCLASS(CVector, Matrix)

public:
   // ---
   // Empty vector
   // ---
   CVector() : Matrix() { STANDARD_CONSTRUCTOR() }

   // ---
   // 'N' size vector filled with zeros.
   // ---
   CVector(const unsigned int N) : Matrix(N,1) { STANDARD_CONSTRUCTOR() }

   // ---
   // 'N' size vector filled with 'dataSize' values from the 'data' array.
   // ---
   CVector(const unsigned int N, const double* const data, const unsigned int dataSize)
      : Matrix(N,1,data,dataSize) { STANDARD_CONSTRUCTOR() }

   // ---
   // construct vectors from OSG vectors
   // ---
   CVector(const osg::Vec2d& v) : Matrix()   { setMatrix(2,1,v.ptr(),2); STANDARD_CONSTRUCTOR() }
   CVector(const osg::Vec2f& v) : Matrix()   { setMatrix(2,1,v.ptr(),2); STANDARD_CONSTRUCTOR() }
   CVector(const osg::Vec3d& v) : Matrix()   { setMatrix(3,1,v.ptr(),3); STANDARD_CONSTRUCTOR() }
   CVector(const osg::Vec3f& v) : Matrix()   { setMatrix(3,1,v.ptr(),3); STANDARD_CONSTRUCTOR() }
   CVector(const osg::Vec4d& v) : Matrix()   { setMatrix(4,1,v.ptr(),4); STANDARD_CONSTRUCTOR() }
   CVector(const osg::Vec4f& v) : Matrix()   { setMatrix(4,1,v.ptr(),4); STANDARD_CONSTRUCTOR() }

   // ---
   // Assigned from OSG vectors
   // ---
   CVector& operator=(const osg::Vec2d& v)   { setMatrix(2,1,v.ptr(),2); return *this; }
   CVector& operator=(const osg::Vec2f& v)   { setMatrix(2,1,v.ptr(),2); return *this; }
   CVector& operator=(const osg::Vec3d& v)   { setMatrix(3,1,v.ptr(),3); return *this; }
   CVector& operator=(const osg::Vec3f& v)   { setMatrix(3,1,v.ptr(),3); return *this; }
   CVector& operator=(const osg::Vec4d& v)   { setMatrix(4,1,v.ptr(),4); return *this; }
   CVector& operator=(const osg::Vec4f& v)   { setMatrix(4,1,v.ptr(),4); return *this; }


   // Size of the vector
   unsigned int getSize() const     { return getRows(); }

   // Returns the i'th value; zero based
   double& operator[](int i)        { return (*this)(i,0); }
   double operator[](int i) const   { return (*this)(i,0); }

   // Is this a good vector
   bool isGoodVector() const        { return (getRows() > 0); }

   // Is this a zero vector
   bool isZeroVector() const;

   // Returns the norm (or magnitude)
   double getNorm() const;

   // Normalize the vector
   bool normalize();

   // Fill with the value of 'x'
   bool fillWith(const double x);

   // Get the maximum magnitude element of this vector
   double getMaxMag() const;

   // Get the minimum magnitude element of this vector
   double getMinMag() const;

   // ---
   // Adds 'v' to 'this' vector and returns true;
   // if 'v' is not compatible then 'this' vector is unchanged and false is returned.
   // ---
   bool add(const CVector& v) {
      return Matrix::add( v );
   }

   // ---
   // Subtracts 'v' from 'this' vector and returns true;
   // if 'v' is not compatible then 'this' vector is unchanged and false is returned.
   // ---
   bool subtract(const CVector& v) {
      return Matrix::subtract( v );
   }

   // ---
   // Multiplies 'this' vector by scalar 's'
   // ---
   bool multiply(const double s) {
      return Matrix::multiply(s);
   }

   // ---
   // Returns a pre-ref'()d pointer the transpose of this vector
   // ---
   RVector* getTranspose() const {
      return (RVector*) Matrix::getTranspose();
   }

   // showVector
   void showVector(const unsigned int DP=2, const unsigned int FW=8) const;

   bool setDecPoint(const unsigned int DP);
   bool setFldWidth(const unsigned int FW);

   int getDecPoint() const {
      return Matrix::getDecPoint();
   }
   int getFldWidth() const {
      return Matrix::getFldWidth();
   }


   // overloaded "<<" operator
   friend std::ostream& operator<<(std::ostream& sout, const CVector& V);

   void ref() { BaseClass::ref(); }
   void unref() { BaseClass::unref(); }
};


// ---
// equal
// ---
inline bool operator==(const CVector& v1, const CVector& v2)
{
   return (*((const Matrix*)&v1) == *((const Matrix*)&v2));
}

// ---
// not equal
// ---
inline bool operator!=(const CVector& v1, const CVector& v2)
{
   return (*((const Matrix*)&v1) != *((const Matrix*)&v2));
}

// ---
// Returns a pre-ref()'d pointer to a new CVector that is the sum of 'v1'
// and 'v2', or returns zero if they are not compatible for addition
// ---
inline CVector* add(const CVector& v1, const CVector& v2)
{
   Matrix* r = add( *((const Matrix*)&v1), *((const Matrix*)&v2) );
   return (CVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new CVector that is the result of 'v1'
// minus 'v2', or returns zero if they are not compatible for subtraction
// ---
inline CVector* subtract(const CVector& v1, const CVector& v2)
{
   Matrix* r = subtract( *((const Matrix*)&v1), *((const Matrix*)&v2) );
   return (CVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new CVector that is the negation of 'v'
// ---
inline CVector* minus(const CVector& v)
{
   Matrix* r = minus( *((const Matrix*)&v) );
   return (CVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new CVector that is the result of (m * v)
// or returns zero if they are not compatible for multiplication
// ---
inline CVector* multiply(const Matrix& m, const CVector& v)
{
   Matrix* r = multiply(m, *((const Matrix*)&v));
   return (CVector*) r;
}

// ---
// Returns a pre-ref()'d pointer to a new CVector that is the result of v * s
// ---
inline CVector* multiply(const CVector& v, const double s)
{
   Matrix* r = multiply( *((const Matrix*)&v), s);
   return (CVector*) r;
}

// ---
// Returns a pre-ref'()d pointer to a CVector copy of the c'th column vector of matrix A
// ---
inline CVector* getColVector(const Matrix& A, const unsigned int c)
{
   bool B1 = A.isGoodMatrix();
   bool B2 = c < A.getCols();
   if (!B1 || !B2) return 0;

   const unsigned int N = A.getRows();
   CVector* pV = new CVector(N);
   if (pV == 0) return 0;

   for (unsigned int r = 0; r < N; r++) {
      (*pV)[r] = A(r,c);
   }
   return pV;
}


//------------------------------------------------------------------------------
// common functions
//------------------------------------------------------------------------------

// ---
// Dot (scalar) product of a row and column vector
// ---
inline double dotProduct(const RVector& v1, const CVector& v2)
{
   double sum = 0;
   const unsigned int N = v1.getSize();

   if (N == v2.getSize()) {
      for (unsigned int i = 0; i < N; i++) {
         sum += (v1[i] * v2[i]);
      }
   }
   return sum;
}

// ---
// Outer (tensor) product of a column and row vector:
// Returns a pre-ref()'d pointer to a new Matrix that is the outer product
// of a column and row matrix.
// ---
inline Matrix* outerProduct(const CVector& v1, const RVector& v2)
{
   Matrix* r = multiply( *((const Matrix*)&v1), *((const Matrix*)&v2) );
   return r;
}

// ---
// Cross product of two 3-D [ i j k ] vectors
// ---
inline CVector* crossProduct(const CVector& v1, const CVector& v2)
{
   CVector* p = 0;
   if (v1.getSize() == 3 && v2.getSize() == 3) {
      p = new CVector(3);
      (*p)[0] =  ( v1[1]*v2[2] - v1[2]*v2[1] );
      (*p)[1] = -( v1[0]*v2[2] - v1[2]*v2[0] );
      (*p)[2] =  ( v1[0]*v2[1] - v1[1]*v2[0] );
   }
   return p;
}

inline RVector* crossProduct(const RVector& v1, const RVector& v2)
{
   RVector* p = 0;
   if (v1.getSize() == 3 && v2.getSize() == 3) {
      p = new RVector(3);
      (*p)[0] =  ( v1[1]*v2[2] - v1[2]*v2[1] );
      (*p)[1] = -( v1[0]*v2[2] - v1[2]*v2[0] );
      (*p)[2] =  ( v1[0]*v2[1] - v1[1]*v2[0] );
   }
   return p;
}

inline CVector* getDiag(const Matrix& A)
{
   if (!A.isGoodMatrix() || !A.isSquare()) {
      return 0;
   }

   const unsigned int N = A.getRows();
   CVector* p = new CVector(N);
   for (unsigned int i = 0; i < N; i++) {
      (*p)[i] = A(i,i);
   }
   return p;
}

} // End namespace
} // End namespace

#endif
