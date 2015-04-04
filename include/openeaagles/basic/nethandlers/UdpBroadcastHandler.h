//------------------------------------------------------------------------------
// Class: UdpBroadcastHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_UdpBroadcastHandler_H__
#define __Eaagles_Basic_UdpBroadcastHandler_H__

#include "openeaagles/basic/nethandlers/PosixHandler.h"

namespace Eaagles {
namespace Basic {

class String;
class Number;

//------------------------------------------------------------------------------
// Class: UdpBroadcastHandler
//
// Description: UDP broadcast network handler
//
// Factory name: UdpBroadcastHandler
//
// Slots:
//       networkMask  <String>    ! Host Net Mask   "255.255.255.255"
//
// Input File Example:
//
//        ( UdpBroadcastHandler
//           localIpAddress: "224.0.0.251"  // Local host IP address
//           networkMask: "255.255.255.0"   // Local host Network Mask
//           port: 2010                     // Destination port
//           localPort: 2011                // Port to send from
//           shared: 1                      // Shared port
//        )
//
//------------------------------------------------------------------------------
class UdpBroadcastHandler : public PosixHandler
{
    DECLARE_SUBCLASS(UdpBroadcastHandler, PosixHandler)

public:
    UdpBroadcastHandler();

    // Slot Functions
    virtual bool setSlotNetworkMask(const String* const msg);

protected:
    // PosixHandler interface
    bool init() override;

    // PosixHandler interface
    bool bindSocket() override;

private:
    char* networkMask;           // Network Mask
};

} // End Basic namespace
} // End Eaagles namespace

#endif

