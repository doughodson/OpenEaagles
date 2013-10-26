//------------------------------------------------------------------------------
// Class: TcpServerSingle
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_TcpServerSingle_H__
#define __Eaagles_Basic_TcpServerSingle_H__

#include "openeaagles/basic/nethandlers/TcpHandler.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: TcpServerSingle
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
   DECLARE_SUBCLASS(TcpServerSingle, TcpHandler)

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

