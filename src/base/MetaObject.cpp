
#include "openeaagles/base/MetaObject.hpp"

#include "openeaagles/base/SlotTable.hpp"

namespace oe {
namespace base {

MetaObject::MetaObject(
      const char* const cn,
      const char* const fn,
      const SlotTable* const slottbl,
      const MetaObject* const metaobj
   ) : slottable(slottbl), baseMetaObject(metaobj), count(0), mc(0), tc(0), cname(cn), fname(fn)
{}

const char* MetaObject::getClassName() const
{
    return cname;
}

const char* MetaObject::getFactoryName() const
{
    return fname;
}

}
}

