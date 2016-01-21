#include "openeaagles/simulation/Emission.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/RfSystem.h"
#include "openeaagles/simulation/Player.h"

#include <cmath>

namespace oe {
namespace Simulation {

//==============================================================================
// Class Emission
//==============================================================================
IMPLEMENT_SUBCLASS(Emission,"Emission")
EMPTY_SLOTTABLE(Emission)
EMPTY_SERIALIZER(Emission)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Emission::Emission()
{
    STANDARD_CONSTRUCTOR()

    freq  = 0.0;
    lambda = 0.0;
    pw    = 0.0;
    bw = 0.0;
    prf   = 0.0;
    pulses = 1;
    power = 0.0;
    gain = 0.0;
    lossRng = 1.0;
    lossAtmos = 1.0;
    lossXmit = 1.0;
    rcs = 0.0;
    polar = Antenna::NONE;
    transmitter = nullptr;
    ecmFlag = ECM_OFF;
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Emission::copyData(const Emission& org, const bool cc)
{
    BaseClass::copyData(org);

    // If copy constructor, init these pointers
    if (cc) {
        transmitter = nullptr;
    }

    // Copy the data
    freq = org.freq;
    lambda = org.lambda;
    pw = org.pw;
    power = org.power;
    polar = org.polar;
    bw = org.bw;
    gain = org.gain;
    prf = org.prf;
    pulses = org.pulses;
    lossRng = org.lossRng;
    lossAtmos = org.lossAtmos;
    lossXmit = org.lossXmit;
    rcs = org.rcs;

    const RfSystem* mm = org.transmitter;
    setTransmitter( const_cast<RfSystem*>(static_cast<const RfSystem*>(mm)) );

    ecmFlag = org.ecmFlag;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Emission::deleteData()
{
   clear();
}

//------------------------------------------------------------------------------
// clear() -- clears out the emissions
//------------------------------------------------------------------------------
void Emission::clear()
{
   BaseClass::clear();
   setTransmitter(nullptr);
}

//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void Emission::setRange(const LCreal r)
{
   BaseClass::setRange(r);

   if (r > 1.0) lossRng = static_cast<LCreal>(1.0/(4.0 * PI * r * r));
   else lossRng = 1.0;
}

//------------------------------------------------------------------------------
// setTransmitter() -- Sets the pointer to the source
//------------------------------------------------------------------------------
void Emission::setTransmitter(RfSystem* const t)
{
   //if (transmitter != 0) {
   //   transmitter->unref();
   //}
   transmitter = t;
   //if (transmitter != 0) {
   //   transmitter->ref();
   //}
}

} // End Simulation namespace
} // End oe namespace
