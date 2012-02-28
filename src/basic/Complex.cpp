//==============================================================================
// Complex
//==============================================================================

#include "openeaagles/basic/Complex.h"
#include "openeaagles/basic/Number.h"

#include <iostream>
#include <iomanip>
#include <cmath>

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Complex,"Complex")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Complex)
   "real",      // 1: real component
   "imag",      // 2: imaginary component
END_SLOTTABLE(Complex)

// Map slot table to handles 
BEGIN_SLOT_MAP(Complex)
   ON_SLOT(1,setSlotReal,Number)
   ON_SLOT(2,setSlotImaginary,Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------

Complex::Complex()
{
   STANDARD_CONSTRUCTOR()
   real = 0.0;
   imag = 0.0;
}

Complex::Complex(const double r)
{
   STANDARD_CONSTRUCTOR()
   real = r;
   imag = 0.0;
}

Complex::Complex(const double r, const double i)
{
   STANDARD_CONSTRUCTOR()
   real = r;
   imag = i;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Complex::copyData(const Complex& org, const bool)
{
   BaseClass::copyData(org);
   
   real = org.real;
   imag = org.imag;
}

//------------------------------------------------------------------------------
//deleteData() -- delete member data
//------------------------------------------------------------------------------
void Complex::deleteData()
{
}

//------------------------------------------------------------------------------
// Output Functions
//------------------------------------------------------------------------------
void Complex::showComplex(const int decpnt) const
{
    std::cout << std::setprecision(decpnt) << std::setiosflags(std::ios::fixed);
    std::cout << real << " + j*" << imag << std::endl;
}

void Complex::showPhasor(const int decpnt) const
{
    std::cout << std::setprecision(decpnt) << std::setiosflags(std::ios::fixed);
    std::cout << "<" << this->getMag() << ", " << this->getArg() << ">" << std::endl;
}

std::ostream& operator<<(std::ostream& sout, const Complex& z)
{
    const int decpnt = 2;
    sout << std::setprecision(decpnt) << std::setiosflags(std::ios::fixed);
    sout << z.getReal() << " + j*" << z.getImag();
    return sout;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Complex::setSlotReal(const Number* const msg)
{
    double value = msg->getDouble();
    bool ok = setReal( value );
    if (!ok) std::cerr << "Complex::setSlotReal: invalid entry(" << value << ")" << std::endl;
    return ok;
}

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
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << "real:   " << getReal() << std::endl;

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

