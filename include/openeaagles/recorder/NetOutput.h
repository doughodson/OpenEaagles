//------------------------------------------------------------------------------
// Class: NetOutput
//------------------------------------------------------------------------------
#ifndef __Eaagles_Recorder_NetOutput_H__
#define __Eaagles_Recorder_NetOutput_H__

#include "openeaagles/recorder/OutputHandler.h"

namespace Eaagles {
   namespace Basic { class NetHandler; class Number; }

namespace Recorder {

//------------------------------------------------------------------------------
// Class:   NetOutput
// Description: Serialize and write a DataRecord to a network
//
// Factory name: NetOutput
// Slots:
//      netHandler  <NetHandler>    Network output handler
//      noWait      <Number>        No wait (unblocked) I/O flag (default: false -- blocked I/O)
//------------------------------------------------------------------------------
class NetOutput : public OutputHandler
{
    DECLARE_SUBCLASS(NetOutput, OutputHandler)

public:
   NetOutput();

   virtual bool isNetworkEnabled() const;    // Has the network been initialized?
   virtual bool initNetworks();              // Init the network
   virtual void closeConnections();          // close the network connection

protected:
   // Slot functions
   virtual bool setSlotNetwork(Basic::NetHandler* const msg);
   virtual bool setSlotNoWait(Basic::Number* const msg);

   void processRecordImp(const DataRecordHandle* const handle) override;

private:
   void initData();

    Basic::safe_ptr<Basic::NetHandler> netHandler; // Network handler (input/output, or just output if netInput is defined)
    bool   networkInitialized;                     // Network has been initialized
    bool   networkInitFailed;                      // Network initialization has failed
    bool   noWaitFlag;                             // No wait (unblocked) I/O flag
};

} // End Recorder namespace
} // End Eaagles namespace

#endif
