//------------------------------------------------------------------------------
// Sz2 class -- Second order transfer function
// ##### IN-WORK #####
//------------------------------------------------------------------------------
#ifndef __Eaagles_LinearSystem_Sz2_H__
#define __Eaagles_LinearSystem_Sz2_H__

#include "openeaagles/linearSys/SecondOrderTf.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  Sz2
// Base class:  Basic::Object -> ScalerFunc-> DiffEquation -> SecondOrderTf -> Sz2
//
// Description: Models a second order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coefficients.
//
// Transfer function:
//       N1*s*s + N2*s + N3
//       ------------------
//       D1*s*s + D2*s + D3
//
// Factory name: Sz2
// Slots:
//    N1    <Number>     Coefficient N1 (default: 0)
//    n1    <Number>     Coefficient N1 (default: 0)
//
//    N2    <Number>     Coefficient N2 (default: 0)
//    n2    <Number>     Coefficient N2 (default: 0)
//
//    N3    <Number>     Coefficient N3 (default: 0)
//    n3    <Number>     Coefficient N3 (default: 0)
//
//    D1    <Number>     Coefficient D1 (default: 0)
//    d1    <Number>     Coefficient D1 (default: 0)
//
//    D2    <Number>     Coefficient D2 (default: 0)
//    d1    <Number>     Coefficient D2 (default: 0)
//
//    D3    <Number>     Coefficient D3 (default: 0)
//    d3    <Number>     Coefficient D3 (default: 0)
//
//------------------------------------------------------------------------------
class Sz2 : public SecondOrderTf {
    DECLARE_SUBCLASS(Sz2,SecondOrderTf)

public:
   Sz2();

   // Set slot functions
   virtual bool setSlotN1(const Basic::Number* const msg);
   virtual bool setSlotN2(const Basic::Number* const msg);
   virtual bool setSlotN3(const Basic::Number* const msg);
   virtual bool setSlotD1(const Basic::Number* const msg);
   virtual bool setSlotD2(const Basic::Number* const msg);
   virtual bool setSlotD3(const Basic::Number* const msg);
};

} // End LinearSystem namespace
} // End Eaagles namespace

#endif
