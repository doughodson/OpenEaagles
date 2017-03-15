
#ifndef __oe_models_Bullseye_H__
#define __oe_models_Bullseye_H__

#include "openeaagles/models/navigation/Steerpoint.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Bullseye
// Description: Generic Bullseye, used as a reference point
//
// Factory name: Bullseye
//------------------------------------------------------------------------------
class Bullseye : public Steerpoint
{
   DECLARE_SUBCLASS(Bullseye, Steerpoint)

public:
    Bullseye();
};

}
}

#endif
