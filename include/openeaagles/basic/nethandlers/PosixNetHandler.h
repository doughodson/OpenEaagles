//------------------------------------------------------------------------------
// Class: PosixNetHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_PosixNetHandler_H__
#define __Eaagles_Basic_PosixNetHandler_H__

#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Basic {

class Number;
class String;

//------------------------------------------------------------------------------
// Class: PosixNetHandler
//
// Description: General (connectionless) network handler: Can be used for UDP/IP,
//              TCP/IP, Multicast and Broadcast.  Each handler manages a socket
//              and can be used to send data, receive data, or both.
//
// Windows: using Winsock2.h; link with Ws2_32.lib
//
// Slots:
//    localIpAddress    ! String containing the local host's name or its IP
//                      ! address in the Internet standard "." (dotted) notation.
//                      ! (default: found via local host name)
//
//    localPort         ! Local (source) port number (optional) (see examples below)
//                      !   send:    Local port that we'll send the packets from
//                      !   receive: Local port that we'll read packets
//
//    port              ! Number of the port we're connecting or listening to (required) (see examples below)
//                      !   send:    Report port we'll send packets to, and if 'localPort' is
//                      !            not defined, it's the local port we'll send data from.
//                      !   receive: We'll receive packets from this port if 'localPort'
//                      !            not defined.
//
//    shared            ! Set socket's shared (reuse) flag [default: false (not shared)]
//
//    sendBuffSizeKb    ! Send buffer size in KB's    (default:  32 Kb; max 1024)
//
//    recvBuffSizeKb    ! Receive buffer size in KB's (default: 128 Kb; max 1024)
//
//    ignoreSourcePort  ! Ignore messages from this port. This is one way to prevent receiving our
//                      ! own data with multicast or broadcast.
//
//
//    Local host side   Data Flow   Remote host side
//    ---------------   ---------   ----------------
//
//      port#           ------->       port#       ! Sending with only 'port' defined
//
//      localPort#      ------->       port#       ! Sending with 'port' and 'localPort' defined
//
//      port#           <-------     <any-port>    ! Receiving with only port defined
//
//      localPort#      <-------     <any-port>    ! Receiving anytime that 'localPort' is defined.
//
//------------------------------------------------------------------------------
class PosixNetHandler : public NetHandler
{
   DECLARE_SUBCLASS(PosixNetHandler, NetHandler)

public:
   PosixNetHandler();

   uint16_t getPort() const;                    // Port#
   uint16_t getLocalPort() const;               // Local port #
   uint16_t getIgnoreSourcePort() const;        // Ignore source port #

   uint32_t getLocalAddr() const;               // Gets the local host IP address
   uint32_t getNetAddr() const;                 // Gets the network (remote) host IP address

   bool getSharedFlag() const;                  // Is the socket address shared?
   void setSharedFlag(const bool b);

   // Initialize the network handler -- 'noWaitFlag' is true for unblocked I/O
   // Note: all parameters (slots) need to be set before calling this function.
   virtual bool initNetwork(const bool noWaitFlag);

   // Returns true if the network handler has been initialized (and connected if TCP)
   virtual bool isConnected() const;

   // Close (un-initialize) this network
   virtual bool closeConnection();

   // Send 'size' bytes from packet; returns true if successful
   virtual bool sendData(const char* const packet, const int size);

   // Receives a maximum of 'maxSize' bytes into 'packet.  Returns
   // the actual number of bytes received.
   virtual unsigned int recvData(char* const packet, const int maxSize);

   // Set our socket (or the optional 's' socket) for blocked (wait) I/O
   virtual bool setBlocked(const LcSocket s = NET_INVALID_SOCKET);

   // Set our socket (or the optional 's' socket) for unblocked (no wait) I/O
   virtual bool setNoWait(const LcSocket s = NET_INVALID_SOCKET);

   // Last recvData() origin IP and port
   uint32_t getLastFromAddr() const;     // IP address of last valid recvData()
   uint16_t getLastFromPort() const;     // Port address of last valid recvData()

   // Slot functions
   virtual bool setSlotLocalIpAddress(const String* const msg);
   virtual bool setSlotPort(const Number* const msg);
   virtual bool setSlotLocalPort(const Number* const msg);
   virtual bool setSlotShared(const Number* const msg);
   virtual bool setSlotSendBuffSize(const Number* const msg);
   virtual bool setSlotRecvBuffSize(const Number* const msg);
   virtual bool setSlotIgnoreSourcePort(const Number* const msg);

protected:
   virtual bool init();                // Initialize this socket handler
   virtual bool bindSocket();          // Bind socket to address

   // Sets the network IP address
   bool setNetAddr(const uint32_t netAddr);

   // Sets the network IP address by host name
   bool setNetAddr(const char* const hostname);

   // Sets the port number
   bool setPort(const uint16_t n);

   // Sets the local IP address
   bool setLocalAddr(const uint32_t addr);

   // Sets the local IP address using the Internet standard "." (dotted) notation
   bool setLocalAddr(const char* const addr);

   // Sets the local port number
   bool setLocalPort(const uint16_t n);

   // Set the output buffer size
   bool setSendBuffSize();

   // Sets the input buffer size
   bool setRecvBuffSize();

   LcSocket socketNum;             // Our Socket

private:
   char* localIpAddr;              // Local host IP address
   uint32_t localAddr;             // Local host address
   uint32_t netAddr;               // Network (remote) host address
   uint32_t fromAddr1;             // Last recvData() 'from' ip address
   uint16_t port;                  // Port
   uint16_t localPort;             // Local (source) port
   uint16_t ignoreSourcePort;      // Ignore message from this source port
   uint16_t fromPort1;             // Last recvData() 'from' port number
   bool sharedFlg;                 // Shared port flag
   bool initialized;               // handler has been initialized
   unsigned int sendBuffSizeKb;    // Send buffer size in KBs
   unsigned int recvBuffSizeKb;    // Receive buffer size in KBs
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

// Port#
inline uint16_t PosixNetHandler::getPort() const
{
   return port;
}

// Local port #
inline uint16_t PosixNetHandler::getLocalPort() const
{
   return localPort;
}

// Ignore source port #
inline uint16_t PosixNetHandler::getIgnoreSourcePort() const
{
   return ignoreSourcePort;
}

// Gets the local host IP address
inline uint32_t PosixNetHandler::getLocalAddr() const
{
   return localAddr;
}

// Gets the network (remote) host IP address
inline uint32_t PosixNetHandler::getNetAddr() const
{
   return netAddr;
}

// Shared flag
inline bool PosixNetHandler::getSharedFlag() const
{
   return sharedFlg;
}

inline uint32_t PosixNetHandler::getLastFromAddr() const
{
   return fromAddr1;
}

inline uint16_t PosixNetHandler::getLastFromPort() const
{
   return fromPort1;
}

} // End Basic namespace
} // End Eaagles namespace

#endif

