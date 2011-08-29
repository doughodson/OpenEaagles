//------------------------------------------------------------------------------
// Sz2 class -- Second order transfer function
// ##### IN-WORK #####
//------------------------------------------------------------------------------
#ifndef __LinearSystem_Sz2_H_5E5ABB8B_B45B_48e4_8D8E_4EFF066B9C86__
#define __LinearSystem_Sz2_H_5E5ABB8B_B45B_48e4_8D8E_4EFF066B9C86__

#include "openeaagles/linearSys/SecondOrderTf.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  Sz2
// Base class:  Basic::Object -> ScalerFunc-> DiffEquation -> SecondOrderTf -> Sz2
//
// Description: Models a second order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coeffients.
//
// Transfer function:
//       N1*s*s + N2*s + N3
//       ------------------
//       D1*s*s + D2*s + D3
//
// Form name: Sz2
// Slots:
//    N1    <Number>     Coefficent N1 (default: 0)
//    n1    <Number>     Coefficent N1 (default: 0)
//
//    N2    <Number>     Coefficent N2 (default: 0)
//    n2    <Number>     Coefficent N2 (default: 0)
//
//    N3    <Number>     Coefficent N3 (default: 0)
//    n3    <Number>     Coefficent N3 (default: 0)
//
//    D1    <Number>     Coefficent D1 (default: 0)
//    d1    <Number>     Coefficent D1 (default: 0)
//
//    D2    <Number>     Coefficent D2 (default: 0)
//    d1    <Number>     Coefficent D2 (default: 0)
//
//    D3    <Number>     Coefficent D3 (default: 0)
//    d3    <Number>     Coefficent D3 (default: 0)
//
//
// GUID: {5E5ABB8B-B45B-48e4-8D8E-4EFF066B9C86}
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

#endif      /* __LinearSystem_Sz2_H_5E5ABB8B_B45B_48e4_8D8E_4EFF066B9C86__ */
