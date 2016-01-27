//------------------------------------------------------------------------------
// FirstOrderTf class -- General first order transfer function
//------------------------------------------------------------------------------
#ifndef __oe_linearSys_FirstOrderTf_H__
#define __oe_linearSys_FirstOrderTf_H__

#include "openeaagles/linearSys/DiffEquation.h"

namespace oe {
namespace linearSys {

//------------------------------------------------------------------------------
// Class:  FirstOrderTf
// Base class:  basic::Object -> ScalerFunc-> DiffEquation -> FirstOrderTf
//
// Description: Models a first order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coefficients.
//
// Transfer function:
//       N1*s + N2
//       ---------
//       D1*s + D2
//
//------------------------------------------------------------------------------
class FirstOrderTf : public DiffEquation {
    DECLARE_SUBCLASS(FirstOrderTf,DiffEquation)

public:
   FirstOrderTf();
   FirstOrderTf(const unsigned int rate, const LCreal n1, const LCreal n2, const LCreal d1, const LCreal d2);

   // Get Parameters
   LCreal getN1() const          { return n1; }
   LCreal getN2() const          { return n2; }
   LCreal getD1() const          { return d1; }
   LCreal getD2() const          { return d2; }

   // Set Parameters
   virtual bool setN1(const LCreal v);
   virtual bool setN2(const LCreal v);
   virtual bool setD1(const LCreal v);
   virtual bool setD2(const LCreal v);

   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();  // Initialize our data

   static const unsigned int ORDER = 2;

   // Parameters and initial values
   LCreal n1;
   LCreal n2;
   LCreal d1;
   LCreal d2;
};

} // End linearSys namespace
} // End oe namespace

#endif
