
#include "openeaagles/base/ObjMetadata.hpp"

#include "openeaagles/base/SlotTable.hpp"

namespace oe {
namespace base {

ObjMetadata::ObjMetadata(
      const char* const cn,
      const char* const fn,
      const SlotTable* const slottbl,
      const ObjMetadata* const metadata
   ) : cname(cn), fname(fn), slottable(slottbl), baseMetadata(metadata), count(0), mc(0), tc(0)
{}

const char* ObjMetadata::getClassName() const
{
    return cname;
}

const char* ObjMetadata::getFactoryName() const
{
    return fname;
}

}
}

