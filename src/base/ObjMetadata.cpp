
#include "openeaagles/base/ObjMetadata.hpp"

#include "openeaagles/base/SlotTable.hpp"

namespace oe {
namespace base {

ObjMetadata::ObjMetadata(
      const unsigned int ci,
      const char* const cn,
      const char* const fn,
      const SlotTable* const slottbl,
      const ObjMetadata* const metadata
   ) : classIndex(ci), cname(cn), fname(fn), slottable(slottbl), baseMetadata(metadata), count(0), mc(0), tc(0)
{
}

}
}

