//------------------------------------------------------------------------------
// Sz1 class -- First order transfer function
//------------------------------------------------------------------------------
#ifndef __Eaagles_LinearSystem_Sz1_H__
#define __Eaagles_LinearSystem_Sz1_H__

#include "openeaagles/linearSys/FirstOrderTf.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  Sz1
// Base class:  Basic::Object -> ScalerFunc-> DiffEquation -> FirstOrderTf -> Sz1
//
// Description: Models a first order continuous transfer function (see FirstOrderTf)
//
// Transfer function:
//       N1*s + N2
//       ---------
//       D1*s + D2
//
// Form name: Sz1
// Slots:
//    N1    <Number>     Coefficent N1 (default: 0)
//    n1    <Number>     Coefficent N1 (default: 0)
//
//    N2    <Number>     Coefficent N2 (default: 0)
//    n2    <Number>     Coefficent N2 (default: 0)
//
//    D1    <Number>     Coefficent D1 (default: 0)
//    d1    <Number>     Coefficent D1 (default: 0)
//
//    D2    <Number>     Coefficent D2 (default: 0)
//    d1    <Number>     Coefficent D2 (default: 0)
//
//------------------------------------------------------------------------------
class Sz1 : public FirstOrderTf {
    DECLARE_SUBCLASS(Sz1,FirstOrderTf)

public:
   Sz1();

   // Set slot functions
   virtual bool setSlotN1(const Basic::Number* const msg);
   virtual bool setSlotN2(const Basic::Number* const msg);
   virtual bool setSlotD1(const Basic::Number* const msg);
   virtual bool setSlotD2(const Basic::Number* const msg);
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif
