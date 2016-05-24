//------------------------------------------------------------------------------
// Class: TcpServerSingle
//------------------------------------------------------------------------------
#ifndef __oe_base_TcpServerSingle_H__
#define __oe_base_TcpServerSingle_H__

#include "openeaagles/base/nethandlers/TcpHandler.h"

namespace oe {
namespace base {

//------------------------------------------------------------------------------
// Class: TcpServerSingle
//
// Description: Server side of a single TCP/IP connection.
//
// Factory name: TcpServer
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

   virtual bool initNetwork(const bool noWaitFlag) override;

protected:
   virtual bool bindSocket() override;

   virtual bool listenForConnections();
   virtual bool acceptConnection();
};

}
}

#endif

