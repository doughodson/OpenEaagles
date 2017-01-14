
#ifndef __oe_base_Matrix_H__
#define __oe_base_Matrix_H__

#include "openeaagles/base/Object.hpp"

#include "openeaagles/base/osg/Matrixf"
#include "openeaagles/base/osg/Matrixd"

#include <cmath>

namespace oe {
namespace base {
   class CVector; // Column vector
   class RVector; // Row vector

//------------------------------------------------------------------------------
// Class: Matrix
// Description:  Generic NxM matrix class
//
// Factory name: Matrix
//
// References:
// "Introduction to Numerical Analysis", 2ed, Kendall E. Atkinson, Wiley (1989)
// "Numerical Methods", 1ed, Robert W. Hornbeck, Quantum (1975)
// "Applied Numerical Methods for Digital Computation", 4ed, James-Smith-Wolford, Harper Collins (1993)
// "Applied Numerical Analysis Using Matlab", 1ed, Laurene V. Fausett, Prentice Hall (1999)
//------------------------------------------------------------------------------
class Matrix : public Object
{
   DECLARE_SUBCLASS(Matrix, Object)

public:
   static const unsigned int DEF_FLDWIDTH=8;
   static const unsigned int DEF_DECPOINT=2;

public:
   Matrix();

   // Matrix with 'r' rows and 'c' columns and filled with zeros and ones on the diagonal
   Matrix(const unsigned int r, const unsigned int c);

   // Matrix with 'r' rows and 'c' columns and filled with 'dataSize' values from the 'data' array.
   Matrix(const unsigned int r, const unsigned int c, const double* const data, const unsigned int dataSize);

   // Construct the matrix from an OSG matrix (double and float)
   Matrix(const osg::Matrixd& m);
   Matrix(const osg::Matrixf& m);

   // Assigned from an OSG matrix (double and float)
   Matrix& operator=(const osg::Matrixd& m);
   Matrix& operator=(const osg::Matrixf& m);

   unsigned int getRows() const                                         { return rows; }
   unsigned int getCols() const                                         { return cols; }

   bool isSquare() const                                                { return (rows == cols); }
   bool isGoodIndex(const unsigned int i, const unsigned int j) const   { return (i < rows) && (j < cols); }
   bool isGoodMatrix() const                                            { return ((mda != nullptr) && ((rows > 0) & (cols > 0))); }
   bool isSymmetric() const;

   // The (i,j) element
   double getElem(const unsigned int i, const unsigned int j) const     { return mda[i*cols + j]; }
   double operator()(unsigned int i, unsigned int j) const              { return mda[i*cols + j]; }
   double& operator()(unsigned int i, unsigned int j)                   { return mda[i*cols + j]; }

   double* operator[](unsigned int r);                                  // pointer to the n'th row
   const double* operator[](unsigned int r) const;                      // pointer to the n'th row (const version)

   friend bool operator==(const Matrix& m1, const Matrix& m2);          // equal
   friend bool operator!=(const Matrix& m1, const Matrix& m2);          // not equal

   double getMaxMag() const;
   double getMinMag() const;

   // Returns the determinate of the matrix
   double getDeterm() const;

   // Make this an identity matrix; returns false if not a square matrix
   bool makeIdent();
   bool makeIdent(const unsigned int k);

   // Make this a zero matrix
   bool makeZero();

   // ---
   // Adds 'm' to this matrix;
   // if 'm' is not compatible then this matrix is unchanged and false is returned.
   // ---
   bool add(const Matrix& m);

   // ---
   // Subtracts 'm' from this matrix;
   // if 'm' is not compatible then this matrix is unchanged and false is returned.
   // ---
   bool subtract(const Matrix& m);

   // ---
   // Multiplies this matrix by 'm';
   // if 'm' is not compatible then this matrix is unchanged and false is returned.
   // ---
   bool multiply(const Matrix& m);

   // Multiplies this matrix by 's'
   bool multiply(const double s);

   // Returns a pre-ref'()d pointer the transpose of this matrix
   Matrix* getTranspose() const;

   // ---
   // Returns a pre-ref()'d pointer the inverse of this matrix, or returns zero
   // if the matrix can not be inverted
   // ---
   Matrix* getInvGJ() const;
   Matrix* getInvLU() const;

   // ---
   // Returns a pre-ref()'d pointer to a new matrix that is the sum of 'm1'
   // and 'm2', or returns zero if they are not compatible for addition
   // ---
   friend Matrix* add(const Matrix& m1, const Matrix& m2);

