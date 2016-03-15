
#include "openeaagles/simulation/Datalink.h"

#include "openeaagles/simulation/Message.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Radio.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/simulation/OnboardComputer.h"

#include "openeaagles/base/Number.h"
#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"
#include "openeaagles/base/String.h"
#include "openeaagles/base/units/Distances.h"
#include "openeaagles/base/util/system.h"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(Datalink,"Datalink")

// Slot table
BEGIN_SLOTTABLE(Datalink)
   "radioId",           // 1: Radio ID (see note #1)
   "maxRange",          // 2: Max range of the datalink (w/o a radio model)
   "radioName",         // 3: Name of the (optional) communication radio mode
   "trackManagerName",  // 4: Track Manager Name
END_SLOTTABLE(Datalink)

//  Map slot table
BEGIN_SLOT_MAP(Datalink)
    ON_SLOT(1,setSlotRadioId,base::Number)
    ON_SLOT(2,setSlotMaxRange,base::Distance)
    ON_SLOT(3,setRadioName,base::String)
    ON_SLOT(4,setTrackManagerName,base::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Datalink)
    ON_EVENT_OBJ(DATALINK_MESSAGE,onDatalinkMessageEvent,base::Object)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
Datalink::Datalink()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Datalink::initData()
{
   noRadioMaxRange = 5000; //default is high in case someone doesn't set it correctly

   radioId = 0;
   useRadioIdFlg = false;

   sendLocal = true;
   queueForNetwork = true;

   radio = nullptr;
   radioName = nullptr;

   trackManager = nullptr;
   tmName = nullptr;

   inQueue = new base::safe_queue<base::Object*>(MAX_MESSAGES);
   outQueue = new base::safe_queue<base::Object*>(MAX_MESSAGES);
}

void Datalink::copyData(const Datalink& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   noRadioMaxRange = org.noRadioMaxRange;
   radioId = org.radioId;
   useRadioIdFlg = org.useRadioIdFlg;

   sendLocal = org.sendLocal;
   queueForNetwork = org.queueForNetwork;

   {
      const base::String* p = nullptr;
      if (org.radioName != nullptr) {
         p = org.radioName->clone();
      }
      setRadioName( p );
      setRadio(nullptr);
   }

   {
      const base::String* p = nullptr;
      if (org.tmName != nullptr) {
         p = org.tmName->clone();
      }
      setTrackManagerName( p );
      setTrackManager(nullptr);
   }
}

