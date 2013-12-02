//------------------------------------------------------------------------------
// mapsFormFunc()
//------------------------------------------------------------------------------
#include "openeaagles/maps/mapsFF.h"

#include "openeaagles/maps/rpfMap/rpfMapFF.h"

namespace Eaagles {
namespace Maps {

Basic::Object* mapsFormFunc(const char* formname)
{
    Basic::Object* newform = Rpf::rpfMapFormFunc(formname);
    return newform;
}

} // end Maps namespace
} // end Eaagles namespace

