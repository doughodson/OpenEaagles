#include "openeaagles/simulation/Jammer.hpp"

#include "openeaagles/simulation/Antenna.hpp"
#include "openeaagles/simulation/Emission.hpp"
#include "openeaagles/simulation/Player.hpp"
#include "openeaagles/simulation/Simulation.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Frequencies.hpp"
#include "openeaagles/base/units/Powers.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Jammer,"Jammer")
EMPTY_SERIALIZER(Jammer)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
Jammer::Jammer()
{
    STANDARD_CONSTRUCTOR()
    setTransmitterEnableFlag(true);
    setReceiverEnabledFlag(false);
    setTypeId("JAMMER");
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Jammer::copyData(const Jammer& org, const bool)
{
    BaseClass::copyData(org);
}

EMPTY_DELETEDATA(Jammer)

//------------------------------------------------------------------------------
// transmit() -- send jam emissions
//------------------------------------------------------------------------------
void Jammer::transmit(const double)
{
    // Send the emission to the other player
    if ( !areEmissionsDisabled() && isTransmitting() ) {
        Emission* em = new Emission();
        em->setFrequency(getFrequency());
        double p = getPeakPower();
        em->setPower(p);
        em->setTransmitLoss(getRfTransmitLoss());
        em->setMaxRangeNM(getRange());
        em->setBandwidth(getBandwidth());
        em->setTransmitter(this);
        em->setReturnRequest(false);
        em->setECM(Emission::ECM_NOISE);
        getAntenna()->rfTransmit(em);
        em->unref();
    }
}

}
}
