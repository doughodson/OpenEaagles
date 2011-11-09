#include "openeaagles/simulation/Jammer.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Emission.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Frequencies.h"
#include "openeaagles/basic/units/Powers.h"

namespace Eaagles {
namespace Simulation {

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
void Jammer::transmit(const LCreal)
{
    // Send the emission to the other player
    if ( !areEmissionsDisabled() && isTransmitting() ) {
        Emission* em = new Emission();
        em->setFrequency(getFrequency());
        LCreal p = getPeakPower();
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

} // End Simulation namespace
} // End Eaagles namespace
