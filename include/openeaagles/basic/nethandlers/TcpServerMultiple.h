//------------------------------------------------------------------------------
// Class: TcpServerMultiple
//------------------------------------------------------------------------------
#ifndef __Eaagles_Basic_TcpServerMultiple_H__
#define __Eaagles_Basic_TcpServerMultiple_H__

#include "openeaagles/basic/nethandlers/TcpHandler.h"

namespace Eaagles {
namespace Basic {

//------------------------------------------------------------------------------
// Class: TcpServerMultiple
//
// Description: Server side that can support multiple TCP/IP connections.
//              Our application must request each connection via acceptConnection(),
//              which will return a new instance of a TcpHandler when a connection
//              is made, or zero is return if no connection was made.
//
// Factory name: TcpServerMultiple
// Slots:
//      backlog   <Number>    ! Listen socket's max backlog (default: 1)
//
// Input File Example:
//
//        ( TcpServerMultiple
//            port: 2010      // Server port
//            backlog: 10     // max client backlog
//        )
//
// Note: The 'noWaitFlag' slot applies only to the new TcpHandler objects.
//       The functions initNetwork() and acceptConnection() are unblocked (or no
//       wait) I/O functions.
//
//------------------------------------------------------------------------------
class TcpServerMultiple : public TcpHandler
{
   DECLARE_SUBCLASS(TcpServerMultiple, TcpHandler)

public:
   TcpServerMultiple();

   virtual TcpHandler* acceptConnection();

   unsigned int getBacklog() const        { return backlog; }

   // Slot functions
   virtual bool setSlotBacklog(const Number* const msg);

   bool initNetwork(const bool noWaitFlag) override;

protected:
   bool bindSocket() override;

   virtual bool setBacklog(const unsigned int value);
   virtual bool listenForConnections();

private:
   unsigned int backlog;
};

} // End Basic namespace
} // End Eaagles namespace

#endif

