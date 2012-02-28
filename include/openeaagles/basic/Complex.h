//==============================================================================
// Class: Complex
//==============================================================================
#ifndef __Eaagles_Basic_Complex_H__
#define __Eaagles_Basic_Complex_H__

#include "openeaagles/basic/Object.h"

namespace Eaagles {
namespace Basic {
   class Number;

//==============================================================================
// Class: Complex
// Description:  Generic complex number class 
// Form name: Complex
//==============================================================================
class Complex : public Object {
   DECLARE_SUBCLASS(Complex, Object)

public: 
   //--------------------------------------------------------------------------
   // Constructors
   //--------------------------------------------------------------------------
   Complex();                                // default constructor
   Complex(const double r);                  // assignment constructor 
   Complex(const double r, const double i);  // assignment constructor 

   //--------------------------------------------------------------------------
   // Unary Arithmetic Functions and Overloaded Operators
   //--------------------------------------------------------------------------
   const Complex& add(const Complex& z);               // this += z 
   const Complex& operator+=(const Complex& z);        // this += z 
   const Complex& add(const double r);                 // this += r 
   const Complex& operator+=(const double r);          // this += r 

   const Complex& subtract(const Complex& z);          // this += z
   const Complex& operator-=(const Complex& z);        // this -= z
   const Complex& subtract(const double r);            // this -= r 
   const Complex& operator-=(const double r);          // this -= r 

   const Complex& multiply(const Complex& z);          // this *= z  
   const Complex& operator*=(const Complex& z);        // this *= z
   const Complex& multiply(const double r);            // this *= r
   const Complex& operator*=(const double r);          // this *= r

   const Complex& divide(const Complex& z);            // this /= z 
   const Complex& operator/=(const Complex& z);        // this /= z
   const Complex& divide(const double r);              // this /= r 
   const Complex& operator/=(const double r);          // this /= r


   //--------------------------------------------------------------------------
   // Binary Arithmetic Functions and Overloaded Operators
   //--------------------------------------------------------------------------
   friend Complex add(const Complex& z1, const Complex& z2);         // rhs = z1 + z2
   friend Complex operator+(const Complex& z1, const Complex& z2);   // rhs = z1 + z2
   friend Complex add(const double r, const Complex& z);             // rhs =  r + z
   friend Complex operator+(const double r, const Complex& z);       // rhs =  r + z
   friend Complex add(const Complex& z, const double r);             // rhs =  z + r
   friend Complex operator+(const Complex& z, const double r);       // rhs =  z + r 

   friend Complex subtract(const Complex& z1, const Complex& z2);    // rhs = z1 - z2
   friend Complex operator-(const Complex& z1, const Complex& z2);   // rhs = z1 - z2
   friend Complex subtract(const double r, const Complex& z);        // rhs =  r - z
   friend Complex operator-(const double r, const Complex& z);       // rhs =  r - z 
   friend Complex subtract(const Complex& z, const double r);        // rhs =  z - r
   friend Complex operator-(const Complex& z, const double r);       // rhs =  z - r 

   friend Complex minus(const Complex& z);                           // rhs = -z
   friend Complex operator-(const Complex& z);                       // rhs = -z

   friend Complex multiply(const Complex& z1, const Complex& z2);    // rhs = z1 * z2
   friend Complex operator*(const Complex& z1, const Complex& z2);   // rhs = z1 * z2
   friend Complex multiply(const double r, const Complex& z);        // rhs =  r * z
   friend Complex operator*(const double r, const Complex& z);       // rhs =  r * z
   friend Complex multiply(const Complex& z, const double r);        // rhs =  z * r 
   friend Complex operator*(const Complex& z, const double r);       // rhs =  z * r 

   friend Complex divide(const Complex& z1, const Complex& z2);      // rhs = z1 / z2
   friend Complex operator/(const Complex& z1, const Complex& z2);   // rhs = z1 / z2
   friend Complex divide(const double r, const Complex& z);          // rhs =  r / z
   friend Complex operator/(const double r, const Complex& z);       // rhs =  r / z
   friend Complex divide(const Complex& z, const double r);          // rhs =  z / r 
   friend Complex operator/(const Complex& z, const double r);       // rhs =  z / r 


   //--------------------------------------------------------------------------
   // Access Functions 
   //--------------------------------------------------------------------------
   double getReal() const;       // returns real part
   double getImag() const;       // returns imaginary part
   double getMag() const;        // returns phasor magnitude 
   double getArg() const;        // returns phasor argument

   bool setReal(const double r); // real = r;
   bool setImag(const double i); // imag = i;

   //--------------------------------------------------------------------------
   // Information Functions 
   //--------------------------------------------------------------------------
   bool operator==(const Complex& z) const;  // this == z returns true
   bool operator!=(const Complex& z) const;  // this != z returns true

   //--------------------------------------------------------------------------
   // Display Functions 
   //--------------------------------------------------------------------------
   void showComplex(const int dp=2) const;
   void showPhasor(const int dp=2) const;
   friend std::ostream& operator<<(std::ostream& sout, const Complex& z);

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
   // Slot functions
   //--------------------------------------------------------------------------
   virtual bool setSlotReal(const Number* const msg);
   virtual bool setSlotImaginary(const Number* const msg);

private:
   double real;   // real component
   double imag;   // imaginary component
};

#include "openeaagles/basic/Complex.inl"

}  // end namespace Basic
}  // end namespace Eaagles

#endif
