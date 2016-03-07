//------------------------------------------------------------------------------
// SecondOrderTf class -- Second order transfer function
// ##### IN-WORK #####
//------------------------------------------------------------------------------
#ifndef __oe_linearsystem_SecondOrderTf_H__
#define __oe_linearsystem_SecondOrderTf_H__

#include "openeaagles/linearsystem/DiffEquation.h"

namespace oe {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  SecondOrderTf
// Base class:  base::Object -> ScalerFunc-> DiffEquation -> SecondOrderTf
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
      const double n1, const double n2, const double n3,
      const double d1, const double d2, const double d3);

   // Get Parameters
   double getN1() const          { return n1; }
   double getN2() const          { return n2; }
   double getN3() const          { return n3; }
   double getD1() const          { return d1; }
   double getD2() const          { return d2; }
   double getD3() const          { return d3; }

   // Set Parameters
   virtual bool setN1(const double v);
   virtual bool setN2(const double v);
   virtual bool setN3(const double v);
   virtual bool setD1(const double v);
   virtual bool setD2(const double v);
   virtual bool setD3(const double v);

   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();

   static const unsigned int ORDER = 6;

   // Parameters and initial values
   double n1;
   double n2;
   double n3;
   double d1;
   double d2;
   double d3;
};

} // End linearsystem namespace
} // End oe namespace

#endif
