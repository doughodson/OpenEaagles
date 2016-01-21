//------------------------------------------------------------------------------
// Class: UdpMulticastHandler
//------------------------------------------------------------------------------
#ifndef __oe_Basic_UdpMulticastHandler_H__
#define __oe_Basic_UdpMulticastHandler_H__

#include "openeaagles/basic/nethandlers/PosixHandler.h"

namespace oe {
namespace Basic {

class String;

//------------------------------------------------------------------------------
// Class: UdpMulticastHandler
//
// Description: UDP multicast network handler.
//
// Factory name: UdpMulticastHandler
//
// multicast:
// "A point-to-many networking model in which a packet is sent to a specific address,
//  and only those computers that are set to receive information from this address
//  receive the packet. On the Internet, the possible IP multicast addresses range
//  from 224.0.0.0 through 239.255.255.255. Computer networks typically use a unicast
//  model, in which a different version of the same packet is sent to each address
//  that must receive it. The multicast model greatly reduces traffic and increases
//  efficiency on such networks."
//
// Slots:
//      multicastGroup  <String>    ! String containing the multicast IP address in
//                                  the Internet standard "." (dotted) notation.
//                                  IP multicast addresses range from 224.0.0.0
//                                  through 239.255.255.255 (e.g., "225.0.0.251")
//
//      ttl             <Number>    ! Multicast Time-To-Live (TTL) value (default: 1)
//
//      loopback        <Number>    ! Multicast Loopback flag (default: 0 (off))
//
//
// From Windows documentation:
//    "Note  The Winsock version of the IP_MULTICAST_LOOP option is semantically
//    different than the UNIX version of the IP_MULTICAST_LOOP option:
//    -> In Winsock, the IP_MULTICAST_LOOP option applies only to the receive path.
//    -> In the UNIX version, the IP_MULTICAST_LOOP option applies to the send path.
//    -> For example, applications ON and OFF (which are easier to track than X and Y)
//       join the same group on the same interface; application ON sets the IP_MULTICAST_LOOP
//       option on, application OFF sets the IP_MULTICAST_LOOP option off. If ON and OFF are
//       Winsock applications, OFF can send to ON, but ON cannot sent to OFF. In contrast,
//       if ON and OFF are UNIX applications, ON can send to OFF, but OFF cannot send to ON."
//
//
// Input File Example:
//
//        ( UdpMulticastHandler
//           multicastGroup: "224.0.0.251"      // Multicast group
//           port: 2010                         // Multicast port
//           localPort: 2011                    // Port to send from
//           shared: 1                          // Shared socket
//           ttl:    4                          // Time-to-live
//           loopback: 1                        // Loop back
//        )
//
//------------------------------------------------------------------------------
class UdpMulticastHandler : public PosixHandler
{
    DECLARE_SUBCLASS(UdpMulticastHandler, PosixHandler)

public:
    UdpMulticastHandler();

    int getTTL() const                      { return ttl; }
    void setTTL(const int v)                { ttl = v; }

    bool getLoopback() const                { return loopback; }
    void setLoopback(const bool b)          { loopback = b; }

    // Slot functions
    virtual bool setSlotMulticastGroup(const String* const msg);
    virtual bool setSlotTTL(const Number* const msg);
    virtual bool setSlotLoopback(const Number* const msg);

    bool initNetwork(const bool noWaitFlag) override;
    bool isConnected() const override;
    bool closeConnection() override;

protected:
    virtual bool joinTheGroup();

    bool init() override;
    bool bindSocket() override;

private:
    char* multicastGroup;           // Multicast Group Name
    int   ttl;                      // Time-to-live value
    bool  loopback;                 // Loop back flag
    bool  initialized;              // handler has been initialized
};

} // End Basic namespace
} // End oe namespace

#endif

