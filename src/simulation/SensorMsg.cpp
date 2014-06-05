#include "openeaagles/simulation/SensorMsg.h"

#include "openeaagles/simulation/Gimbal.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class SensorMsg
//==============================================================================
IMPLEMENT_SUBCLASS(SensorMsg,"SensorMsg")
EMPTY_SLOTTABLE(SensorMsg)
EMPTY_SERIALIZER(SensorMsg)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
SensorMsg::SensorMsg()
{
    STANDARD_CONSTRUCTOR()

    initData();
}

void SensorMsg::initData()
{
    maxRng = 0.0;
    rng   = 0.0;
    rngRate = 0.0;
    gaz  = 0.0;
    gel  = 0.0;
    iaz  = 0.0;
    iel  = 0.0;
    losO2T.set(0,0,0);
    losT2O.set(0,0,0);
    aoi.set(0,0,0);
    ownship = 0;
    gimbal = 0;
    target = 0;
    dataMsg = 0;

    returnReq = false;
    localOnly = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void SensorMsg::copyData(const SensorMsg& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    // Copy the data
    maxRng = org.maxRng;
    rng = org.rng;
    rngRate = org.rngRate;
    gaz = org.gaz;
    gel = org.gel;
    iaz = org.iaz;
    iel = org.iel;
    aoi = org.aoi;
    losO2T = org.losO2T;
    losT2O = org.losT2O;

    const Player* oo = org.ownship;
    setOwnship( const_cast<Player*>(static_cast<const Player*>(oo)) );

    const Gimbal* aa = org.gimbal;
    setGimbal( const_cast<Gimbal*>(static_cast<const Gimbal*>(aa)) );

    const Player* pp = org.target;
    setTarget( const_cast<Player*>(static_cast<const Player*>(pp)) );

    const Basic::Object* msg = org.dataMsg;
    setDataMessage( const_cast<Basic::Object*>(static_cast<const Basic::Object*>(msg)) );

    returnReq = org.returnReq;
    localOnly = org.localOnly;
}


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void SensorMsg::deleteData()
{
   clear();
}


//------------------------------------------------------------------------------
// clear() -- clears out the emissions
//------------------------------------------------------------------------------
void SensorMsg::clear()
{
   setOwnship(0);
   setGimbal(0);
   setTarget(0);
   setDataMessage(0);
}
//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void SensorMsg::setRange(const LCreal r)
{
   rng = r;
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

LCreal SensorMsg::getRangeRateFPS() const
{
   return getRangeRate() * Basic::Distance::M2FT;
}

LCreal SensorMsg::getRangeRateKts() const
{
   return getRangeRate() * Basic::Distance::M2NM * 3600.0f;
}


//------------------------------------------------------------------------------
// setOwnship() -- Sets the pointer to the originator (ownship)
//------------------------------------------------------------------------------
void SensorMsg::setOwnship(Player* const p)
{
   ownship = p;
}

//------------------------------------------------------------------------------
// setGimbal() -- Sets the pointer to the transmitting gimbal (seeker/antenna)
//------------------------------------------------------------------------------
void SensorMsg::setGimbal(Gimbal* const a)
{
   gimbal = a;
}

//------------------------------------------------------------------------------
// setTarget() -- Sets the pointer to the target
//------------------------------------------------------------------------------
void SensorMsg::setTarget(Player* const p)
{
   target = p;
}

//------------------------------------------------------------------------------
// setDataMessage() -- Sets a pointer to an optional data message
//------------------------------------------------------------------------------
void SensorMsg::setDataMessage(Basic::Object* const msg)
{
   dataMsg = msg;
}

} // End Simulation namespace
} // End Eaagles namespace
