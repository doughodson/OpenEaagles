
#include "openeaagles/models/systems/Jammer.hpp"

#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/systems/Antenna.hpp"
#include "openeaagles/models/Emission.hpp"

//#include "openeaagles/models/WorldModel.hpp"

#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Frequencies.hpp"
#include "openeaagles/base/units/Powers.hpp"

namespace oe {
namespace models {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(Jammer, "Jammer")
EMPTY_SERIALIZER(Jammer)

Jammer::Jammer()
{
    STANDARD_CONSTRUCTOR()
    setTransmitterEnableFlag(true);
    setReceiverEnabledFlag(false);
    setTypeId("JAMMER");
}

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
        const auto em = new Emission();
        em->setFrequency(getFrequency());
        const double p = getPeakPower();
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
