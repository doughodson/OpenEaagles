
#include "openeaagles/base/ClassMetadata.hpp"

#include "openeaagles/base/SlotTable.hpp"

namespace oe {
namespace base {

ClassMetadata::ClassMetadata(
      const unsigned int ci,
      const char* const cn,
      const char* const fn,
      const SlotTable* const p,
      const ClassMetadata* const bs
   ) : classIndex(ci), cname(cn), fname(fn), st(p), bstatic(bs), count(0), mc(0), tc(0)
{
}

ClassMetadata& ClassMetadata::operator=(const ClassMetadata&)
{
   return *this;
}


}
}

