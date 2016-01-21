//------------------------------------------------------------------------------
// Class: NetInput
//------------------------------------------------------------------------------
#ifndef __oe_Recorder_NetInput_H__
#define __oe_Recorder_NetInput_H__

#include "openeaagles/recorder/InputHandler.h"

namespace oe {
   namespace Basic { class NetHandler; class Number; }

namespace Recorder {

//------------------------------------------------------------------------------
// Class:   NetInput
// Description: Read and parse data records from a network stream
//
// Factory name: NetInput
// Slots:
//      netHandler  <NetHandler>    Network input handler
//      noWait      <Number>        No wait (unblocked) I/O flag (default: false -- blocked I/O)
//------------------------------------------------------------------------------
class NetInput : public InputHandler
{
    DECLARE_SUBCLASS(NetInput, InputHandler)

public:
   static const unsigned int MAX_INPUT_BUFFER_SIZE = 2000;

public:
   NetInput();

   virtual bool isNetworkEnabled() const;

   virtual bool initNetworks();
   virtual void closeConnections();

protected:
   // Slot functions
   virtual bool setSlotNetwork(Basic::NetHandler* const msg);
   virtual bool setSlotNoWait(Basic::Number* const msg);

   const DataRecordHandle* readRecordImp() override;

private:
   void initData();

    Basic::safe_ptr<oe::Basic::NetHandler> netHandler;   // Network handler (input/output, or just output if netInput is defined)
    bool   networkInitialized;                     // Network has been initialized
    bool   networkInitFailed;                      // Network initialization has failed
    bool   noWaitFlag;                             // No wait (unblocked) I/O flag
    bool firstPassFlg;               // First pass flag

   char* ibuf;    // Input buffer
};

} // End Recorder namespace
} // End oe namespace

#endif
