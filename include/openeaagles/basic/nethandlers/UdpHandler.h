//------------------------------------------------------------------------------
// Class: UdpHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_UdpHandler_H__
#define __Eaagles_Basic_UdpHandler_H__

#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Basic {

class String;

//------------------------------------------------------------------------------
// Class: UdpHandler
//
// Description: UDP/IP Unicast network handler.  Sends unicast packets to the
//             destination IP and port.
//
//             The destination port can be set by the input file, or by using
//             the setNetAddr() and setPort() functions prior to calling the
//             sendData() function, or by the using sendDataTo() function,
//             which changes to destination IP and port only for the one packet.
//
//             The local port must be defined if you're going to be changing
//             the destination port.
//              
//
// Form name: UdpHandler
// Slots:
//      ipAddress  <String>    ! Destination host name or IP address "111.122.133.144"
//                             ! (default: found via local host name)
//
// Input File Example:
//
//        ( UdpHandler
//           ipAddress: hostname    // Destination host name
//           port: 2010             // Destination port
//           localPort: 2011        // Local port to send from
//        )
//
//------------------------------------------------------------------------------
class UdpHandler : public NetHandler
{
   DECLARE_SUBCLASS(UdpHandler, NetHandler)

public:
   UdpHandler();

   // Sets the destination IP address (all future packets)
   bool setNetAddr(const uint32_t netAddr)      { return BaseClass::setNetAddr(netAddr); }

   // Sets the destination IP address by host name (all future packets)
   bool setNetAddr(const char* const hostname)  { return BaseClass::setNetAddr(hostname); }

   // Sets the destination port number (all future packets)
   bool setPort(const uint16_t n)               { return BaseClass::setPort(n); }

   // Send data to a specific IP/Port
   virtual bool sendDataTo(
         const char* const packet,  // Data packet
         const int size,            // Size of the data packet
         const uint32_t ip,         // Destination IP address (this packet only)
         const uint16_t port        // Destination port (this packet only)
      );

   // Slot functions
   virtual bool setSlotIpAddress(const String* const msg);

protected:
   // NetHandler interface
   virtual bool init();            // Initialize this socket handler
   virtual bool bindSocket();      // Bind socket to address

private:
   char* ipAddr;   // Host's name or IP Address
};

} // End Basic namespace
} // End Eaagles namespace

#endif

