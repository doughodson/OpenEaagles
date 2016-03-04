//------------------------------------------------------------------------------
// Classes:  Add, Subtract, Multiply, Divide
//------------------------------------------------------------------------------
#ifndef __oe_base_Operators_H__
#define __oe_base_Operators_H__

#include "openeaagles/base/Number.h"

namespace oe {
namespace basic {

//------------------------------------------------------------------------------
// Class:  Add
// Description:  Class for adding Numbers.
//
// Factory name: "+"
// Slots:
//    n2  <Number>  ! 2nd number (first number is from Number) (default: 0.0)
//    n3  <Number>  ! 3rd number
//    n4  <Number>  ! 4th number
//    n5  <Number>  ! 5th number
//    n6  <Number>  ! 6th number
//    n7  <Number>  ! 7th number
//    n8  <Number>  ! 8th number
//    n9  <Number>  ! 9th number
//    n10  <Number> ! 10th number
//
//        ( + val n2 ) gives val plus n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( + ).
//------------------------------------------------------------------------------
class Add : public Number {
    DECLARE_SUBCLASS(Add,Number)

public:
    Add();

    LCreal getSecondNumber() const              { return n2; }

    virtual bool setSecondNumber(const Number* const ssnobj);
    virtual void operation();

protected:
    LCreal n2;
};

//------------------------------------------------------------------------------
// Class:  Subtract
// Description:  Class for subtracting Numbers.
//
// Factory name: "-"
//
//        ( - val n2 ) gives val minus n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( - ).
//------------------------------------------------------------------------------
class Subtract : public Add {
    DECLARE_SUBCLASS(Subtract,Add)

public:
    Subtract();
    void operation() override;
};

//------------------------------------------------------------------------------
// Class:  Multiply
// Description:  Class for multiplying Numbers.
//
// Factory name: "*"
//
//        ( * val n2 ) gives val multiplied by n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( * ).
//------------------------------------------------------------------------------
class Multiply : public Add {
    DECLARE_SUBCLASS(Multiply,Add)

public:
    Multiply();
    void operation() override;
};

//------------------------------------------------------------------------------
// Class:  Divide
// Description:  Class for dividing Numbers.
//
// Factory name: "/"
//
//        ( / val n2 ) gives val divided by n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( / ).
//          Note:  In Divide, a zero divisor causes operation() to do nothing.
//------------------------------------------------------------------------------
class Divide : public Add {
    DECLARE_SUBCLASS(Divide,Add)

public:
    Divide();

    void operation() override;
};

} // End basic namespace
} // End oe namespace

#endif
