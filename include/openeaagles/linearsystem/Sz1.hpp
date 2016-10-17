
#ifndef __oe_linearsystem_Sz1_H__
#define __oe_linearsystem_Sz1_H__

#include "openeaagles/linearsystem/FirstOrderTf.hpp"

namespace oe {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  Sz1
//
// Description: Models a first order continuous transfer function (see FirstOrderTf)
//
// Transfer function:
//       N1*s + N2
//       ---------
//       D1*s + D2
//
// Factory name: Sz1
// Slots:
//    N1    <Number>     Coefficient N1 (default: 0)
//    n1    <Number>     Coefficient N1 (default: 0)
//
//    N2    <Number>     Coefficient N2 (default: 0)
//    n2    <Number>     Coefficient N2 (default: 0)
//
//    D1    <Number>     Coefficient D1 (default: 0)
//    d1    <Number>     Coefficient D1 (default: 0)
//
//    D2    <Number>     Coefficient D2 (default: 0)
//    d1    <Number>     Coefficient D2 (default: 0)
//
//------------------------------------------------------------------------------
class Sz1 : public FirstOrderTf
{
    DECLARE_SUBCLASS(Sz1, FirstOrderTf)

public:
   Sz1();

   // Set slot functions
   virtual bool setSlotN1(const base::Number* const msg);
   virtual bool setSlotN2(const base::Number* const msg);
   virtual bool setSlotD1(const base::Number* const msg);
   virtual bool setSlotD2(const base::Number* const msg);
};

}
}

#endif
