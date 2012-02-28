//------------------------------------------------------------------------------
// Class: Complex
// inline functions
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_Complex_Inline__
#define __Eaagles_Basic_Complex_Inline__

//------------------------------------------------------------------------------
// Unary Arithmetic Functions and Overloaded Operators
//------------------------------------------------------------------------------

inline const Complex& Complex::add(const Complex& z)         
{
   real += z.getReal();
   imag += z.getImag();
   return *this;
}

inline const Complex& Complex::operator+=(const Complex& z)         
{
   real += z.getReal();
   imag += z.getImag();
   return *this;
}

//------------------------------------------------------------------------------
inline const Complex& Complex::add(const double r)         
{
   real += r;
   return *this;
}

inline const Complex& Complex::operator+=(const double r)         
{
   real += r;
   return *this;
}

//------------------------------------------------------------------------------
inline const Complex& Complex::subtract(const Complex& z)         
{
   real -= z.getReal();
   imag -= z.getImag();
   return *this;
}

inline const Complex& Complex::operator-=(const Complex& z)
{
   real -= z.getReal();
   imag -= z.getImag();
   return *this;
}

//------------------------------------------------------------------------------
inline const Complex& Complex::subtract(const double r)         
{
   real -= r;
   return *this;
}

inline const Complex& Complex::operator-=(const double r)   
{
   real -= r;
   return *this;
}

//------------------------------------------------------------------------------
inline const Complex& Complex::multiply(const Complex& z)         
{
   double r1 = real;
   double i1 = imag;
   double r2 = z.getReal();
   double i2 = z.getImag();
   real = r1*r2 - i1*i2;
   imag = r1*i2 + i1*r2;
   return *this;
}

inline const Complex& Complex::operator*=(const Complex& z)         
{
   this->multiply(z);
   return *this;
}

//------------------------------------------------------------------------------
inline const Complex& Complex::multiply(const double r)         
{
   real *= r;
   imag *= r;
   return *this;
}

inline const Complex& Complex::operator*=(const double r)         
{
   real *= r;
   imag *= r;
   return *this;
}

//------------------------------------------------------------------------------
inline const Complex& Complex::divide(const Complex& z)         
{
   if (z != Complex(0.0)) {
      double r1 = real;
      double i1 = imag;
      double r2 = z.getReal();
      double i2 = z.getImag();
      double d  = r2*r2 + i2*i2;
      real      = (r1*r2 + i1*i2) / d;
      imag      = (i1*r2 - r1*i2) / d;
   }
   return *this;
}

inline const Complex& Complex::operator/=(const Complex& z)         
{
   this->divide(z);
   return *this;
}

//------------------------------------------------------------------------------
inline const Complex& Complex::divide(const double r)         
{
   if (r != 0.0) {
      real /= r;
      imag /= r;
   }
   return *this;
}

inline const Complex& Complex::operator/=(const double r)         
{
   this->divide(r);
   return *this;
}


//-----------------------------------------------------------------------------
// Binary Arithmetic Functions and Overloaded Operators
//-----------------------------------------------------------------------------

inline Complex add(const Complex& z1, const Complex& z2)  
{
   Complex t;
   t.setReal(z1.getReal() + z2.getReal());
   t.setImag(z1.getImag() + z2.getImag());
   return t;
}

inline Complex operator+(const Complex& z1, const Complex& z2)  
{
   return add(z1, z2);
}

//------------------------------------------------------------------------------
inline Complex add(const double r, const Complex& z)  
{
   Complex t;
   t.setReal(r + z.getReal());
   t.setImag(    z.getImag());
   return t;
}

inline Complex operator+(const double r, const Complex& z)  
{
   return add(r, z);
}

//------------------------------------------------------------------------------
inline Complex add(const Complex& z, const double r)  
{
   Complex t;
   t.setReal(z.getReal() + r);
   t.setImag(z.getImag());
   return t;
}

inline Complex operator+(const Complex& z, const double r)  
{
   return add(z, r);
}

//------------------------------------------------------------------------------
inline Complex subtract(const Complex& z1, const Complex& z2)  
{
   Complex t;
   t.setReal(z1.getReal() - z2.getReal());
   t.setImag(z1.getImag() - z2.getImag());
   return t;
}

inline Complex operator-(const Complex& z1, const Complex& z2)  
{
   return subtract(z1, z2);
}

//------------------------------------------------------------------------------
inline Complex subtract(const double r, const Complex& z)     
{
   Complex t;
   t.setReal(r   - z.getReal());
   t.setImag(0.0 - z.getImag());
   return t;
}

inline Complex operator-(const double r, const Complex& z)  
{
   return subtract(r, z);
}

//------------------------------------------------------------------------------
inline Complex subtract(const Complex& z, const double r)     
{
   Complex t;
   t.setReal(z.getReal() - r);
   t.setImag(z.getImag());
   return t;
}

inline Complex operator-(const Complex& z, const double r)     
{
   return subtract(z, r);
}

