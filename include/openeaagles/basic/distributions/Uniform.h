
#ifndef __oe_basic_Uniform_H__
#define __oe_basic_Uniform_H__

#include "../Rng.h"

namespace oe {
namespace basic {

class Number;

class Uniform: public Rng
{
  DECLARE_SUBCLASS(Uniform, Rng)

public:

  Uniform();

  double draw() override;

  bool setMin(const double x)                           { min = x; return true; }
  double getMin() const                                 { return min; }
  bool setSlotMin(const Number* const);

  bool setMax(const double x)                           { max = x; return true; }
  double getMax() const                                 { return max; }
  bool setSlotMax(const Number* const);

protected:

  double min;
  double max;


};

} // End basic namespace
} // End oe namespace

#endif
