//------------------------------------------------------------------------------
// Numeric Operators:  Add, Subtract, Multiply, Divide
//------------------------------------------------------------------------------
#include "openeaagles/base/Operators.h"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Add,"+")
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Subtract,"-")
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Multiply,"*")
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Divide,"/")

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Add)
    "n2",       //  2nd number (first number is from Number)
    "n3",       //  3rd number
    "n4",       //  4th number
    "n5",       //  5th number
    "n6",       //  6th number
    "n7",       //  7th number
    "n8",       //  8th number
    "n9",       //  9th number
    "n10",      // 10th number
END_SLOTTABLE(Add)

//------------------------------------------------------------------------------
//  Map slot table to handles
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Add)
    // Accept an Number at any slot index ...
    Number* _msg = dynamic_cast<Number*>(obj);
    if (_msg != nullptr) { _ok = setSecondNumber(_msg); }
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------
Add::Add()
{
   STANDARD_CONSTRUCTOR()
   n2 = 0.0;
}

Subtract::Subtract()
{
   STANDARD_CONSTRUCTOR()
}

Multiply::Multiply()
{
   STANDARD_CONSTRUCTOR()
}

Divide::Divide()
{
   STANDARD_CONSTRUCTOR()
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Add::copyData(const Add& org, const bool)
{
   BaseClass::copyData(org);
   n2 = org.n2;
}
EMPTY_DELETEDATA(Add)

EMPTY_COPYDATA(Subtract)
EMPTY_DELETEDATA(Subtract)

EMPTY_COPYDATA(Multiply)
EMPTY_DELETEDATA(Multiply)

EMPTY_COPYDATA(Divide)
EMPTY_DELETEDATA(Divide)


//------------------------------------------------------------------------------
// getSlotByIndex() for Add
//------------------------------------------------------------------------------
Object* Add::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// operation() -- the operations
//------------------------------------------------------------------------------
void Add::operation()
{
    val = val + n2;
}

void Subtract::operation()
{
    val = val - n2;
}

void Multiply::operation()
{
    val = val * n2;
}

void Divide::operation()
{
    if (n2 != 0.0) val = (val / n2);  /* only if non-zero divisor */
}

//------------------------------------------------------------------------------
// serialize() -- print the value of this object to the output stream sout.
//------------------------------------------------------------------------------
std::ostream& Add::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName()  << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    indent(sout,i+j);
    sout << n2 << std::endl;

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}


std::ostream& Subtract::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName()  << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

std::ostream& Multiply::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName()  << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

std::ostream& Divide::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName()  << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// setSecondNumber() --
//------------------------------------------------------------------------------
bool Add::setSecondNumber(const Number* const ssnobj)
{
    if (ssnobj != nullptr) {
        n2 = ssnobj->getReal();
        operation();
    }
    return true;
}

} // End base namespace
}
