//------------------------------------------------------------------------------
// Classes: TcpHandler, TcpClient, TcpServerMulti, TcpServerSingle
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_TcpHandler_H__
#define __Eaagles_Basic_TcpHandler_H__

#include "openeaagles/basic/NetHandler.h"

namespace Eaagles {
namespace Basic {

class String;

//------------------------------------------------------------------------------
// Class: TcpHandler
//
// Description: General TCP/IP support class.
//------------------------------------------------------------------------------
class TcpHandler : public NetHandler
{
   DECLARE_SUBCLASS(TcpHandler,NetHandler)

public:
   TcpHandler();

   bool hasBeenTerminated()          { return connectionTerminated; }

   // NetHandler interface
   virtual bool sendData(const char* const packet, const int size);
   virtual unsigned int recvData(char* const packet, const int maxSize);
   virtual bool isConnected() const;
   virtual bool closeConnection();

   // "Socket Only" constructor that will be used by one of our derived classes;
   // The socket should be an AF_INET, SOCK_STREAM type socket that has been
   // initialized, bound, connected, and is ready for use.
   TcpHandler(const LcSocket socketNum);

protected:
   // NetHandler interface
   virtual bool init();         // Initialize this socket handler
   virtual bool bindSocket();   // Bind socket to address

   bool connected;              // true if we are connected
   bool connectionTerminated;   // true if we had a connection, then lost it (or terminated it ourselves)

   LcSocket  tcpSocket;         // Our Connected TCP Socket
   bool  noWait;                // TCP's no wait I/O flag
};

//------------------------------------------------------------------------------
// Class: TcpClient
// Base class: Object -> NetHandler -> TcpHandler -> TcpClient
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
    DECLARE_SUBCLASS(TcpClient,TcpHandler)

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

//------------------------------------------------------------------------------
// Class: TcpServerMulti
// Base class: Object -> NetHandler -> TcpHandler -> TcpServerMulti
//
// Description: Server side that can support multiple TCP/IP connections.
//              Our application must request each connection via acceptConnection(),
//              which will return a new instance of a TcpHandler when a connection
//              is made, or zero is return if no connection was made.
//
// Form name: TcpServerMulti
// Slots:
//      backlog   <Number>    ! Listen socket's max backlog (default: 1)
//
// Input File Example:
//
//        ( TcpServerMulti
//            port: 2010      // Server port
//            backlog: 10     // max client backlog
//        )
//
// Note: The 'noWaitFlag' slot applies only to the new TcpHandler objects.
//       The functions initNetwork() and acceptConnection() are unblocked (or no
//       wait) I/O functions.
//
//------------------------------------------------------------------------------
class TcpServerMulti : public TcpHandler
{
   DECLARE_SUBCLASS(TcpServerMulti,TcpHandler)

public:
   TcpServerMulti();

   virtual TcpHandler* acceptConnection();

   unsigned int getBacklog() const { return backlog; }

   // Slot functions
   virtual bool setSlotBacklog(const Number* const msg);

   // NetHandler interface
   virtual bool initNetwork(const bool noWaitFlag);

protected:
   virtual bool setBacklog(const unsigned int value);
   virtual bool listenForConnections();

private:
   unsigned int backlog;
};

//------------------------------------------------------------------------------
// Class: TcpServerSingle
// Base class: Object -> NetHandler -> TcpHandler -> TcpServerSingle
//
// Description: Server side of a single TCP/IP connection.  
//
// Form name: TcpServer
//
// Input File Example:
//
//        ( TcpServer
//            port: 2010    // Server port
//        )
//
// Note: initNetwork() will wait on a connection even if the 'noWaitFlag' is true.
//
//------------------------------------------------------------------------------
class TcpServerSingle : public TcpHandler
{
   DECLARE_SUBCLASS(TcpServerSingle,TcpHandler)

public:
   TcpServerSingle();

   // NetHandler interface
   virtual bool initNetwork(const bool noWaitFlag);

protected:
   virtual bool listenForConnections();
   virtual bool acceptConnection();
};

} // End Basic namespace
} // End Eaagles namespace

#endif
