//------------------------------------------------------------------------------
// Class: Datalink
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Datalink_H__
#define __oe_simulation_Datalink_H__

#include "openeaagles/simulation/System.h"
#include "openeaagles/basic/safe_queue.h"

namespace oe {
   namespace basic { class Distance; class Number; class String; }

namespace simulation {
   class CommRadio;
   class TrackManager;

//------------------------------------------------------------------------------
// Class: Datalink
// Description: Base class for all datalink models
//
// Factory name: Datalink
// Slots:
//    radioId           <Number>     ! Radio ID (see note #1) (default: 0)
//    maxRange          <Distance>   ! Max range of the datalink (w/o a radio model) (see note #2)
//                                   ! (default: 5000)
//    radioName         <Identifier> ! Name of the (optional) communication radio model (see notes #1 and #2)
//                                   ! (default: 0)
//    trackManagerName  <Identifier> ! Track Manager Name (default: 0)
//
// Events:
//    DATALINK_MESSAGE  (basic::Object)  Default handler: Pass messages to subcomponents.
//
//  Notes:
//    1) If both 'radioId' and 'radioName' are given and 'radioId' is not zero
//       then 'radioId' will override the ID of the named radio.
//    2) 'maxRange' is used when a named radio, 'radioName', is not provided.
//    3) This class is one of the "top level" systems attached to a Player
//       class (see Player.h).
//------------------------------------------------------------------------------
class Datalink : public System
{
   DECLARE_SUBCLASS(Datalink,System)

public:
   Datalink();

   virtual bool sendMessage(basic::Object* const msg);
   virtual basic::Object* receiveMessage();

   unsigned short getRadioID() const;

   CommRadio* getRadio()                                               { return radio; }
   const CommRadio* getRadio() const                                   { return radio; }
   virtual bool setRadio(CommRadio* const);

   const basic::String* getRadioName() const                           { return radioName; }
   virtual bool setRadioName(const basic::String* const name);

   // Max range (nm) of datalink without radio
   double getMaxRange() const                                          { return noRadioMaxRange; }
   virtual bool setMaxRange(const double nm);

   // Send messages to our local players; direct or via the radio (default: true)
   bool isLocalSendEnabled() const                                     { return sendLocal; }
   virtual bool setLocalSendEnabled(const bool flg);

   // Send messages to the network output queue (default: true)
   bool isNetworkQueueEnabled() const                                  { return queueForNetwork; }
   virtual bool setNetworkQueueEnabled(const bool flg);

   // For network handler to get to the messages
   basic::safe_queue<basic::Object*>* getOutputQueue()                 { return outQueue; }

   TrackManager* getTrackManager()                                     { return trackManager; }
   const TrackManager* getTrackManager() const                         { return trackManager; }
   virtual bool setTrackManager(TrackManager* const tm);

   const basic::String* getTrackManagerName() const                    { return tmName; }
   virtual bool setTrackManagerName(const basic::String* const name);

   // Event handler(s)
   virtual bool onDatalinkMessageEvent(basic::Object* const);

   bool event(const int event, basic::Object* const obj = nullptr) override;
   void reset() override;

protected:
   virtual bool queueIncomingMessage(basic::Object* const msg);  // Queue up an incoming message
   virtual bool queueOutgoingMessage(basic::Object* const msg);  // Queue up an outgoing message
   virtual void clearQueues(); // Clear all queues

   // Slot functions
   virtual bool setSlotRadioId(const basic::Number* const num);
   virtual bool setSlotMaxRange(const basic::Distance* const num);

   void dynamics(const LCreal dt) override;

   bool shutdownNotification() override;

private:
   void initData();

   static const int MAX_MESSAGES = 1000;  // Max number of messages in queues

   basic::safe_queue<basic::Object*>* inQueue;   // Received message queue
   basic::safe_queue<basic::Object*>* outQueue;  // Queue for messages going out over the network/DIS
   double noRadioMaxRange;                       // Max range of our datalink (NM)

   const basic::String* radioName;    // Name of our radio
   CommRadio* radio;                  // Our radio

   unsigned short radioId;            // Radio ID input
   bool useRadioIdFlg;                // Use the 'radioId' flag

   bool sendLocal;                    // Send to local players flag; direct or via the radio
   bool queueForNetwork;              // Send the message to the network output queue

   TrackManager* trackManager;        // Track manager
   const basic::String* tmName;       // Track manager name
};

} // End simulation namespace
} // End oe namespace

#endif
