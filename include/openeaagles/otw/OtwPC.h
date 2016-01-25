//------------------------------------------------------------------------------
// Class: OtwPC
// Description: PC Visual Interface
//------------------------------------------------------------------------------
#ifndef __oe_otw_OtwPC_H__
#define __oe_otw_OtwPC_H__

#include "openeaagles/simulation/Otw.h"

namespace oe {

namespace basic { class NetHandler; }

namespace otw {

class OtwPC : public Simulation::Otw
{
    DECLARE_SUBCLASS(OtwPC,Simulation::Otw)

public:
    OtwPC();

    void reset() override;                          // Send frame sync (if any)

protected:
    void sendOwnshipAndModels() override;           // Send state data for ownship and models
    void sendElevationRequests() override;          // Sends terrain height requests
    void recvElevations() override;                 // Receives terrain height data
    void frameSync() override;                      // Send frame sync (if any)
    Simulation::OtwModel* modelFactory() override;  // Create OtwModel objects unique to interface
    Simulation::OtwModel* hotFactory() override;    // Create OtwHot objects unique to interface

    bool isNetworkInitialized() const       { return netInit; }
    bool didInitializationFail() const      { return netInitFail; }

private:
    void sendPcData();                      // Send PC data block
    bool initNetwork();                     // Initialize the network

    // Set Slot functions
    bool setSlotNetOutput(basic::NetHandler* const msg);

    basic::safe_ptr<basic::NetHandler> netOutput;  // Output network handler
    bool netInit;                              // Network has been initialized
    bool netInitFail;                          // Initialization attempt failed
    int scnt;                                  // Send timer
};

} // End otw namespace
} // End oe namespace

#endif