void Datalink::deleteData()
{
   if (inQueue != nullptr && outQueue != nullptr) {
      clearQueues();
      delete inQueue;
      delete outQueue;
      inQueue = nullptr;
      outQueue = nullptr;
   }
   setRadio(nullptr);
   setRadioName(nullptr);
   setTrackManager(nullptr);
   setTrackManagerName(nullptr);
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool Datalink::shutdownNotification()
{
   clearQueues();
   setRadio(nullptr);
   setTrackManager(nullptr);
   setTrackManagerName(nullptr);

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned short Datalink::getRadioID() const
{
   unsigned short id = 0;
   if (useRadioIdFlg) {
      id = radioId;
   }
   else if (radio != nullptr) {
      id = radio->getRadioId();
   }
   return id;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// max datalink range
bool Datalink::setMaxRange(const double nm)
{
   noRadioMaxRange = nm;
   return true;
}

// Send to local players flag
bool Datalink::setLocalSendEnabled(const bool flg)
{
   sendLocal = flg;
   return true;
}

// Send to the network output queue
bool Datalink::setNetworkQueueEnabled(const bool flg)
{
   queueForNetwork = flg;
   return true;
}

// set our comm radio system
bool Datalink::setRadio(CommRadio* const p)
{
   if (radio != nullptr) {
      radio->setDatalink(nullptr);
      radio->unref();
   }
   radio = p;
   if (radio != nullptr) {
      radio->ref();
      radio->setDatalink(this);
   }
   return true;
}

//  Sets our radio's name
bool Datalink::setRadioName(const base::String* const p)
{
    if (radioName != nullptr) {
        radioName->unref();
    }
    radioName = p;
    if (radioName != nullptr) {
        radioName->ref();
    }
    return true;
}

// set the track manager
bool Datalink::setTrackManager(TrackManager* const tm)
{
    if (trackManager != nullptr) {
        trackManager->unref();
    }
    trackManager = tm;
    if (trackManager != nullptr) {
        trackManager->ref();
    }
    return true;
}

// set the track manager's name
bool Datalink::setTrackManagerName(const base::String* const name)
{
    if (tmName != nullptr) {
        tmName->unref();
    }
    tmName = name;
    if (tmName != nullptr) {
        tmName->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Datalink::reset()
{
   clearQueues();
   // ---
   // Do we need to find the track manager?
   // ---
   if (getTrackManager() == nullptr && getTrackManagerName() != nullptr) {
        // We have a name of the track manager, but not the track manager itself
        const char* name = *getTrackManagerName();
        // Get the named track manager from the onboard computer
        Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
        if (ownship != nullptr) {
            OnboardComputer* obc = ownship->getOnboardComputer();
            if (obc != nullptr) {
               setTrackManager(obc->getTrackManagerByName(name));
            }
        }
        if (getTrackManager() == nullptr) {
            // The assigned track manager was not found!
            //if (isMessageEnabled(MSG_ERROR)) {
            //std::cerr << "Datalink ERROR -- track manager, " << name << ", was not found!" << std::endl;
            //}
        }
   }
   // ---
   // Do we need to find the comm radio?
   // ---
   if (getRadio() == nullptr && getRadioName() != nullptr) {
        // We have a name of the radio, but not the radio itself
        const char* name = *getRadioName();
        // Get the named radio from the component list of radios
        Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
        if (ownship != nullptr) {
            CommRadio* cr = dynamic_cast<CommRadio*>(ownship->getRadioByName(name));
            setRadio(cr);
        }
        CommRadio* rad = getRadio();
        if (rad == nullptr) {
            // The assigned radio was not found!
            if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Datalink ERROR -- radio, " << name << ", was not found!" << std::endl;
        }
        }
        else {
            rad->setDatalink(this);
            rad->setReceiverEnabledFlag(true);
            rad->setTransmitterEnableFlag(true);
        }
   }
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// dynamics() -- Age queues
//------------------------------------------------------------------------------
void Datalink::dynamics(const double)
{
    //age queues
    oe::base::Object* tempInQueue[MAX_MESSAGES];
    int numIn = 0;
    oe::simulation::Message* msg = nullptr;
    while((numIn < MAX_MESSAGES) && inQueue->isNotEmpty()) {
        oe::base::Object* tempObj = inQueue->get();
        msg = dynamic_cast<oe::simulation::Message*>(tempObj);
        if(msg != nullptr) {
            if(base::getComputerTime() - msg->getTimeStamp() > msg->getLifeSpan()) {
                //remove message by not adding to list to be put back into queue
                msg->unref();
            }
            else {
                tempInQueue[numIn++] = msg;
            }
        }
        else if(tempObj != nullptr) {
            tempInQueue[numIn++] = tempObj;
        }
    }
    if(numIn != 0) {
        for(int i = 0; i < numIn; i++) {
            inQueue->put(tempInQueue[i]);
        }
    }

    oe::base::Object* tempOutQueue[MAX_MESSAGES];
    int numOut = 0;
    msg = nullptr;
    while((numOut < MAX_MESSAGES) && outQueue->isNotEmpty()) {
        oe::base::Object* tempObj = outQueue->get();
        msg = dynamic_cast<oe::simulation::Message*>(tempObj);
        if(msg != nullptr) {
            if(base::getComputerTime() - msg->getTimeStamp() > msg->getLifeSpan()) {
                //remove message by not adding to list to be put back into queue
                msg->unref();
            }
            else {
                tempOutQueue[numOut++] = msg;
            }
            }
        else if(tempObj != nullptr) {
            tempOutQueue[numOut++] = tempObj;
        }
    }
    if(numOut != 0) {
        for(int i = 0; i < numOut; i++) {
            outQueue->put(tempOutQueue[i]);
        }
    }
}

//------------------------------------------------------------------------------
// sendMessage() -- send the datalink message out to the world.
//------------------------------------------------------------------------------
bool Datalink::sendMessage(base::Object* const msg)
{
   bool sent = false;

   // If we can send to our local players directly (or via radio)
   if (sendLocal) {
      // ---
      // Have a comm radio -- then we'll just let our companion radio system handle this
      // ---
      if (radio != nullptr) {
         sent = radio->transmitDataMessage(msg);
      }

      // ---
      // No comm radio -- then we'll send this out to the other players ourself.
      // ---
      else if (getOwnship() != nullptr) {
         Simulation* sim = getSimulation();
         if (sim != nullptr) {

            base::PairStream* players = sim->getPlayers();
            if (players != nullptr) {

            base::List::Item* playerItem = players->getFirstItem();
            while (playerItem != nullptr) {

               base::Pair* playerPair = static_cast<base::Pair*>(playerItem->getValue());
               Player* player = static_cast<Player*>(playerPair->object());

               if (player->isLocalPlayer()) {
                  // Send to active, local players only (and not to ourself)
                     if ((player->isActive() || player->isMode(Player::PRE_RELEASE)) && player != getOwnship() ) {
                     player->event(DATALINK_MESSAGE, msg);
                  }
                  playerItem = playerItem->getNext();
               }
               else {
                  // Networked players are at the end of the list,
                  // so we can stop now.
                  playerItem = nullptr;
               }

            }

               players->unref();
               players = nullptr;
            }
         }
         sent = true;
      }
   }

   // ---
   // and let any (optional) outgoing queue know about this.
   // ---
   if (queueForNetwork) {
      Player* ownship = getOwnship();
      if (ownship != nullptr) {
         if (ownship->isLocalPlayer()) {
            queueOutgoingMessage(msg);
         }
      }
   }

   return sent;
}

//------------------------------------------------------------------------------
// receiveMessage() --
//------------------------------------------------------------------------------
base::Object* Datalink::receiveMessage()
{
   // Get the next one off of the incoming message queue.
   return inQueue->get();
}

//------------------------------------------------------------------------------
// queueIncomingMessage() -- Queue up an incoming message
//------------------------------------------------------------------------------
bool Datalink::queueIncomingMessage(base::Object* const msg)
{
   // Only queue message if Ownship is local.  IPlayer messages are processed on their local systems
   if ((getOwnship() == nullptr) || !(getOwnship()->isLocalPlayer())) {
      return true;
   }

   //if (isMessageEnabled(MSG_INFO)) {
   //std::cout << getOwnship()->getID() << "\tincomming QQueue Size: " << inQueue->entries() << std::endl;
   //}

   if(inQueue->isFull()) {
      if (isMessageEnabled(MSG_WARNING)) {
         std::cerr << "dumping 10 oldest messages in Datalink::inQueue" << std::endl;
      }

      for(int i = 0; i < 10; i++) {
         base::Object* obj = inQueue->get();
         obj->unref();
      } //clear out 10 oldest messages
   }
   if (msg != nullptr) {
      msg->ref();
      inQueue->put(msg);
   }
   return true;
}

//------------------------------------------------------------------------------
// queueOutgoingMessage() -- Queue up an out going message --
//------------------------------------------------------------------------------
bool Datalink::queueOutgoingMessage(base::Object* const msg)
{
    //if (isMessageEnabled(MSG_INFO)) {
    //std::cout << getOwnship()->getID() << "\tOutgoing QQueue Size: " << outQueue->entries() << std::endl;
    //}

    if(outQueue->isFull()) {
        if (isMessageEnabled(MSG_WARNING)) {
        std::cerr << "dumping 10 oldest messages in Datalink::outQueue" << std::endl;
        }

        for(int i = 0; i < 10; i++) {
            base::Object* obj = outQueue->get();
            if (obj != nullptr) obj->unref();
        } //clear out 10 oldest messages
    }
    if (msg != nullptr) {
       msg->ref();
       outQueue->put(msg);
    }
    return true;
}

//------------------------------------------------------------------------------
// clearQueues() -- clear all queues
//------------------------------------------------------------------------------
void Datalink::clearQueues()
{
   base::Object* msg = inQueue->get();
   while (msg != nullptr) {
      msg->unref();
      msg = inQueue->get();
   }
   msg = outQueue->get();
   while (msg != nullptr) {
      msg->unref();
      msg = outQueue->get();
   }
}


//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// DATALINK_MESSAGE event handler
bool Datalink::onDatalinkMessageEvent(base::Object* const msg)
{
   // Just pass it down to all of our subcomponents
   base::PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      for (base::List::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
         base::Pair* pair = static_cast<base::Pair*>(item->getValue());
         base::Component* sc = static_cast<base::Component*>(pair->object());
         sc->event(DATALINK_MESSAGE, msg);
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool Datalink::setSlotRadioId(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int v = msg->getInt();
      if (v >= 0 && v <= 0xffff) {
         radioId = static_cast<unsigned short>(v);
         useRadioIdFlg = true;
         ok = true;
      }
   }
   return ok;
}

bool Datalink::setSlotMaxRange(const base::Distance* const msg)
{
   bool ok = false;
   if(msg != nullptr) {
      const double rng = base::NauticalMiles::convertStatic(*msg);
      ok = setMaxRange(rng);
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* Datalink::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Datalink::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }
    // DPG #### Need to print slots!!!
    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End simulation namespace
} // End oe namespace
