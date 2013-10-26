//------------------------------------------------------------------------------
// Class: TcpClient
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_TcpClient_H__
#define __Eaagles_Basic_TcpClient_H__

#include "openeaagles/basic/nethandlers/TcpHandler.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: TcpClient
//
// Description: Client side of a single TCP/IP connection.
//
//
// Form name: TcpClient
// Slots:
//      ipAddress  <String>    ! Destination host name or IP address "111.122.133.144"
//                             ! (default: found via local host name)
//
//
// Input File Example:
//
//        ( TcpClient
//             ipAddress: hostname    // Server host name
//             port: 2010             // Server port
//        )
//
// Note: initNetwork() will wait on a connection even if the 'noWaitFlag' is true.
// 
//------------------------------------------------------------------------------
class TcpClient : public TcpHandler
{
    DECLARE_SUBCLASS(TcpClient, TcpHandler)

public:
    TcpClient();

    // Slot functions
    virtual bool setSlotIpAddress(const String* const msg);

    // NetHandler interface
    virtual bool initNetwork(const bool noWaitFlag);

protected:
    const char* getIpAddress()  { return ipAddr; }
    bool connectToServer();      // Connect to the server

    // NetHandler interface
    virtual bool init();         // Initialize this socket handler

private:
    char* ipAddr;                // IP Address
};

} // End Basic namespace
} // End Eaagles namespace

#endif

