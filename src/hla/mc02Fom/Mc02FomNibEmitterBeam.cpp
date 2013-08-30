#include "openeaagles/hla/mc02Fom/Mc02FomNibEmitterBeam.h"

#include "openeaagles/hla/mc02Fom/Mc02FomIO.h"
#include "openeaagles/hla/mc02Fom/Mc02Fom.h"
#include "openeaagles/hla/mc02Fom/Mc02FomNib.h"
#include "openeaagles/hla/Ambassador.h"

#include "openeaagles/simulation/AirVehicle.h"
#include "openeaagles/simulation/Missile.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Signatures.h"
#include "openeaagles/simulation/Weapon.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"
#include "openeaagles/basic/Number.h"

namespace Eaagles {
namespace Network {
namespace Hla {
namespace Mc02Fom {


//==============================================================================
// Class: EmitterBeamNib
//==============================================================================
IMPLEMENT_PARTIAL_SUBCLASS(EmitterBeamNib,"Mc02FomEmitterBeamNib")
EMPTY_SLOTTABLE(EmitterBeamNib)
EMPTY_SERIALIZER(EmitterBeamNib)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
EmitterBeamNib::EmitterBeamNib(const Simulation::NetworkIO::IoType ioType)  : Hla::Nib(ioType) 
{
   SET_SLOTTABLE

   emitterBeam = 0;
   jammer = false;
}

EmitterBeamNib::EmitterBeamNib(const EmitterBeamNib& org) : Hla::Nib(org.getIoType())
{ 
    SET_SLOTTABLE
    copyData(org,true);
}

EmitterBeamNib::~EmitterBeamNib()
{
   deleteData();
}

EmitterBeamNib& EmitterBeamNib::operator=(const EmitterBeamNib& org)
{
    deleteData();
    copyData(org,false);
    return *this;
}

Basic::Object* EmitterBeamNib::clone() const
{
    return new EmitterBeamNib(*this);
}

//------------------------------------------------------------------------------
// copyData() --
//------------------------------------------------------------------------------
void EmitterBeamNib::copyData(const EmitterBeamNib& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) {
        emitterBeam = 0;
    }

    jammer = org.jammer;

    // Base entity
    if (org.emitterBeam != 0)
        setEmitterBeam( (Mc02Fom::EmitterBeam*) org.emitterBeam->clone() );
    else
        setEmitterBeam(0);

}

//------------------------------------------------------------------------------
// deleteData() --
//------------------------------------------------------------------------------
void EmitterBeamNib::deleteData()
{
    setEmitterBeam(0);
    BaseClass::deleteData();
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void EmitterBeamNib::setEmitterBeam(Mc02Fom::EmitterBeam* const p)
{
   if (emitterBeam != 0) emitterBeam->unref();
   emitterBeam = p;
   if (emitterBeam != 0) emitterBeam->ref();
}

} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
