
#include "openeaagles/models/Emission.hpp"

#include "openeaagles/models/systems/RfSystem.hpp"

#include <cmath>
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(Emission, "Emission")
EMPTY_SLOTTABLE(Emission)
EMPTY_SERIALIZER(Emission)

Emission::Emission()
{
    STANDARD_CONSTRUCTOR()
}

void Emission::copyData(const Emission& org, const bool)
{
    BaseClass::copyData(org);

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
void Emission::setRange(const double r)
{
   BaseClass::setRange(r);

   if (r > 1.0) lossRng = static_cast<double>(1.0/(4.0 * base::PI * r * r));
   else lossRng = 1.0;
}

//------------------------------------------------------------------------------
// setTransmitter() -- Sets the pointer to the source
//------------------------------------------------------------------------------
void Emission::setTransmitter(RfSystem* const t)
{
   //if (transmitter != nullptr) {
   //   transmitter->unref();
   //}
   transmitter = t;
   //if (transmitter != nullptr) {
   //   transmitter->ref();
   //}
}

}
}
