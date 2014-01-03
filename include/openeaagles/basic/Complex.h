//==============================================================================
// Class: Complex
//==============================================================================
#ifndef __Eaagles_Basic_Complex_H__
#define __Eaagles_Basic_Complex_H__

#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {
   class Number;

//==============================================================================
// Class: Complex
// Description:  Generic complex number class
//
// Factory name: Complex
// Slots:
//     imag  <Number>  ! imaginary (default: 0)
//==============================================================================
class Complex : public Number {
   DECLARE_SUBCLASS(Complex, Number)

public:
   //--------------------------------------------------------------------------
   // Constructors
   //--------------------------------------------------------------------------
   Complex();
   Complex(const double r);
   Complex(const double r, const double i);


   //--------------------------------------------------------------------------
   // Get/Set Functions
   //--------------------------------------------------------------------------
   // Number::getReal()          // returns real part
   double getImag() const;       // returns imaginary part
   double getMag() const;        // returns phasor magnitude
   double getArg() const;        // returns phasor argument

   bool setReal(const double r);
   bool setImag(const double i);


   //--------------------------------------------------------------------------
   // Comparison Functions
   //--------------------------------------------------------------------------
   bool operator==(const Complex& z) const;
   bool operator!=(const Complex& z) const;


   //--------------------------------------------------------------------------
   // Unary Arithmetic Functions and Overloaded Operators
   //--------------------------------------------------------------------------
   const Complex& add(const Complex& z);
   const Complex& operator+=(const Complex& z);
   const Complex& add(const double r);
   const Complex& operator+=(const double r);

   const Complex& subtract(const Complex& z);
   const Complex& operator-=(const Complex& z);
   const Complex& subtract(const double r);
   const Complex& operator-=(const double r);

   const Complex& multiply(const Complex& z);
   const Complex& operator*=(const Complex& z);
   const Complex& multiply(const double r);
   const Complex& operator*=(const double r);

   const Complex& divide(const Complex& z);
   const Complex& operator/=(const Complex& z);
   const Complex& divide(const double r);
   const Complex& operator/=(const double r);


   //--------------------------------------------------------------------------
   // Binary Arithmetic Functions and Overloaded Operators
   //--------------------------------------------------------------------------
   friend Complex add(const Complex& z1, const Complex& z2);
   friend Complex operator+(const Complex& z1, const Complex& z2);
   friend Complex add(const double r, const Complex& z);
   friend Complex operator+(const double r, const Complex& z);
   friend Complex add(const Complex& z, const double r);
   friend Complex operator+(const Complex& z, const double r);

   friend Complex subtract(const Complex& z1, const Complex& z2);
   friend Complex operator-(const Complex& z1, const Complex& z2);
   friend Complex subtract(const double r, const Complex& z);
   friend Complex operator-(const double r, const Complex& z);
   friend Complex subtract(const Complex& z, const double r);
   friend Complex operator-(const Complex& z, const double r);

   friend Complex minus(const Complex& z);
   friend Complex operator-(const Complex& z);

   friend Complex multiply(const Complex& z1, const Complex& z2);
   friend Complex operator*(const Complex& z1, const Complex& z2);
   friend Complex multiply(const double r, const Complex& z);
   friend Complex operator*(const double r, const Complex& z);
   friend Complex multiply(const Complex& z, const double r);
   friend Complex operator*(const Complex& z, const double r);

   friend Complex divide(const Complex& z1, const Complex& z2);
   friend Complex operator/(const Complex& z1, const Complex& z2);
   friend Complex divide(const double r, const Complex& z);
   friend Complex operator/(const double r, const Complex& z);
   friend Complex divide(const Complex& z, const double r);
   friend Complex operator/(const Complex& z, const double r);


   //--------------------------------------------------------------------------
   // Special Functions
   //--------------------------------------------------------------------------
   Complex zExp() const;                  // e^z
   Complex zLog() const;                  // ln(z)
   Complex zPow(const Complex& t) const;  // z^t
   Complex zPow(const double r) const;    // z^r
   Complex zConj() const;                 // z.imag = -z.imag
   Complex zInv() const;                  // 1/z


   //--------------------------------------------------------------------------
   // Display Functions
   //--------------------------------------------------------------------------
   void showComplex(const int dp=2) const;
   void showPhasor(const int dp=2) const;
   friend std::ostream& operator<<(std::ostream& sout, const Complex& z);


   //--------------------------------------------------------------------------
   // Slot functions
   //--------------------------------------------------------------------------
   virtual bool setSlotImaginary(const Number* const msg);

private:
   double imag;   // imaginary component
};

#include "openeaagles/basic/Complex.inl"

}  // end namespace Basic
}  // end namespace Eaagles

#endif
