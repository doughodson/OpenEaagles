//==============================================================================
// Complex
//==============================================================================

#include "openeaagles/basic/Complex.h"
#include "openeaagles/basic/Number.h"

#include <iostream>
#include <iomanip>
#include <cmath>

namespace oe {
namespace Basic {

IMPLEMENT_SUBCLASS(Complex,"Complex")
EMPTY_DELETEDATA(Complex)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Complex)
   "imag",      // 1: imaginary component
END_SLOTTABLE(Complex)

// Map slot table to handles
BEGIN_SLOT_MAP(Complex)
   ON_SLOT(1,setSlotImaginary,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------

Complex::Complex()
{
   STANDARD_CONSTRUCTOR()
   imag = 0.0;
}

Complex::Complex(const double r) : Number(r)
{
   STANDARD_CONSTRUCTOR()
   imag = 0.0;
}

Complex::Complex(const double r, const double i) : Number(r)
{
   STANDARD_CONSTRUCTOR()
   imag = i;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Complex::copyData(const Complex& org, const bool)
{
   BaseClass::copyData(org);

   imag = org.imag;
}

//------------------------------------------------------------------------------
// Output Functions
//------------------------------------------------------------------------------
void Complex::showComplex(const int decpnt) const
{
   std::ios_base::fmtflags oldFmtFlgs =  std::cout.flags();
   std::streamsize oldprec = std::cout.precision();

   std::cout << std::setprecision(decpnt) << std::setiosflags(std::ios::fixed);
   std::cout << getReal() << " + j*" << getImag() << std::endl;

   std::cout.setf(oldFmtFlgs);
   std::cout << std::setprecision(oldprec);
}

void Complex::showPhasor(const int decpnt) const
{
   std::ios_base::fmtflags oldFmtFlgs =  std::cout.flags();
   std::streamsize oldprec = std::cout.precision();

   std::cout << std::setprecision(decpnt) << std::setiosflags(std::ios::fixed);
   std::cout << "<" << this->getMag() << ", " << this->getArg() << ">" << std::endl;

   std::cout.setf(oldFmtFlgs);
   std::cout << std::setprecision(oldprec);
}

std::ostream& operator<<(std::ostream& sout, const Complex& z)
{
   std::ios_base::fmtflags oldFmtFlgs =  sout.flags();
   std::streamsize oldprec = sout.precision();

   const int decpnt = 2;
   sout << std::setprecision(decpnt) << std::setiosflags(std::ios::fixed);
   sout << z.getReal() << " + j*" << z.getImag();

   sout.setf(oldFmtFlgs);
   sout << std::setprecision(oldprec);

   return sout;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Complex::setSlotImaginary(const Number* const msg)
{
    double value = msg->getDouble();
    bool ok = setImag( value );
    if (!ok) std::cerr << "Complex::setSlotImaginary: invalid entry(" << value << ")" << std::endl;
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Rgb
//------------------------------------------------------------------------------
Object* Complex::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Complex::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "value:   " << getReal() << std::endl;

    indent(sout,i+j);
    sout << "imag: " << getImag() << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}  // end namespace Basic
}  // end namespace Eaagles

