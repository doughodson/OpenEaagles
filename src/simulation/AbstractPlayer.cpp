
#include "openeaagles/simulation/AbstractPlayer.hpp"

#include "openeaagles/base/Number.hpp"
#include "openeaagles/base/String.hpp"

#include "openeaagles/simulation/AbstractNetIO.hpp"
#include "openeaagles/simulation/AbstractNib.hpp"

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractPlayer, "AbstractPlayer")

BEGIN_SLOTTABLE(AbstractPlayer)
   "id",                // 01) Player id   [ 1 .. 65535 ]
   "mode",              // 02) Initial player mode ( INACTIVE, ACTIVE, DEAD )
END_SLOTTABLE(AbstractPlayer)

BEGIN_SLOT_MAP(AbstractPlayer)
   ON_SLOT(01, setSlotID,       base::Number)
   ON_SLOT(02, setSlotInitMode, base::String)
END_SLOT_MAP()

AbstractPlayer::AbstractPlayer()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

//------------------------------------------------------------------------------
// initData() -- init our member data
//------------------------------------------------------------------------------
void AbstractPlayer::initData()
{
   id = 0;
   pname = "";

   mode = ACTIVE;
   initMode = mode;

   nib = nullptr;
   netID = 0;
   nibList = new AbstractNib*[AbstractNetIO::MAX_NETWORD_ID];
   for (unsigned int i = 0; i < AbstractNetIO::MAX_NETWORD_ID; i++) {
      nibList[i] = nullptr;
   }
   enableNetOutput = true;
}

void AbstractPlayer::copyData(const AbstractPlayer& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   id = org.id;

   if (org.pname != nullptr) {
      setName(org.pname);
   }

   mode = org.mode;
   initMode = org.initMode;

   netID = org.netID;
   enableNetOutput = org.enableNetOutput;

   // NIB pointers are not copied!
   setNib( nullptr );
   for (unsigned int i = 0; i < simulation::AbstractNetIO::MAX_NETWORD_ID; i++) {
      setOutgoingNib(nullptr, i);
   }
}

void AbstractPlayer::deleteData()
{
   setNib(nullptr);
   if (nibList != nullptr) {
      for (unsigned int i = 0; i < simulation::AbstractNetIO::MAX_NETWORD_ID; i++) {
         setOutgoingNib(nullptr, i);
      }
      delete[] nibList;
      nibList = nullptr;
   }
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool AbstractPlayer::shutdownNotification()
{
   if (nib != nullptr) nib->event(SHUTDOWN_EVENT);
   if (nibList != nullptr) {
      for (unsigned int i = 0; i < simulation::AbstractNetIO::MAX_NETWORD_ID; i++) {
         if (nibList[i] != nullptr) nibList[i]->event(SHUTDOWN_EVENT);
      }
   }
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void AbstractPlayer::reset()
{
   if (isLocalPlayer()) {
      setMode(initMode);
   }

   // ---
   // Reset our base class
   // -- Do this last because it sends reset pulses to our components and
   //    we want to make sure our data is initialized first.
   // ---
   BaseClass::reset();
}

//-----------------------------------------------------------------------------

// Sets the player's ID
void AbstractPlayer::setID(const unsigned short v)
{
   id = v;
}

//-----------------------------------------------------------------------------

// Set the player's name
void AbstractPlayer::setName(const base::Identifier& n)
{
   pname = n;
}

// Set the player's name
void AbstractPlayer::setName(const char* const str)
{
   pname = str;
}

//-----------------------------------------------------------------------------

// Sets the player's mode (ACTIVE, DEAD, etc)
void AbstractPlayer::setMode(const Mode m)
{
   mode = m;
}

// Sets the player's initial (reset) mode
void AbstractPlayer::setInitMode(const Mode m)
{
   initMode = m;
}

//-----------------------------------------------------------------------------

// Player's outgoing NIB(s)
AbstractNib* AbstractPlayer::getLocalNib(const unsigned int netId)
{
   AbstractNib* p = nullptr;
   if (nibList != nullptr && netId >= 1 && netId <= AbstractNetIO::MAX_NETWORD_ID) {
      p = nibList[netId-1];
   }
   return p;
}

// Player's outgoing NIB(s)  (const version)
const AbstractNib* AbstractPlayer::getLocalNib(const unsigned int netId) const
{
   const AbstractNib* p = nullptr;
   if (nibList != nullptr && netId >= 1 && netId <= AbstractNetIO::MAX_NETWORD_ID) {
      p = nibList[netId-1];
   }
   return p;
}

//-----------------------------------------------------------------------------

// Sets a pointer to the Network Interface Block (NIB)
bool AbstractPlayer::setNib(AbstractNib* const n)
{
   if (nib != nullptr) nib->unref();
   nib = n;
   if (nib != nullptr) {
      // Ref() the new NIB and get the network ID
      nib->ref();
      AbstractNetIO* netIO = nib->getNetIO();
      if (netIO != nullptr) netID = netIO->getNetworkID();
   }
   else {
      netID = 0;
   }
   return true;
}

// Sets the network output enabled flag
bool AbstractPlayer::setEnableNetOutput(const bool x)
{
   enableNetOutput = x;
   return true;
}

// Sets the outgoing NIB for network 'id'
bool AbstractPlayer::setOutgoingNib(AbstractNib* const p, const unsigned int id)
{
   bool ok = false;
   if (nibList != nullptr && id >= 1 && id <= AbstractNetIO::MAX_NETWORD_ID) {
      unsigned int idx = id - 1;
      if (nibList[idx] != nullptr) nibList[idx]->unref();
      nibList[idx] = p;
      if (nibList[idx] != nullptr) nibList[idx]->ref();
   }
   return ok;
}

//-----------------------------------------------------------------------------

// id: Player id  [ 1 .. 65535 ]
bool AbstractPlayer::setSlotID(const base::Number* const num)
{
   bool ok = false;
   int newID = num->getInt();
   if (newID > 0 && newID <= 65535) {
      setID( static_cast<unsigned short>(newID)  );
      ok = true;
   }
   else {
      std::cerr << "AbstractPlayer::setSlotID(): Invalid ID number: range 1 .. 65535" << std::endl;
   }
   return ok;
}

// mode: Initial player mode ( INACTIVE, ACTIVE, DEAD )
bool AbstractPlayer::setSlotInitMode(base::String* const msg)
{
   bool ok = false;
   if (*msg == "inactive" || *msg == "INACTIVE") { setInitMode(INACTIVE); ok = true; }
   else if (*msg == "active" || *msg == "ACTIVE") { setInitMode(ACTIVE); ok = true; }
   else if (*msg == "killed" || *msg == "KILLED") { setInitMode(KILLED); ok = true; }
   else if (*msg == "crashed" || *msg == "CRASHED") { setInitMode(CRASHED); ok = true; }
   else if (*msg == "detonated" || *msg == "DETONATED") { setInitMode(DETONATED); ok = true; }
   else if (*msg == "launched" || *msg == "LAUNCHED") { setInitMode(LAUNCHED); ok = true; }
   return ok;
}

std::ostream& AbstractPlayer::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
/*
   // initial player mode
   indent(sout,i+j);
   sout << "mode: ";
   if (initMode == ACTIVE)
      sout << "ACTIVE";
   else if (initMode == KILLED)
      sout << "KILLED";
   else if (initMode == CRASHED)
      sout << "CRASHED";
   else if (initMode == DETONATED)
      sout << "DETONATED";
   else if (initMode == LAUNCHED)
      sout << "LAUNCHED";
   else
      sout << "INACTIVE";
   sout << std::endl;
*/
   return sout;

}


}
}
