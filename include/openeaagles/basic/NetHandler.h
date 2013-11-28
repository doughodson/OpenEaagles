//------------------------------------------------------------------------------
// Class: NetHandler
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_NetHandler_H__
#define __Eaagles_Basic_NetHandler_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
namespace Basic {

class Number;
class String;

//------------------------------------------------------------------------------
// Class: NetHandler
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
class NetHandler : public Component
{
   DECLARE_SUBCLASS(NetHandler, Component)

public:  // Define the Windows vs Unix socket type
   #if defined(WIN32)
      typedef SOCKET LcSocket;
      static const LcSocket NET_INVALID_SOCKET = INVALID_SOCKET;
   #else
      typedef int LcSocket;
      static const LcSocket NET_INVALID_SOCKET = -1;
   #endif

public:
   NetHandler();

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

   // Set our socket (or the optional 's' socket) for blocked (wait) I/O
   virtual bool setBlocked(const LcSocket s = NET_INVALID_SOCKET);

   // Set our socket (or the optional 's' socket) for unblocked (no wait) I/O
   virtual bool setNoWait(const LcSocket s = NET_INVALID_SOCKET);

   // Send 'size' bytes from packet; returns true if successful
   virtual bool sendData(const char* const packet, const int size);

   // Receives a maximum of 'maxSize' bytes into 'packet.  Returns
   // the actual number of bytes received.
   virtual unsigned int recvData(char* const packet, const int maxSize);

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

   // To/From NET byte swap routines
   static void toNet(const void* const hostData, void* const netData, const int nl, const int ns);
   static void toHost(const void* const netData, void* const hostData, const int nl, const int ns);

   // Byte order
   static bool isNetworkByteOrder()        { return netByteOrder; }
   static bool isNotNetworkByteOrder()     { return !netByteOrder; }
   static bool checkByteOrder();           // Returns true if in network byte order

   // Convert to network byte order ('vout' is network byte order from 'vin' host network order)
   static void toNetOrder(int16_t* const vout, const int16_t vin);
   static void toNetOrder(uint16_t* const vout, const uint16_t vin);
   static void toNetOrder(int32_t* const vout, const int32_t vin);
   static void toNetOrder(uint32_t* const vout, const uint32_t vin);
   static void toNetOrder(int64_t* const vout, const int64_t vin);
   static void toNetOrder(uint64_t* const vout, const uint64_t vin);
   static void toNetOrder(float* const vout, const float vin);
   static void toNetOrder(double* const vout, const double vin);

   // convert from network byte order ('vout' is host byte order from 'vin' network byte order)
   static void fromNetOrder(int16_t* const vout, const int16_t vin);
   static void fromNetOrder(uint16_t* const vout, const uint16_t vin);
   static void fromNetOrder(int32_t* const vout, const int32_t vin);
   static void fromNetOrder(uint32_t* const vout, const uint32_t vin);
   static void fromNetOrder(int64_t* const vout, const int64_t vin);
   static void fromNetOrder(uint64_t* const vout, const uint64_t vin);
   static void fromNetOrder(float* const vout, const float vin);
   static void fromNetOrder(double* const vout, const double vin);

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
   static bool netByteOrder;       // True if this machine is in 'network byte order'
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

// Port#
inline uint16_t NetHandler::getPort() const
{
   return port;
}

// Local port #
inline uint16_t NetHandler::getLocalPort() const
{
   return localPort;
}

// Ignore source port #
inline uint16_t NetHandler::getIgnoreSourcePort() const
{
   return ignoreSourcePort;
}

// Gets the local host IP address
inline uint32_t NetHandler::getLocalAddr() const
{
   return localAddr;
}

// Gets the network (remote) host IP address
inline uint32_t NetHandler::getNetAddr() const
{
   return netAddr;
}

// Shared flag
inline bool NetHandler::getSharedFlag() const
{
   return sharedFlg;
}

inline uint32_t NetHandler::getLastFromAddr() const
{
   return fromAddr1;
}

inline uint16_t NetHandler::getLastFromPort() const
{
   return fromPort1;
}

// ---
// Convert to network byte order
// ---
inline void NetHandler::toNetOrder(int16_t* const vout, const int16_t vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(int16_t);
        *--q = *p++;
        *--q = *p++;
    }
}

inline void NetHandler::toNetOrder(uint16_t* const vout, const uint16_t vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(uint16_t);
        *--q = *p++;
        *--q = *p++;
    }
}

inline void NetHandler::toNetOrder(int32_t* const vout, const int32_t vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(int32_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void NetHandler::toNetOrder(uint32_t* const vout, const uint32_t vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(uint32_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void NetHandler::toNetOrder(int64_t* const vout, const int64_t vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(int64_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void NetHandler::toNetOrder(uint64_t* const vout, const uint64_t vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(uint64_t);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void NetHandler::toNetOrder(float* const vout, const float vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(float);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}

inline void NetHandler::toNetOrder(double* const vout, const double vin)
{
    if (isNotNetworkByteOrder()) {
        unsigned char* p = (unsigned char*) &vin;
        unsigned char* q = ((unsigned char*) vout) + sizeof(double);
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
        *--q = *p++;
    }
}


// ---
// convert from network byte order
// ---
inline void NetHandler::fromNetOrder(int16_t* const vout, const int16_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void NetHandler::fromNetOrder(uint16_t* const vout, const uint16_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void NetHandler::fromNetOrder(int32_t* const vout, const int32_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void NetHandler::fromNetOrder(uint32_t* const vout, const uint32_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void NetHandler::fromNetOrder(int64_t* const vout, const int64_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void NetHandler::fromNetOrder(uint64_t* const vout, const uint64_t vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void NetHandler::fromNetOrder(float* const vout, const float vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

inline void NetHandler::fromNetOrder(double* const vout, const double vin)
{
    // Same as the 'to' function
    return toNetOrder(vout,vin);
}

} // End Basic namespace
} // End Eaagles namespace

#endif

