
#include "openeaagles/simulation/Datalink.h"

#include "openeaagles/simulation/Message.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Radio.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/TrackManager.h"
#include "openeaagles/simulation/OnboardComputer.h"

#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

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
    ON_SLOT(1,setSlotRadioId,Basic::Number)
    ON_SLOT(2,setSlotMaxRange,Basic::Distance)
    ON_SLOT(3,setRadioName,Basic::String)
    ON_SLOT(4,setTrackManagerName,Basic::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Event() map
//------------------------------------------------------------------------------
BEGIN_EVENT_HANDLER(Datalink)
    ON_EVENT_OBJ(DATALINK_MESSAGE,onDatalinkMessageEvent,Basic::Object)
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

   radio = 0;
   radioName = 0;

   trackManager = 0;
   tmName = 0;

   inQueue = new QQueue<Basic::Object*>(MAX_MESSAGES);
   outQueue = new QQueue<Basic::Object*>(MAX_MESSAGES);
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
      const Basic::String* p = 0;
      if (org.radioName != 0) {
         p = org.radioName->clone();
      }
      setRadioName( p );
      setRadio(0);
   }

   {
      const Basic::String* p = 0;
      if (org.tmName != 0) {
         p = org.tmName->clone();
      }
      setTrackManagerName( p );
      setTrackManager(0);
   }
}

