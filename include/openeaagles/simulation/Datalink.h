//------------------------------------------------------------------------------
// Class: Datalink
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Datalink_H__
#define __Eaagles_Simulation_Datalink_H__

#include "openeaagles/simulation/System.h"
#include "openeaagles/basic/safe_queue.h"

namespace Eaagles {
   namespace Basic { class Distance; class Number; class String; }

namespace Simulation {
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
//    DATALINK_MESSAGE  (Basic::Object)  Default handler: Pass messages to subcomponents.
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

   virtual bool sendMessage(Basic::Object* const msg);
   virtual Basic::Object* receiveMessage();

   unsigned short getRadioID() const;

   CommRadio* getRadio()                                               { return radio; }
   const CommRadio* getRadio() const                                   { return radio; }
   virtual bool setRadio(CommRadio* const);

   const Basic::String* getRadioName() const                           { return radioName; }
   virtual bool setRadioName(const Basic::String* const name);

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
   Basic::safe_queue<Basic::Object*>* getOutputQueue()                 { return outQueue; }

   TrackManager* getTrackManager()                                     { return trackManager; }
   const TrackManager* getTrackManager() const                         { return trackManager; }
   virtual bool setTrackManager(TrackManager* const tm);

   const Basic::String* getTrackManagerName() const                    { return tmName; }
   virtual bool setTrackManagerName(const Basic::String* const name);

   // Event handler(s)
   virtual bool onDatalinkMessageEvent(Basic::Object* const);

   bool event(const int event, Basic::Object* const obj = nullptr) override;
   void reset() override;

protected:
   virtual bool queueIncomingMessage(Basic::Object* const msg);  // Queue up an incoming message
   virtual bool queueOutgoingMessage(Basic::Object* const msg);  // Queue up an outgoing message
   virtual void clearQueues(); // Clear all queues

   // Slot functions
   virtual bool setSlotRadioId(const Basic::Number* const num);
   virtual bool setSlotMaxRange(const Basic::Distance* const num);

   void dynamics(const LCreal dt) override;

   bool shutdownNotification() override;

private:
   void initData();

   static const int MAX_MESSAGES = 1000;  // Max number of messages in queues

   Basic::safe_queue<Basic::Object*>* inQueue;   // Received message queue
   Basic::safe_queue<Basic::Object*>* outQueue;  // Queue for messages going out over the network/DIS
   double noRadioMaxRange;                       // Max range of our datalink (NM)

   const Basic::String* radioName;    // Name of our radio
   CommRadio* radio;                  // Our radio

   unsigned short radioId;            // Radio ID input
   bool useRadioIdFlg;                // Use the 'radioId' flag

   bool sendLocal;                    // Send to local players flag; direct or via the radio
   bool queueForNetwork;              // Send the message to the network output queue

   TrackManager* trackManager;        // Track manager
   const Basic::String* tmName;       // Track manager name
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
