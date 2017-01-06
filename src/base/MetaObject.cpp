
#include "openeaagles/base/MetaObject.hpp"

#include "openeaagles/base/SlotTable.hpp"

namespace oe {
namespace base {

MetaObject::MetaObject(
      const unsigned int ci,
      const char* const cn,
      const char* const fn,
      const SlotTable* const p,
      const MetaObject* const bs
   ) : classIndex(ci), cname(cn), fname(fn), st(p), bstatic(bs), count(0), mc(0), tc(0)
{
}

MetaObject& MetaObject::operator=(const MetaObject&)
{
   return *this;
}


}
}

