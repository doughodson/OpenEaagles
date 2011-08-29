//------------------------------------------------------------------------------
// SecondOrderTf class -- Second order transfer function
// ##### IN-WORK #####
//------------------------------------------------------------------------------
#ifndef __LinearSystem_SecondOrderTf_H_8F6B2AD4_67D6_427d_BB5C_7ED8BA22E0C5__
#define __LinearSystem_SecondOrderTf_H_8F6B2AD4_67D6_427d_BB5C_7ED8BA22E0C5__

#include "openeaagles/linearSys/DiffEquation.h"

namespace Eaagles {
namespace LinearSystem {

//------------------------------------------------------------------------------
// Class:  SecondOrderTf
// Base class:  Basic::Object -> ScalerFunc-> DiffEquation -> SecondOrderTf
//
// Description: Models a second order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coeffients.
//
// Transfer function:
//       N1*s*s + N2*s + N3
//       ------------------
//       D1*s*s + D2*s + D3
//
// GUID: {8F6B2AD4-67D6-427d-BB5C-7ED8BA22E0C5}
//------------------------------------------------------------------------------
class SecondOrderTf : public DiffEquation {
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

   // ScalerFunc class interface
   virtual bool isValid() const;

protected:
   // ScalerFunc class protected interface
   virtual void initialize();

private:
   void initData();  // Initialize our data

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
} // End Eaagles namespace

#endif      /* __LinearSystem_SecondOrderTf_H_8F6B2AD4_67D6_427d_BB5C_7ED8BA22E0C5__ */
