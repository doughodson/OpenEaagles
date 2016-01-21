//------------------------------------------------------------------------------
// SecondOrderTf class -- Second order transfer function
// ##### IN-WORK #####
//------------------------------------------------------------------------------
#ifndef __oe_LinearSystem_SecondOrderTf_H__
#define __oe_LinearSystem_SecondOrderTf_H__

#include "openeaagles/linearSys/DiffEquation.h"

namespace oe {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  SecondOrderTf
// Base class:  basic::Object -> ScalerFunc-> DiffEquation -> SecondOrderTf
//
// Description: Models a second order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coefficients.
//
// Transfer function:
//       N1*s*s + N2*s + N3
//       ------------------
//       D1*s*s + D2*s + D3
//
//------------------------------------------------------------------------------
class SecondOrderTf : public DiffEquation
{
    DECLARE_SUBCLASS(SecondOrderTf,DiffEquation)

public:
   SecondOrderTf();
   SecondOrderTf(const unsigned int rate,
      const LCreal n1, const LCreal n2, const LCreal n3,
      const LCreal d1, const LCreal d2, const LCreal d3);

   // Get Parameters
   LCreal getN1() const          { return n1; }
   LCreal getN2() const          { return n2; }
   LCreal getN3() const          { return n3; }
   LCreal getD1() const          { return d1; }
   LCreal getD2() const          { return d2; }
   LCreal getD3() const          { return d3; }

   // Set Parameters
   virtual bool setN1(const LCreal v);
   virtual bool setN2(const LCreal v);
   virtual bool setN3(const LCreal v);
   virtual bool setD1(const LCreal v);
   virtual bool setD2(const LCreal v);
   virtual bool setD3(const LCreal v);

   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();

   static const unsigned int ORDER = 6;

   // Parameters and initial values
   LCreal n1;
   LCreal n2;
   LCreal n3;
   LCreal d1;
   LCreal d2;
   LCreal d3;
};

} // End LinearSystem namespace
} // End oe namespace

#endif
