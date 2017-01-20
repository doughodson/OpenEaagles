
#include "openeaagles/base/Metadata.hpp"

#include "openeaagles/base/SlotTable.hpp"

namespace oe {
namespace base {

Metadata::Metadata(
      const char* const cn,
      const char* const fn,
      const SlotTable* const slottbl,
      const Metadata* const metadata
   ) : cname(cn), fname(fn), slottable(slottbl), baseMetadata(metadata), count(0), mc(0), tc(0)
{}

const char* Metadata::getClassName() const
{
    return cname;
}

const char* Metadata::getFactoryName() const
{
    return fname;
}

}
}

