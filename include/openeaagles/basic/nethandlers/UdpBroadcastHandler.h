//------------------------------------------------------------------------------
// Class: UdpBroadcastHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_UdpBroadcastHandler_H__
#define __Eaagles_Basic_UdpBroadcastHandler_H__

#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Basic {

class String;
class Number;

//------------------------------------------------------------------------------
// Class: UdpBroadcastHandler
//
// Description: UDP broadcast network handler
//
// Form name: UdpBroadcastHandler
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
class UdpBroadcastHandler : public NetHandler
{
    DECLARE_SUBCLASS(UdpBroadcastHandler, NetHandler)

public:
    UdpBroadcastHandler();

    // Slot Functions
    virtual bool setSlotNetworkMask(const String* const msg);

protected:
    // NetHandler interface
    virtual bool init();         // Initialize this socket handler
    virtual bool bindSocket();   // Bind socket to address

private:
    char* networkMask;           // Network Mask
};

} // End Basic namespace
} // End Eaagles namespace

#endif

