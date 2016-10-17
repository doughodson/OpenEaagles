#include "openeaagles/simulation/SensorMsg.hpp"

#include "openeaagles/simulation/Gimbal.hpp"
#include "openeaagles/simulation/Player.hpp"
#include "openeaagles/base/units/Distances.hpp"

namespace oe {
namespace simulation {

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
    ownship = nullptr;
    gimbal = nullptr;
    target = nullptr;
    dataMsg = nullptr;

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

    const base::Object* msg = org.dataMsg;
    setDataMessage( const_cast<base::Object*>(static_cast<const base::Object*>(msg)) );

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
   setOwnship(nullptr);
   setGimbal(nullptr);
   setTarget(nullptr);
   setDataMessage(nullptr);
}
//------------------------------------------------------------------------------
// Sets the range to the target
//------------------------------------------------------------------------------
void SensorMsg::setRange(const double r)
{
   rng = r;
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

double SensorMsg::getRangeRateFPS() const
{
   return getRangeRate() * base::Distance::M2FT;
}

double SensorMsg::getRangeRateKts() const
{
   return getRangeRate() * base::Distance::M2NM * 3600.0f;
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
void SensorMsg::setDataMessage(base::Object* const msg)
{
   dataMsg = msg;
}

}
}
