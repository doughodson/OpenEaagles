//------------------------------------------------------------------------------
// Class: Hla::Mc02Fom::EmitterBeamNib
//
// Millenium Challenge 2002 FOM
// Version: RPR 1.0 Draft 2 w/extensions: $Revision: 1.6
//------------------------------------------------------------------------------
#ifndef __Mc02FomNibEmitterBeam_H_15004ECB_4961_42c1_8C35_712A84FE44DB__
#define __Mc02FomNibEmitterBeam_H_15004ECB_4961_42c1_8C35_712A84FE44DB__

namespace Mc02 { class BaseEntity; class EmitterBeam; }

#include "openeaagles/hla/Nib.h"

namespace Eaagles {
namespace Network {
namespace Hla {
namespace Mc02Fom {
   class EmitterBeam;

//==============================================================================
// Class:       EmitterBeamNib
// Base class:  ... -> Simulation::Nib -> Hla::Nib -> Hla::Mc02Fom::EmitterBeamNib
// GUID: {15004ECB-4961-42c1-8C35-712A84FE44DB}
//==============================================================================
class EmitterBeamNib : public Hla::Nib
{
    DECLARE_SUBCLASS(EmitterBeamNib,Hla::Nib)
    
public:
    EmitterBeamNib(const Simulation::NetworkIO::IoType ioType);

    bool isJammerBeam() const                         { return jammer; }
    void setJammerBeamFlag(const bool flg)            { jammer = flg; }

    // MC02 data structures
    EmitterBeam* getEmitterBeam()               { return emitterBeam; }
    const EmitterBeam* getEmitterBeam() const   { return emitterBeam; }
    virtual void setEmitterBeam(EmitterBeam* const p);
    
    // Input support functions
    virtual void reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs);
    //virtual bool makeEmitterBeamObjectName(const int idx);                  // Makes a default RTI name

private:
    EmitterBeam* emitterBeam;  // The object's data
    bool         jammer;       // Is a jammer beam
};


} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif // __Mc02FomNibEmitterBeam_H_15004ECB_4961_42c1_8C35_712A84FE44DB__
