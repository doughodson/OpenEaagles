
#include <iostream>

#include "openeaagles/basic/distributions/Exponential.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Basic {

IMPLEMENT_SUBCLASS(Exponential,"Exponential")

// slot table for this class type
BEGIN_SLOTTABLE(Exponential)
  "lambda",            // 1
  "gamma",             // 2
END_SLOTTABLE(Exponential)
//  map slot table to handles
BEGIN_SLOT_MAP(Exponential)
  ON_SLOT(1, setSlotLambda, Number)
  ON_SLOT(2, setSlotGamma, Number)
END_SLOT_MAP()

EMPTY_DELETEDATA(Exponential)

//------------------------------------------------------------------------------
// slot table function
//------------------------------------------------------------------------------
Object* Exponential::getSlotByIndex(const int si)
{
  return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Exponential::Exponential()
{
  STANDARD_CONSTRUCTOR()
  lambda = 1.0;
  gamma = 0.0;
}

//------------------------------------------------------------------------------
// copyData
//------------------------------------------------------------------------------
void Exponential::copyData(const Exponential& org, const bool cc)
{
  BaseClass::copyData(org);
  lambda = org.lambda;
  gamma = org.gamma;
}

//------------------------------------------------------------
// draw
//------------------------------------------------------------
double Exponential::draw()
{
  // draw a random value in the open interval (0,1)
  const double x = drawOpen();
  const double z = (-1/lambda) * std::log(x);
  return gamma + z;
}

//------------------------------------------------------------
// setSlotLambda
//------------------------------------------------------------
bool Exponential::setSlotLambda(const Number* const x)
{
  bool ok = false;
  if(x != nullptr)
    ok = setLambda(x->getDouble());
  return ok;
}

//------------------------------------------------------------
// setSlotGamma
//------------------------------------------------------------
bool Exponential::setSlotGamma(const Number* const x)
{
  bool ok = false;
  if(x != nullptr)
    ok = setGamma(x->getDouble());
  return ok;
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Exponential::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
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
  sout << "lambda: " << lambda << std::endl;
  indent(sout,i+j);
  sout << "gamma: " << gamma << std::endl;

  if ( !slotsOnly ) {
    indent(sout,i);
    sout << ")" << std::endl;
  }

  return sout;
}


} // End Basic namespace
} // End Eaagles namespace


