#include "openeaagles/sensors/Tws.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Powers.h"
#include "openeaagles/basic/units/Times.h"
#include "openeaagles/basic/units/Frequencies.h"

namespace Eaagles {
namespace Sensor {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Tws,"Tws")

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Tws::Tws()
{
    STANDARD_CONSTRUCTOR()
}


//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Tws::copyData(const Tws& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Tws::deleteData()
{
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Tws::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    // ###DPG need to print slots 
    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Sensor namespace
} // End Eaagles namespace