   // ---
   // Returns a pre-ref()'d pointer to a new matrix that is the result of 'm1'
   // minus 'm2', or returns zero if they are not compatible for subtraction
   // ---
   friend Matrix* subtract(const Matrix& m1, const Matrix& m2);

   // ---
   // Returns a pre-ref()'d pointer to a new matrix that is the negation of 'm1'
   // ---
   friend Matrix* minus(const Matrix& m1);

   // ---
   // Returns a pre-ref()'d pointer to a new matrix that is the result of m1 * m2,
   // or returns zero if they are not compatible for multiplication
   // ---
   friend Matrix* multiply(const Matrix& m1, const Matrix& m2);

   // ---
   // Returns a pre-ref()'d pointer to a new matrix that is the result of m1 * s
   // ---
   friend Matrix* multiply(const Matrix& m1, const double s);

   // ---
   // Transpose this matrix
   // ---
   bool transpose();

   // ---
   // Invert this matrix
   // ---
   bool invert();

   // ---
   // For this matrix, given the maximum allowed iterations and the maximum allowed
   // error, returns the dominant eigenvalue and its associated eigenvector.
   //
   // matrix requirements: square
   // ---
   bool getEigenPower(const double maxErr, const int maxIter,
                   double* const eval, CVector* const evec);

   // ---
   // For this matrix, the two pointers in the argument list point to the lower and
   // upper Cholesky matrices. The upper matrix is the transpose of the lower.
   //
   // matrix requirements: symmetric
   // ---
   bool getCholesky(Matrix* const pL, Matrix* const pU) const;

   // ---
   // For this matrix, the two pointers in the argument list point to the lower and
   // upper LU matrices (Doolittle).
   //
   // matrix requirements: square
   // ---
   bool getLU(Matrix* const pL, Matrix* const pU) const;

   // ---
   // For this matrix, the two pointers in the argument list (pQ and pR) point to
   // the Q (lower) and R (upper) matrices respectively.
   //
   // matrix requirements: square
   // ---
   bool getQR(Matrix* const pQ, Matrix* const pR) const;

   // ---
   // getTriDiagonal (Uses Householder method)
   //
   // Given an n-dimensional vector X and an index k such that 0 < k < n, a vector W
   // is found so that the matrix H = I - 2*W*W' reduces positions k,...,n-1 of
   // vector X to zero.
   // i.e. If X = [x0,x1,...,x(n-1)]', then H*X has the form [x0,...,x(k-1),0,...,0]'.
   //
   // matrix requirements: symmetric
   // ---
   bool getTriDiagonal(Matrix* const pA) const;

   // ---
   // Set the (i,j) element to 'x'
   // ---
   bool setElem(const unsigned int i, const unsigned int j, const double x);


   // Field width for operator<<
   unsigned int getFldWidth() const;
   bool setFldWidth(const unsigned int);

   // Decimal point for operator<<
   unsigned int getDecPoint() const;
   bool setDecPoint(const unsigned int);

   void showMatrix(const unsigned int DP=DEF_DECPOINT,
                   const unsigned int FW=DEF_FLDWIDTH) const;

   friend std::ostream& operator<<(std::ostream& sout, const Matrix& A);

protected:
   // empty the matrix
   void emptyMatrix();

   // Matrix with 'r' rows and 'c' columns and filled with zeros and ones on the diagonal
   void setMatrix(const unsigned int r, const unsigned int c);

   // Matrix with 'r' rows and 'c' columns and filled with 'dataSize' values from the 'data' array.
   void setMatrix(const unsigned int r, const unsigned int c, const double* const data, const unsigned int dataSize);
   void setMatrix(const unsigned int r, const unsigned int c, const float* const data, const unsigned int dataSize);

   bool pivotRow(const unsigned int, const unsigned int);
   bool pivotCol(const unsigned int, const unsigned int);
   bool augment(const Matrix&);
   bool swapRow(const unsigned int, const unsigned int);
   bool swapCol(const unsigned int, const unsigned int);
   bool addRow(const unsigned int, const unsigned int, const double k=1.0);
   bool addCol(const unsigned int, const unsigned int, const double k=1.0);
   bool mulRow(const unsigned int, const double k=1.0);
   bool mulCol(const unsigned int, const double k=1.0);
   bool remRow(const unsigned int);
   bool remRows(const unsigned int, const unsigned int);
   bool remCol(const unsigned int);
   bool remCols(const unsigned int, const unsigned int);
   bool remRowCol(const unsigned int, const unsigned int);

private:
   void initData();        // initialize data

