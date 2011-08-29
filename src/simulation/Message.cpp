
#include "openeaagles/simulation/Message.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(Message,"Message")
EMPTY_SLOTTABLE(Message)
EMPTY_SERIALIZER(Message)

//-------------------------------------------------------------------------
//Constructor
//-------------------------------------------------------------------------
Message::Message()
{
   initData();
}

//-------------------------------------------------------------------------
// initData()
//-------------------------------------------------------------------------
void Message::initData()
{
   timeStamp = 0;
   lifeSpan = 5;    // seconds
   ack = ACK;
   senderName = 0;
   destNames = 0;
}

//-------------------------------------------------------------------------
// deleteData()
//-------------------------------------------------------------------------
void Message::deleteData()
{
   if (senderName != 0) { senderName->unref();  senderName = 0; }
   if (destNames  != 0) { destNames->unref();   destNames  = 0; }
}

//-------------------------------------------------------------------------
// copyData()
//-------------------------------------------------------------------------
void Message::copyData(const Message& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   timeStamp = org.timeStamp;
   lifeSpan = org.lifeSpan;
   ack = org.ack;
   if (senderName != 0) {
      senderName->unref();
      senderName = 0;
   }
   if (org.senderName != 0)
      senderName = (Basic::String*)org.senderName->clone();

   if (destNames != 0) {
      destNames->unref();
      destNames = 0;
   }
   if (org.destNames != 0)
      destNames = (Basic::List*)org.destNames->clone();
}

//-------------------------------------------------------------------------
// set sender information
//-------------------------------------------------------------------------
void Message::setSenderName(Basic::String* sName)
{
   if(sName != 0) {
      if(senderName != 0) {
         senderName->unref();
      }
      senderName = sName;
      senderName->ref();
   }
}

void Message::setSenderName(const char* const sName)
{
   if(sName != 0) {
      if (senderName != 0)
         senderName->setStr(sName);
      else
         senderName = new Basic::String(sName);
   }
}

//-------------------------------------------------------------------------
// add / remove / clear destination methods
//-------------------------------------------------------------------------
bool Message::addDestName(Basic::String* name)
{
   bool ret = false;
   if(name != 0) {
      if (destNames == 0)
         destNames = new Basic::List();
      destNames->addTail(name);
      ret = true;
   }
   return ret;
}

void Message::removeDestName(Basic::String* name)
{
   if (destNames != 0) {
      destNames->remove(name);
      if (destNames->entries() == 0) {
         destNames->unref();
         destNames = 0;
      }
   }
}

void Message::clearDestNames()
{
   if (destNames != 0) {
      destNames->unref();
      destNames = 0;
   }
}

} //End simulation Namespace
} //End Eaagles Namespace

