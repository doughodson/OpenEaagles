//------------------------------------------------------------------------------
// Class: MulticastHandler
//------------------------------------------------------------------------------
#ifndef	__Eaagles_Basic_MulticastHandler_H__
#define __Eaagles_Basic_MulticastHandler_H__

#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Basic {

class String;

//------------------------------------------------------------------------------
// Class: MulticastHandler
// Form name: MulticastHandler
//
// Description: Multicast network handler.
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
//
// Form name: MulticastHandler
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
//        ( MulticastHandler
//           multicastGroup: "224.0.0.251"      // Multicast group
//           port: 2010                         // Multicast port
//           localPort: 2011                    // Port to send from
//           shared: 1                          // Shared socket
//           ttl:    4                          // Time-to-live
//           loopback: 1                        // Loop back
//        )
//
//
//------------------------------------------------------------------------------
class MulticastHandler : public NetHandler
{
    DECLARE_SUBCLASS(MulticastHandler,NetHandler)

public:
    MulticastHandler();

    int getTTL() const                      { return ttl; }
    void setTTL(const int v)                { ttl = v; }

    bool getLoopback() const                { return loopback; }
    void setLoopback(const bool b)          { loopback = b; }

    // Slot functions
    virtual bool setSlotMulticastGroup(const String* const msg);
    virtual bool setSlotTTL(const Number* const msg);
    virtual bool setSlotLoopback(const Number* const msg);

    // NetHandler interface
    virtual bool initNetwork(const bool noWaitFlag);
    virtual bool isConnected() const;
    virtual bool closeConnection();

protected:
    virtual bool joinTheGroup();

    // NetHandler interface
    virtual bool init();            // Initialize this socket handler
    virtual bool bindSocket();      // Bind socket to address

private:
    char* multicastGroup;           // Multicast Group Name
    int   ttl;                      // Time-to-live value
    bool  loopback;                 // Loop back flag
    bool  initialized;              // handler has been initialized
};

} // End Basic namespace
} // End Eaagles namespace

#endif