   unsigned int rows;      // number of rows
   unsigned int cols;      // number of columns
   double* mda;            // pointer to the Matrix Data Array

   unsigned int fldWidth;  // matrix element field width for display of matrix
   unsigned int decPoint;  // decimal point accuracy for display of matrix
};


//------------------------------------------------------------------------------
// friend functions
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& sout, const Matrix& A);

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

inline bool operator==(const Matrix& m1, const Matrix& m2)
{
   if (m1.rows != m2.rows || m1.cols != m2.cols) return false;
   const double EPS = 1.0e-15;

   for (unsigned int i=0; i < m1.rows; i++) {
      unsigned int idx = m1.cols*i;
      for (unsigned int j=0; j < m1.cols; j++) {
         //if (m1.mda[idx] != m2.mda[idx]) return false;
         if (std::fabs(m1.mda[idx] - m2.mda[idx]) > EPS) return false;
         idx++;
      }
   }

   return true;
}

inline bool operator!=(const Matrix& m1, const Matrix& m2)
{
   if ((m1.rows != m2.rows) || (m1.cols != m2.cols)) return true;
   const double EPS = 1.0e-15;

   for (unsigned int i=0; i < m1.rows; i++) {
      unsigned int idx = m1.cols*i;
      for (unsigned int j=0; j < m1.cols; j++)  {
         //if (m1.mda[idx] != m2.mda[idx]) return true;
         if (std::fabs(m1.mda[idx] - m2.mda[idx]) > EPS) return true;
         idx++;
      }
   }

   return false;
}


inline Matrix* add(const Matrix& m1, const Matrix& m2)
{
   Matrix* temp = 0;

   if (m1.rows == m2.rows && m1.cols == m2.cols) {
      temp = new Matrix(m1);
      for (unsigned int i=0; i < temp->rows; i++) {
         unsigned int idx = temp->cols*i;
         for (unsigned int j=0; j < temp->cols; j++) {
            temp->mda[idx] += m2.mda[idx];
            idx++;
         }
      }
   }

   return temp;
}


inline Matrix* subtract(const Matrix& m1, const Matrix& m2)
{
   Matrix* temp = 0;

   if  (m1.rows == m2.rows && m1.cols == m2.cols) {
      temp = new Matrix(m1);
      for (unsigned int i = 0; i < temp->rows; i++) {
         for (unsigned int j = 0; j < temp->cols; j++) {
            unsigned int idx = temp->cols*i + j;
            temp->mda[idx] -= m2.mda[idx];
         }
      }
   }

   return temp;
}


inline Matrix* minus(const Matrix& m1)
{
   const auto temp = new Matrix(m1);

   for (unsigned int i = 0; i < temp->rows; i++) {
      unsigned int idx = temp->cols*i;
      for (unsigned int j = 0; j < temp->cols; j++) {
         temp->mda[idx] = -temp->mda[idx];
         idx++;
      }
   }
   return temp;
}


inline Matrix* multiply(const Matrix& m1, const Matrix& m2)
{
   Matrix* temp = 0;

   if (m1.cols == m2.rows) {
      temp = new Matrix(m1.rows, m2.cols);

      for (unsigned int i=0; i<temp->rows; i++) {
         for (unsigned int j=0; j < temp->cols; j++) {
            unsigned int idx = temp->cols*i + j;
            temp->mda[idx] = 0.0;
            for (unsigned int k=0; k < m1.cols; k++) {
               unsigned int idx1 = m1.cols*i + k;
               unsigned int idx2 = m2.cols*k + j;
               temp->mda[idx] += (m1.mda[idx1] * m2.mda[idx2]);
            }
         }
      }
   }
   return temp;
}


inline Matrix* multiply(const Matrix& m1, const double s)
{
   const auto temp = new Matrix(m1);
   for (unsigned int i=0; i<temp->rows; i++) {
      for (unsigned int j=0; j<temp->cols; j++) {
         unsigned int idx = temp->cols*i + j;
         temp->mda[idx] *= s;
      }
   }
   return temp;
}

inline double getTrace(const Matrix& A)
{
   if (!A.isGoodMatrix() || !A.isSquare()) {
      return -999999.0;
   }

   int N = A.getRows();
   double trace = 0.0;
   for (int i=0; i<N; i++) {
      trace += A(i,i);
   }
   return trace;
}

}
}

#endif
