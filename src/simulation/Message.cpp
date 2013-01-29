
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
   senderName = new Basic::String();
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
      senderName = org.senderName->clone();

   if (destNames != 0) {
      destNames->unref();
      destNames = 0;
   }
   if (org.destNames != 0)
      destNames = org.destNames->clone();
}

//-------------------------------------------------------------------------
// set sender information
//-------------------------------------------------------------------------
void Message::setSenderName(const Basic::String* const sName)
{
   if(sName != 0) {
      *senderName = *sName;
   }
}

void Message::setSenderName(const char* const sName)
{
   if(sName != 0) {
      *senderName = sName;
   }
}

//-------------------------------------------------------------------------
// add / remove / clear destination methods
//-------------------------------------------------------------------------
bool Message::addDestName(Basic::String* const name)
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

bool Message::addDestName(const char* const name)
{
   bool ret = false;
   if (name != 0) {
      Basic::String* p = new Basic::String(name);
      ret = addDestName(p);
   }
   return ret;
}

void Message::removeDestName(Basic::String* const name)
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

