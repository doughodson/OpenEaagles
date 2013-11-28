//------------------------------------------------------------------------------
// Class: OtwPC
// Base class: Basic::Object -> Component -> Otw -> OtwPC
//
// Description: PC Visual Interface
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Otw_OtwPC_H__
#define __Eaagles_Otw_OtwPC_H__

#include "openeaagles/simulation/Otw.h"

namespace Eaagles {
   namespace Basic { class NetHandler; }
namespace Otw {

class OtwPC : public Simulation::Otw
{
    DECLARE_SUBCLASS(OtwPC,Simulation::Otw)

public:
    OtwPC();

    virtual void reset();                // Send frame sync (if any)

protected:
    // Otw Interface
    virtual void sendOwnshipAndModels();     // Send state data for ownship and models
    virtual void sendElevationRequests();    // Sends terrain height requests
    virtual void recvElevations();           // Receives terrain height data
    virtual void frameSync();                // Send frame sync (if any)
    virtual Simulation::OtwModel* modelFactory();  // Create OtwModel objects unique to interface
    virtual Simulation::OtwModel* hotFactory(); // Create OtwHot objects unique to interface

    bool isNetworkInitialized() const       { return netInit; }
    bool didInitializationFail() const      { return netInitFail; }

private:
    void sendPcData();                      // Send PC data block
    bool initNetwork();                     // Initialize the network

    // Set Slot functions
    bool setSlotNetOutput(Basic::NetHandler* const msg);

    SPtr<Basic::NetHandler> netOutput;      // Output network handler
    bool             netInit;               // Network has been initialized
    bool             netInitFail;           // Initialization attempt failed
    int              scnt;                  // Send timer
};

} // End Otw namespace
} // End Eaagles namespace

#endif
