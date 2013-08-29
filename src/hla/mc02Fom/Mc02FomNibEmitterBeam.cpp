#include "eaagles/hla/mc02Fom/Mc02FomNibEmitterBeam.h"

#include "eaagles/hla/mc02Fom/Mc02FomIO.h"
#include "eaagles/hla/mc02Fom/Mc02Fom.h"
#include "eaagles/hla/mc02Fom/Mc02FomNib.h"
#include "eaagles/hla/Ambassador.h"

#include "eaagles/simulation/AirVehicle.h"
#include "eaagles/simulation/Missile.h"
#include "eaagles/simulation/Player.h"
#include "eaagles/simulation/Simulation.h"
#include "eaagles/simulation/Signatures.h"
#include "eaagles/simulation/Weapon.h"
#include "eaagles/basic/Pair.h"
#include "eaagles/basic/PairStream.h"
#include "eaagles/basic/String.h"
#include "eaagles/basic/Nav.h"
#include "eaagles/basic/NetHandler.h"
#include "eaagles/basic/Number.h"

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
