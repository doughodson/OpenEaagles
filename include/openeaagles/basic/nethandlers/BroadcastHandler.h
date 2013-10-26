//------------------------------------------------------------------------------
// Class: BroadcastHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_BroadcastHandler_H__
#define __Eaagles_Basic_BroadcastHandler_H__

#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Basic {

class String;
class Number;

//------------------------------------------------------------------------------
// Class: BroadcastHandler
// Form name: BroadcastHandler
//
// Description: Broadcast network handler.
//
// Form name: BroadcastHandler
// Slots:
//      networkMask  <String>    ! Host Net Mask   "255.255.255.255"
//
// Input File Example:
//
//        ( BroadcastHandler
//           localIpAddress: "224.0.0.251"  // Local host IP address
//           networkMask: "255.255.255.0"   // Local host Network Mask
//           port: 2010                     // Destination port
//           localPort: 2011                // Port to send from
//           shared: 1                      // Shared port
//        )
//
//
//------------------------------------------------------------------------------
class BroadcastHandler : public NetHandler
{
    DECLARE_SUBCLASS(BroadcastHandler, NetHandler)

public:
    BroadcastHandler();

    // Slot Functions
    virtual bool setSlotNetworkMask(const String* const msg);

protected:
    // NetHandler interface
    virtual bool init();            // Initialize this socket handler
    virtual bool bindSocket();      // Bind socket to address

private:
    char* networkMask;           // Network Mask
};

} // End Basic namespace
} // End Eaagles namespace

#endif