//------------------------------------------------------------------------------
inline Complex minus(const Complex& z)
{
   Complex t;
   t.setReal(-z.getReal());
   t.setImag(-z.getImag());
   return t;
}

inline Complex operator-(const Complex& z)
{
   return minus(z);
}

//------------------------------------------------------------------------------
inline Complex multiply(const Complex& z1, const Complex& z2)  
{
   Complex t;
   double r1 = z1.getReal();
   double i1 = z1.getImag();
   double r2 = z2.getReal();
   double i2 = z2.getImag();
   t.setReal(r1*r2 - i1*i2);
   t.setImag(r1*i2 + i1*r2);
   return t;
}

inline Complex operator*(const Complex& z1, const Complex& z2)  
{
   return multiply(z1, z2); 
}

//------------------------------------------------------------------------------
inline Complex multiply(const double r, const Complex& z)
{
   Complex t;
   t.setReal(r * z.getReal());
   t.setImag(r * z.getImag());
   return t;
}

inline Complex operator*(const double r, const Complex& z)
{
   return multiply(r, z);
}

//------------------------------------------------------------------------------
inline Complex multiply(const Complex& z, const double r)
{
   Complex t;
   t.setReal(z.getReal() * r);
   t.setImag(z.getImag() * r);
   return t;
}

inline Complex operator*(const Complex& z, const double r)
{
   return multiply(z, r);
}

//------------------------------------------------------------------------------
inline Complex divide(const Complex& z1, const Complex& z2)  
{
   Complex t;
   if (z2 != Complex(0.0)) {
      double r1  = z1.getReal();
      double i1  = z1.getImag();
      double r2  = z2.getReal();
      double i2  = z2.getImag();
      double den = r2*r2 + i2*i2;
      t.setReal((r1*r2 + i1*i2) / den);
      t.setImag((i1*r2 - r1*i2) / den);
   }
   return t;
}

inline Complex operator/(const Complex& z1, const Complex& z2)  
{
   return divide(z1, z2);
}

//------------------------------------------------------------------------------
inline Complex divide(const double r, const Complex& z)
{
   Complex t;
   if (z != Complex(0.0)) {
      double r2  = z.getReal();
      double i2  = z.getImag();
      double den = r2*r2 + i2*i2;
      t.setReal(+r*r2 / den);
      t.setImag(-r*i2 / den);
   }
   return t;
}

inline Complex operator/(const double r, const Complex& z)
{
   return divide(r, z);
}

//------------------------------------------------------------------------------
inline Complex divide(const Complex& z, const double r)
{
   Complex t;
   if (r != 0.0) {
      t.setReal(z.getReal() / r);
      t.setImag(z.getImag() / r);
   }
   return t;
}

inline Complex operator/(const Complex& z, const double r)
{
   return divide(z, r);
}

//-----------------------------------------------------------------------------
// Access Functions
//-----------------------------------------------------------------------------

inline double Complex::getReal() const
{
    return real;
}

inline double Complex::getImag() const
{
    return imag;
}

inline double Complex::getMag() const
{
    return std::sqrt(real*real + imag*imag);
}

inline double Complex::getArg() const
{
    return std::atan2(imag, real);
}

inline bool Complex::setReal(const double r)
{
    real = r;
    return true;
}

inline bool Complex::setImag(const double i)
{
    imag = i;
    return true; 
}


////////////////////////////////////////////////////////////////////////////////
// Comparison Functions

inline bool Complex::operator!=(const Complex& z) const
{
    if ((real != z.getReal()) || (imag != z.getImag()))
        return true;

    return false;
}

inline bool Complex::operator==(const Complex& z) const
{
    if ((real != z.getReal()) || (imag != z.getImag()))
        return false;

    return true;
}


////////////////////////////////////////////////////////////////////////////////
// Special Functions

inline Complex Complex::zExp() const
{
    Complex t;
    double x = std::exp(real);
    t.setReal(x*std::cos(imag));
    t.setImag(x*std::sin(imag));
    return t; 
}

inline Complex Complex::zLog() const
{
   Complex t(*this);
   if (t != Complex(0.0)) {
      double re = std::log(real*real + imag*imag) / 2.0;
      double im = std::atan2(imag, real);
      t.setReal(re);
      t.setImag(im);
   }
   return t;
}

inline Complex Complex::zPow(const Complex& w) const
{
   Complex t(*this);
   t = w * t.zLog();
   return t.zExp();
}

inline Complex Complex::zPow(const double r) const
{
   Complex t(*this);
   t = r * t.zLog();
   return t.zExp();
}

inline Complex Complex::zConj() const
{
   Complex t(*this);
   t.setImag(-imag);
   return t;
}

inline Complex Complex::zInv() const
{
   Complex t(*this);
   if (t != Complex(0.0)) {
      double den = real*real + imag*imag;
      t.setImag(-imag);
      t.divide(den);
   }
   return t;
}

#endif
