
#include <iostream>

#include "openeaagles/base/distributions/Uniform.hpp"
#include "openeaagles/base/Number.hpp"

namespace oe {
namespace base {

IMPLEMENT_SUBCLASS(Uniform,"Uniform")

// slot table for this class type
BEGIN_SLOTTABLE(Uniform)
  "min",            // 1
  "max",            // 2
END_SLOTTABLE(Uniform)
//  map slot table to handles
BEGIN_SLOT_MAP(Uniform)
  ON_SLOT(1, setSlotMin, Number)
  ON_SLOT(2, setSlotMax, Number)
END_SLOT_MAP()

EMPTY_DELETEDATA(Uniform)

//------------------------------------------------------------------------------
// slot table function
//------------------------------------------------------------------------------
Object* Uniform::getSlotByIndex(const int si)
{
  return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Uniform::Uniform()
{
  STANDARD_CONSTRUCTOR()
  min = 0.0;
  max  = 0.0;
}

//------------------------------------------------------------------------------
// copyData
//------------------------------------------------------------------------------
void Uniform::copyData(const Uniform& org, const bool cc)
{
  BaseClass::copyData(org);

  min = org.min;
  max = org.max;
}

//------------------------------------------------------------
//------------------------------------------------------------
double Uniform::draw()
{
  // uniform distribution over [min,max)
  const double u = drawHalfOpen();
  return min * (1 - u) + max * u;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool Uniform::setSlotMin(const Number* const x)
{
  bool ok = false;
  if(x != nullptr)
    ok = setMin(x->getDouble());
  return ok;
}

//------------------------------------------------------------
//------------------------------------------------------------
bool Uniform::setSlotMax(const Number* const x)
{
  bool ok = false;
  if(x != nullptr)
    ok = setMax(x->getDouble());
  return ok;
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Uniform::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
  int j = 0;
  if ( !slotsOnly ) {
//    indent(sout,i);
    sout << "( " << getFactoryName() << std::endl;
    j = 4;
  }

  // serialize base class
  BaseClass::serialize(sout,i+j,true);

  indent(sout,i+j);
  sout << "min: " << min << std::endl;
  indent(sout,i+j);
  sout << "max: " << max << std::endl;

  if ( !slotsOnly ) {
    indent(sout,i);
    sout << ")" << std::endl;
  }

  return sout;
}

}
}