void Datalink::deleteData()
{
   if (inQueue != 0 && outQueue != 0) {
      clearQueues();
      delete inQueue;
      delete outQueue;
      inQueue = 0;
      outQueue = 0;
   }
   setRadio(0);
   setRadioName(0);
   setTrackManager(0);
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
   else if (radio != 0) {
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
   if (radio != 0) {
      radio->setDatalink(0);
      radio->unref();
   }
   radio = p;
   if (radio != 0) {
      radio->ref();
      radio->setDatalink(this);
   }
   return true;
}

//  Sets our radio's name
bool Datalink::setRadioName(const Basic::String* const p)
{
    if (radioName != 0) {
        radioName->unref();
    }
    radioName = p;
    if (radioName != 0) {
        radioName->ref();
    }
    return true;
}

// set the track manager
bool Datalink::setTrackManager(TrackManager* const tm)
{
    if (trackManager != 0) {
        trackManager->unref();
    }
    trackManager = tm;
    if (trackManager != 0) {
        trackManager->ref();
    }
    return true;
}

// set the track manager's name
bool Datalink::setTrackManagerName(const Basic::String* const name)
{
    if (tmName != 0) {
        tmName->unref();
    }
    tmName = name;
    if (tmName != 0) {
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
   if (getTrackManager() == 0 && getTrackManagerName() != 0) {
        // We have a name of the track manager, but not the track manager itself
        const char* name = *getTrackManagerName();
        // Get the named track manager from the onboard computer
        Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
        if (ownship != 0) {
            OnboardComputer* obc = ownship->getOnboardComputer();
            if (obc != 0) {
               setTrackManager(obc->getTrackManagerByName(name));
            }
        }    
        if (getTrackManager() == 0) {
            // The assigned track manager was not found!
            //if (isMessageEnabled(MSG_ERROR)) {
            //std::cerr << "Datalink ERROR -- track manager, " << name << ", was not found!" << std::endl;
            //}
        }
   }
   // ---
   // Do we need to find the comm radio?
   // ---
   if (getRadio() == 0 && getRadioName() != 0) {
        // We have a name of the radio, but not the radio itself
        const char* name = *getRadioName();
        // Get the named radio from the component list of radios
        Player* ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
        if (ownship != 0) {
            CommRadio* cr = dynamic_cast<CommRadio*>(ownship->getRadioByName(name));
            setRadio(cr);
        }
        CommRadio* rad = getRadio();
        if (rad == 0) {
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
void Datalink::dynamics(const LCreal)
{
    //age queues
    Eaagles::Basic::Object* tempInQueue[MAX_MESSAGES];
    int numIn = 0;
    Eaagles::Simulation::Message *msg = 0;
    while((numIn < MAX_MESSAGES) && inQueue->isNotEmpty()) {
        Eaagles::Basic::Object* tempObj = inQueue->get();
        msg = dynamic_cast<Eaagles::Simulation::Message*>(tempObj);        
        if(msg != 0) {
            if(getComputerTime() - msg->getTimeStamp() > msg->getLifeSpan()) {
                //remove message by not adding to list to be put back into queue
                msg->unref();
            }
            else {
                tempInQueue[numIn++] = msg;
            }
        }
        else if(tempObj != 0) {
            tempInQueue[numIn++] = tempObj;
        }
    }
    if(numIn != 0) {
        for(int i = 0; i < numIn; i++) {
            inQueue->put(tempInQueue[i]);
        }
    }

    Eaagles::Basic::Object* tempOutQueue[MAX_MESSAGES];
    int numOut = 0;
    msg = 0;
    while((numOut < MAX_MESSAGES) && outQueue->isNotEmpty()) {
        Eaagles::Basic::Object* tempObj = outQueue->get();
        msg = dynamic_cast<Eaagles::Simulation::Message*>(tempObj);        
        if(msg != 0) {
            if(getComputerTime() - msg->getTimeStamp() > msg->getLifeSpan()) {
                //remove message by not adding to list to be put back into queue
                msg->unref();
            }
            else {
                tempOutQueue[numOut++] = msg;
            }
            }
        else if(tempObj != 0) {
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
bool Datalink::sendMessage(Basic::Object* const msg)
{
   bool sent = false;

   // If we can send to our local players directly (or via radio)
   if (sendLocal) {
      // ---
      // Have a comm radio -- then we'll just let our companion radio system handle this
      // ---
      if (radio != 0) {
         sent = radio->transmitDataMessage(msg);
      }

      // ---
      // No comm radio -- then we'll send this out to the other players ourself.
      // ---
      else if (getOwnship() != 0) {
         Simulation* sim = getSimulation();
         if (sim != 0) {

            Basic::PairStream* players = sim->getPlayers();
            if (players != 0) {

            Basic::List::Item* playerItem = players->getFirstItem();
            while (playerItem != 0) {

               Basic::Pair* playerPair = (Basic::Pair*)(playerItem->getValue());
               Player* player = (Player*)(playerPair->object());

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
                  playerItem = 0;
               }

            }

               players->unref();
               players = 0;
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
      if (ownship != 0) {
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
Basic::Object* Datalink::receiveMessage()
{
   // Get the next one off of the incoming message queue.
   return inQueue->get();
}

//------------------------------------------------------------------------------
// queueIncomingMessage() -- Queue up an incoming message
//------------------------------------------------------------------------------
bool Datalink::queueIncomingMessage(Basic::Object* const msg)
{
   // Only queue message if Ownship is local.  IPlayer messages are processed on their local systems
   if ((getOwnship() == 0) || !(getOwnship()->isLocalPlayer())) {
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
         Basic::Object* obj = inQueue->get();
         obj->unref();
      } //clear out 10 oldest messages
   }
   if (msg != 0) {
      msg->ref();
      inQueue->put(msg);
   }
   return true;
}

//------------------------------------------------------------------------------
// queueOutgoingMessage() -- Queue up an out going message -- 
//------------------------------------------------------------------------------
bool Datalink::queueOutgoingMessage(Basic::Object* const msg)
{
    //if (isMessageEnabled(MSG_INFO)) {
    //std::cout << getOwnship()->getID() << "\tOutgoing QQueue Size: " << outQueue->entries() << std::endl;
    //}

    if(outQueue->isFull()) {
        if (isMessageEnabled(MSG_WARNING)) {
        std::cerr << "dumping 10 oldest messages in Datalink::outQueue" << std::endl;
        }

        for(int i = 0; i < 10; i++) {
            Basic::Object* obj = outQueue->get();
            if (obj != 0) obj->unref();
        } //clear out 10 oldest messages
    }
    if (msg != 0) {
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
   Basic::Object* msg = inQueue->get();
   while (msg != 0) {
      msg->unref();
      msg = inQueue->get();
   }
   msg = outQueue->get();
   while (msg != 0) {
      msg->unref();
      msg = inQueue->get();
   }
}


//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// DATALINK_MESSAGE event handler
bool Datalink::onDatalinkMessageEvent(Basic::Object* const msg)
{
   // Just pass it down to all of our subcomponents
   Basic::PairStream* subcomponents = getComponents();
   if (subcomponents != 0) {
      for (Basic::List::Item* item = subcomponents->getFirstItem(); item != 0; item = item->getNext()) {
         Basic::Pair* pair = (Basic::Pair*)(item->getValue());
         Basic::Component* sc = (Basic::Component*) pair->object();
         sc->event(DATALINK_MESSAGE, msg);
      }
      subcomponents->unref();
      subcomponents = 0;
   }
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool Datalink::setSlotRadioId(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != 0) {
      int v = msg->getInt();
      if (v >= 0 && v <= 0xffff) {
         radioId = (unsigned short) v;
         useRadioIdFlg = true;
         ok = true;
      }
   }
   return ok;
}

bool Datalink::setSlotMaxRange(const Basic::Distance* const msg)
{
   bool ok = false;
   if(msg != 0) {
      double rng = Basic::NauticalMiles::convertStatic(*msg);
      ok = setMaxRange(rng);
   }
   return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Datalink::getSlotByIndex(const int si)
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
        sout << "( " << getFormName() << std::endl;
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

} // End Simulation namespace
} // End Eaagles namespace
