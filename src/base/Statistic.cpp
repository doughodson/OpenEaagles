
#include "openeaagles/base/Statistic.hpp"

#include <limits>

namespace oe {
namespace base {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Statistic,"Statistic")
EMPTY_SERIALIZER(Statistic)
EMPTY_DELETEDATA(Statistic)

Statistic::Statistic()
{
   STANDARD_CONSTRUCTOR()
   clear();
}

void Statistic::copyData(const Statistic& org, const bool)
{
   BaseClass::copyData(org);

   n = org.n;
   maximum = org.maximum;
   minimum = org.minimum;
   sum = org.sum;
   absSum = org.absSum;
   sumSq  = org.sumSq ; 
   value1 = org.value1;
}

//------------------------------------------------------------------------------
// clear() -- resets/clears object state
//------------------------------------------------------------------------------
void Statistic::clear()
{
  n = 0;
  maximum = -std::numeric_limits<double>::max();
  minimum =  std::numeric_limits<double>::max(); // DDH? why is both max, flip min/max?
  sum = 0.0;
  absSum = 0.0;
  sumSq  = 0.0;
  value1 = 0.0;
}

}
}

