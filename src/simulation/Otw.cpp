#include "openeaagles/simulation/Otw.h"

#include "openeaagles/simulation/NetIO.h"
#include "openeaagles/simulation/Nib.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Weapon.h"

#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/osg/Vec4"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/units/Distances.h"
#include <cstring>

namespace Eaagles {
namespace Simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(Otw,"Otw")
EMPTY_SERIALIZER(Otw)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Otw)
    "maxRange",         // 1: Max range of visual system (distance: meters)
    "maxModels",        // 2: Max number of models
    "maxElevations",    // 3: Max number of terrain elevation requests
    "latitude",         // 4: Visual reference latitude (deg)
    "longitude",        // 5: Visual reference longitude (deg)
    "otwModelTypes",    // 6: OTW system's model type IDs (PairStream of Otm objects)
END_SLOTTABLE(Otw)

// Map slot table to handles
BEGIN_SLOT_MAP(Otw)
    ON_SLOT(1, setSlotMaxRange,      Basic::Distance)
    ON_SLOT(1, setSlotMaxRange,      Basic::Number)
    ON_SLOT(2, setSlotMaxModels,     Basic::Number)
    ON_SLOT(3, setSlotMaxElevations, Basic::Number)
    ON_SLOT(4, setSlotRefLatitude,   Basic::Number)
    ON_SLOT(5, setSlotRefLongitude,  Basic::Number)
    ON_SLOT(6, setSlotOtwModelTypes, Basic::PairStream)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator & clone()
//------------------------------------------------------------------------------
Otw::Otw()
{
    STANDARD_CONSTRUCTOR()

    ownship = nullptr;
    playerList = nullptr;
    rstFlg = false;
    rstReq = false;

    refLat  = 0.0;
    refLon  = 0.0;

    maxRange = 20000.0;          // Default: 20km

    maxModels = 0;               // Default: no models
    maxElevations = 0;           // Default: no elevation requests

    // Clear the tables
    for (unsigned int i = 0; i < MAX_MODELS; i++) {
        modelTbl[i] = nullptr;
    }
    nModels = 0;

    for (unsigned int i = 0; i < MAX_MODELS; i++) {
        hotTbl[i] = nullptr;
    }
    nHots = 0;


    for (unsigned int i = 0; i < MAX_MODELS_TYPES; i++) {
        otwModelTypes[i] = nullptr;
    }
    nOtwModelTypes = 0;

}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Otw::copyData(const Otw& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      ownship = nullptr;
      playerList = nullptr;
      for (unsigned int i = 0; i < MAX_MODELS; i++) {
         modelTbl[i] = nullptr;
      }
      nModels = 0;
      for (unsigned int i = 0; i < MAX_MODELS; i++) {
         hotTbl[i] = nullptr;
      }
      nHots = 0;
      for (unsigned int i = 0; i < MAX_MODELS_TYPES; i++) {
         otwModelTypes[i] = nullptr;
      }
      nOtwModelTypes = 0;
   }

    resetTables();

    clearOtwModelTypes();
    for (unsigned int i = 0; i < org.nOtwModelTypes; i++) {
       org.otwModelTypes[i]->ref();
       otwModelTypes[i] = org.otwModelTypes[i];
       nOtwModelTypes++;
    }

    maxRange = org.maxRange;
    maxModels = org.maxModels;
    maxElevations = org.maxElevations;
    rstFlg = org.rstFlg;
    rstReq = org.rstReq;

    // Posiiton
    refLat = org.refLat;
    refLon = org.refLon;

    setOwnship(org.ownship);
    setPlayerList(org.playerList);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Otw::deleteData()
{
   setOwnship(nullptr);
   setPlayerList(nullptr);
   resetTables();
   clearOtwModelTypes();
}

//------------------------------------------------------------------------------
// reset() -- Reset the visual system interface
//------------------------------------------------------------------------------
void Otw::reset()
{
    BaseClass::reset();
    setPlayerList(nullptr);
    rstReq = true;
}

//------------------------------------------------------------------------------
// resetTables() -- Resets all of the working tables
//------------------------------------------------------------------------------
void Otw::resetTables()
{
    // Clear the model table
    // (in reverse order just in case another thread is traversing the
    //  table from bottom up)
    while (nModels > 0) {
        removeModelFromList(nModels-1, MODEL_TABLE);
    }

    // Clear the elevation table
    // (in reverse order just in case another thread is traversing the
    //  table from bottom up)
    while (nHots > 0) {
        removeModelFromList(nHots-1, HOT_TABLE);
    }
}

//------------------------------------------------------------------------------
// clearOtwModelTypes() -- Clear the OTW model types table
//------------------------------------------------------------------------------
void Otw::clearOtwModelTypes()
{
   // Clear our old OTW model type table --
   // Just in case someone is scanning the table, we clear the entries
   // and decrement 'nOtwModelTypes' before the object is unref()'ed
   while (nOtwModelTypes > 0) {
      nOtwModelTypes--;
      otwModelTypes[nOtwModelTypes]->unref();
      otwModelTypes[nOtwModelTypes] = nullptr;
   }
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void Otw::updateTC(const LCreal dt)
{
    // Update base classes stuff
    BaseClass::updateTC(dt);

    // Check reset flag
    if (rstReq) {
        rstFlg = true;
        rstReq = false;
    }

    // update one visual system frame
    processesModels();
    processesElevations();
    frameSync();

    rstFlg = false;
}

//------------------------------------------------------------------------------
// processesModels() -- Process ownship & player models
//------------------------------------------------------------------------------
void Otw::processesModels()
{
    mapPlayerList2ModelTable();     // Map current player list to model table
    sendOwnshipAndModels();         // Send ownship model & model table
}

//------------------------------------------------------------------------------
// processesElevations() -- Process terrain elevation requests
//------------------------------------------------------------------------------
void Otw::processesElevations()
{
    recvElevations();               // Received previous elevation requests
    mapPlayers2ElevTable();         // Map new elevation requests
    sendElevationRequests();        // and send the new requests
}

//------------------------------------------------------------------------------
// mapPlayerList2ModelTable() - Map the player list to the model table
//
//  model states are:
//     INACTIVE     -- unused model entry
//     ACTIVE       -- player is alive and in-range
//     DEAD         -- player is dead or destroyed
//     OUT_OF_RANGE -- player is alive but out of range
//
//  Note: this routines will set model entries to DEAD and OUT_OF_RANGE, but the
//  derived class should handle the visual system unique termination sequences and
//  clear the model entry.
//------------------------------------------------------------------------------
void Otw::mapPlayerList2ModelTable()
{
   // ---
   // Check for reset
   // ---
   if (isResetInProgress()) {
      // Set all active models as Out-Of-Range so that sendOwnshipAndModels() can remove them
      for (unsigned int i = 0; i < getModelTableSize(); i++) {
         modelTbl[i]->setState( OtwModel::OUT_OF_RANGE );
      }
      return;
   }

   // ---
   // Remove all inactive, dead or out-of-range models
   //   -- These states were issued last pass, so the OTW system
   //       specific software should have handled them by now.
   //   -- As models are removed, the table above the model is shifted down.
   //   -- We're also clearing the model's 'checked' flag
   // ---
   for (int i = getModelTableSize(); i > 0; --i) {
      if ( modelTbl[i-1]->isState(OtwModel::CLEARED) ) {
         // Deleting this model
         //std::cout << "Otw::mapPlayerList2ModelTable() cleanup: model = " << modelTbl[i] << std::endl;
         removeModelFromList( (i-1), MODEL_TABLE);
      }
   }
   for (unsigned int i = 0; i < getModelTableSize(); i++) {
      modelTbl[i]->setCheckedFlag(false);
   }

   if (playerList != nullptr) {
      // We must have a player list ...

      // ---
      // Find players that are alive and within range of the visual system ...
      // ---
      Basic::List::Item* item = playerList->getFirstItem();
      while (item != nullptr) {

         // Get a pointer to the player, 'p'
         Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
         Player* p = static_cast<Player*>(pair->object());

         bool dummy = false;
         const Weapon* wpn = dynamic_cast<const Weapon*>( p );
         if (wpn != nullptr) dummy = wpn->isDummy();

         if ( p != getOwnship() && !dummy ) {

            // Find the player's model entry (if any)
            OtwModel* model = findModel(p, MODEL_TABLE);

            // Check if in-range
            bool inRange = computeRangeToPlayer(p) <= maxRange;

            // Check if this player is alive and within range.
            if (p->isActive() && inRange) {
               // When alive and in range ...
               if (model != nullptr) {
                  // a) and it already has a model entry: make sure it's active ...
                  model->setState( OtwModel::ACTIVE );
               }
               else {
                  // b) and it doesn't have a model entry (new, in-range player) ...
                  model = newModelEntry(p);
               }
            }
            else if (p->isDead() && inRange) {
               // When player isn't alive and it had a model entry
               if (model != nullptr) {
                  // set state to dead
                  model->setState( OtwModel::DEAD );
               }
            }
            else {
               // When player is out-of-range and it had a model entry
               if (model != nullptr) {
                  // set state to out-of-range
                  model->setState( OtwModel::OUT_OF_RANGE );
               }
            }
            if (model != nullptr) model->setCheckedFlag(true);

         }

         item = item->getNext(); // Next player
      }

   }

   // ---
   // Any models not checked needs to be removed
   // ---
   for (unsigned int i = 0; i < getModelTableSize(); i++) {
      if ( modelTbl[i]->isNotChecked() ) {
         // Request removal;
         // (note: the OTW system specific code now has one frame to cleanup its own code
         //  before the model is dropped from the output list next frame -- see above)
         modelTbl[i]->setState( OtwModel::OUT_OF_RANGE );
      }
   }

}

//------------------------------------------------------------------------------
// mapPlayers2ElevTable() - Map the player list to the model table
//------------------------------------------------------------------------------
void Otw::mapPlayers2ElevTable()
{
   // Set all entries as unchecked
   for (unsigned int i = 0; i < getElevationTableSize(); i++) {
      hotTbl[i]->setCheckedFlag(false);
   }

   // When we have a player list ...
   if (playerList != nullptr) {

      // ---
      // Find players that are alive and require terrain elevation from the visual system ...
      // ---
      Basic::List::Item* item = playerList->getFirstItem();
      while (item != nullptr) {

         // Get a pointer to the player, 'p'
         Basic::Pair* pair = static_cast<Basic::Pair*>(item->getValue());
         Player* p = static_cast<Player*>(pair->object());

         // Check if this player is alive and within range.
         if ( p->isActive() && p->isTerrainElevationRequired() ) {

            // Check if in-range
            bool inRange = computeRangeToPlayer(p) <= maxRange;

            if (inRange) {

               // Find the player's model entry (if any)
               OtwModel* model = findModel(p, HOT_TABLE);

               if (model != nullptr) {
                  // The player has a valid entry.
                  model->incReqCount();
               }
               else {
                  // Player doesn't have an entry, so create one.
                  model = newElevEntry(p);
               }
               if (model != nullptr) model->setCheckedFlag(true);
            }

         }

         //completed  = p->isNetworkedPlayer();
         item = item->getNext(); // Next player
      }
   }

   // ---
   // Remove unmatched model entries; their players are inactive or no longer
   // require terrain elevation
   // ---
   for (int i = getElevationTableSize(); i > 0; --i) {
      if ( hotTbl[i-1]->isNotChecked() ) {
         // Deleting this entry
         removeModelFromList( (i-1), HOT_TABLE);
      }
   }
}

//------------------------------------------------------------------------------
// computeRangeToPlayer() -- Calculate range from ownship to player
//------------------------------------------------------------------------------
LCreal Otw::computeRangeToPlayer(const Player* const ip) const
{
    LCreal rng = maxRange*2.0 + 1.0;  // Default is out-of-range
    if (ownship != nullptr) {
        osg::Vec3 diff = ip->getPosition() - ownship->getPosition();
        rng = diff.length();
    }
    return rng;
}

//------------------------------------------------------------------------------
// newModelEntry() -- Generates a new model entry for this player.
//                    Returns a pointer to the new entry, else zero(0)
//------------------------------------------------------------------------------
OtwModel* Otw::newModelEntry(Player* const ip)
{
   OtwModel* model = nullptr;

   // Only if we have a player pointer ...
   if (ip != nullptr && (getModelTableSize() < getMaxModels())) {

      // Create a model entry for this player
      model = modelFactory();
      if (model != nullptr) {
         // Yes, initialize the model entry
         model->initialize(ip, otwModelTypes, nOtwModelTypes);
         addModelToList(model, MODEL_TABLE);
      }
   }

   return model;
}

//------------------------------------------------------------------------------
// newElevEntry() -- Generates a new elevation entry for this player
//                    Returns a pointer to the new entry, else zero(0)
//------------------------------------------------------------------------------
OtwModel* Otw::newElevEntry(Player* const ip)
{
   OtwModel* model = nullptr;

   // Only if we have a player pointer ...
   if (ip != nullptr && (getElevationTableSize() < getMaxElevations())) {

      // Create a model entry for this player
      model = hotFactory();
      if (model != nullptr) {
         // Yes, initialize the model entry
         model->initialize(ip);
         addModelToList(model, HOT_TABLE);
      }
   }

   return model;
}

//------------------------------------------------------------------------------
// Sets our ownship pointer; public version, which is usually called by
// the Station class.  Derived versions of Otw can override this function
// and control the switch of ownship using setOwnship0()
//------------------------------------------------------------------------------
void Otw::setOwnship(Player* const newOwnship)
{
   setOwnship0(newOwnship);
}

//------------------------------------------------------------------------------
// Sets our ownship player (for derived class control)
//------------------------------------------------------------------------------
void Otw::setOwnship0(Player* const newOwnship)
{
    // Nothing's changed, just return
    if (ownship == newOwnship) return;

    // Unref() the old, set and ref() the new
    if (ownship != nullptr) ownship->unref();
    ownship = newOwnship;
    if (ownship != nullptr) ownship->ref();
}

//------------------------------------------------------------------------------
// setPlayerList() -- Sets our player list pointer
//------------------------------------------------------------------------------
void Otw::setPlayerList(Basic::PairStream* const newPlayerList)
{
    // Nothing's changed, just return
    if (playerList == newPlayerList) return;

    // Unref() the old, set and ref() the new
    if (playerList != nullptr) playerList->unref();
    playerList = newPlayerList;
    if (playerList != nullptr) playerList->ref();
}

//------------------------------------------------------------------------------
// setMaxRange() -- sets the max visual range
//------------------------------------------------------------------------------
bool Otw::setMaxRange(const LCreal r)
{
    maxRange = lcAbs(r);
    return true;
}

//------------------------------------------------------------------------------
// setMaxModels() -- sets the maximum number of models accepted by the visual
//------------------------------------------------------------------------------
bool Otw::setMaxModels(const unsigned int n)
{
    if (n <= MAX_MODELS) maxModels = n;
    else maxModels = MAX_MODELS;
    resetTables();
    return true;
}

//------------------------------------------------------------------------------
// setMaxElevations() -- sets the maximum number of elevation requests accepted by the visual
//------------------------------------------------------------------------------
bool Otw::setMaxElevations(const unsigned int n)
{
    if (n <= MAX_MODELS) maxElevations = n;
    else maxElevations = MAX_MODELS;
    resetTables();
    return true;
}

//------------------------------------------------------------------------------
// addModelToList() -- adds a model to the quick access table
//------------------------------------------------------------------------------
bool Otw::addModelToList(OtwModel* const model, const TableType type)
{
   bool ok = false;
   if (model != nullptr) {

      // Select the table
      OtwModel** tbl = modelTbl;
      int n = nModels;
      int max = maxModels;
      if (type == HOT_TABLE) {
         tbl = hotTbl;
         n = nHots;
         max = maxElevations;
      }

      // If there's room for one more ...
      if (n < max) {

         // Put the model on the top of the table
         model->ref();
         tbl[n] = model;

         // Create a key for this new NIB
         OtwModelKey key(model->getPlayerID(), model->getFederateName());

         if (n > 0) {
            // Now, 'bubble down' to its correct position
            int idx = n-1;
            while (idx >= 0 && compareKey2Model(&key, &tbl[idx]) <= 0) {
               // Swap the table entries
               OtwModel* tmp = tbl[idx];
               tbl[idx] = tbl[idx+1];
               tbl[idx+1] = tmp;
               idx--;
            }
         }

         // Increment the count
         if (type == HOT_TABLE) nHots++;
         else nModels++;

         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// removeModelFromList() -- removes a model from the quick access table
//------------------------------------------------------------------------------
void Otw::removeModelFromList(const int idx, const TableType type)
{
   // Select the table size
   int n = nModels;
   if (type == HOT_TABLE) {
      n = nHots;
   }

   // If a valid index ...
   if (idx >= 0 && idx < n) {

      // Select the table
      OtwModel** tbl = modelTbl;
      if (type == HOT_TABLE) {
         tbl = hotTbl;
      }

      // Remember the model
      OtwModel* model = tbl[idx];

      // Shift down all items above this index by one position
      int n1 = (n - 1);
      for (int i = idx; i < n1; i++) {
         tbl[i] = tbl[i+1];
      }

      // Decrement the count
      if (type == HOT_TABLE) --nHots;
      else --nModels;

      // clear the last pointer
      tbl[n-1] = nullptr;

      // Unref the model
      model->unref();
   }
}

void Otw::removeModelFromList(OtwModel* const model, const TableType type)
{
   OtwModel** tbl = modelTbl;
   int n = nModels;
   if (type == HOT_TABLE) {
      tbl = hotTbl;
      n = nHots;
   }

   int found = -1;
   // Find the model
   for (int i = 0; i < n && found < 0; i++) {
      if (model == tbl[i]) found = i;
   }

   // If the entry was found ...
   if (found >= 0) {

      // Shift down all items above this model by one position
      int n1 = (n - 1);
      for (int i = found; i < n1; i++) {
         tbl[i] = tbl[i+1];
      }

      // Decrement the count
      if (type == HOT_TABLE) --nHots;
      else --nModels;

      // clear the last pointer
      tbl[n-1] = nullptr;

      // Unref the model
      model->unref();
   }
}

//------------------------------------------------------------------------------
// findModel() -- find the model that matches ALL IDs.
//------------------------------------------------------------------------------
OtwModel* Otw::findModel(const unsigned short playerID, const Basic::String* const federateName, const TableType type)
{
   // Define the key
   OtwModelKey key(playerID, federateName);

   // Binary search the table for the models
   OtwModel* found = nullptr;
   if (type == HOT_TABLE) {
      OtwModel** k =
         static_cast<OtwModel**>(bsearch(&key, hotTbl, nHots, sizeof(OtwModel*), compareKey2Model));
      if (k != nullptr) found = *k;
   }
   else {
      OtwModel** k =
         static_cast<OtwModel**>(bsearch(&key, modelTbl, nModels, sizeof(OtwModel*), compareKey2Model));
      if (k != nullptr) found = *k;
   }
   return found;
}

OtwModel* Otw::findModel(const Player* const player, const TableType type)
{
   OtwModel* found = nullptr;
   if (player != nullptr) {
      // Get the player's IDs
      const Basic::String* fName = nullptr;
      if (player->isNetworkedPlayer()) {
         // If networked, used original IDs
         const Nib* pNib = player->getNib();
         fName = pNib->getFederateName();
      }
      // Now find the model using the player's IDs
      found = findModel(player->getID(), fName, type);
   }
   return found;
}

//------------------------------------------------------------------------------
// bsearch callbacks: object name compare function --
//   True types are (const OtwModelKey* key, const OtwModel** model)
//------------------------------------------------------------------------------
int Otw::compareKey2Model(const void* key, const void* model)
{
   // The Key
   const OtwModelKey* pKey = static_cast<const OtwModelKey*>(key);

   // The NIB
   const OtwModel* const* pp = static_cast<const OtwModel* const*>(model);
   const OtwModel* pModel = *pp;

   // Default to equal
   int result = 0;

   // Compare player IDs
   if (pKey->playerID < pModel->getPlayerID()) result = -1;
   else if (pKey->playerID > pModel->getPlayerID()) result = +1;

   if (result == 0) {
      // If they're the same playr IDs, compare the federate names
      const Basic::String* pKeyFedName = pKey->fName;
      const Basic::String* pModelFedName = pModel->getFederateName();

      if (pKeyFedName == nullptr && pModelFedName != nullptr) result = -1;

      else if (pKeyFedName != nullptr && pModelFedName == nullptr) result = +1;

      else if (pKeyFedName != nullptr && pModelFedName != nullptr) {
         result = std::strcmp(*pKeyFedName, *pModelFedName);
      }
   }

   return result;
}

//------------------------------------------------------------------------------
// True if visual system is resetting
//------------------------------------------------------------------------------
bool Otw::isResetInProgress() const
{
   return rstFlg;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------
bool Otw::setRefLatitude(const double v)
{
    bool ok = (v <= 90.0 && v >= -90.0);
    if (ok) refLat = v;
    return ok;
}

bool Otw::setRefLongitude(const double v)
{
    bool ok = (v <= 180.0 && v >= -180.0);
    if (ok) refLon = v;
    return ok;
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

// setSlotMaxRange() -- sets the maxRange slot
bool Otw::setSlotMaxRange(const Basic::Distance* const msg)
{
    bool ok = false;

    if (msg != nullptr) {
        // We have a distance which we can convert to meters
        const LCreal rng = Basic::Meters::convertStatic(*msg);
        ok = setMaxRange( rng );
    }

    if (!ok) {
        // error -- invalid range
        std::cerr << "Otw::setSlotMaxRange: invalid maximum range" << std::endl;
    }
    return ok;
}

// setSlotMaxRange() -- sets the maxRange slot
bool Otw::setSlotMaxRange(const Basic::Number* const msg)
{
    bool ok = false;

    if (msg != nullptr) {
        // We have a simple number, which should be meters!
        ok = setMaxRange(msg->getReal());
    }

    if (!ok) {
        // error -- invalid range
        std::cerr << "Otw::setSlotMaxRange: invalid maximum range" << std::endl;
    }
    return ok;
}

// setSlotMaxModels() -- sets the max number of models slot
bool Otw::setSlotMaxModels(const Basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        const int n = num->getInt();
        if (n >= 0) {
             ok = setMaxModels( static_cast<unsigned int>(n) );
        }
        if (!ok) {
            std::cerr << "Otw::setSlotMaxModels: maximum number of models limited to Otw::MAX_MODELS1" << std::endl;
        }
    }
    return ok;
}

bool Otw::setSlotMaxElevations(const Basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        const int n = num->getInt();
        if (n >= 0) {
             ok = setMaxElevations( static_cast<unsigned int>(n) );
        }
        if (!ok) {
            std::cerr << "Otw::setSlotMaxElevations: maximum number of terrain elevation requests limited to Otw::MAX_ELEV" << std::endl;
        }
    }
    return ok;
}

// latitude: Ref latitude (deg)
bool Otw::setSlotRefLatitude(const Basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        ok = setRefLatitude(num->getDouble());
    }
    return ok;
}

// longitude: Ref longitude (deg)
bool Otw::setSlotRefLongitude(const Basic::Number* const num)
{
    bool ok = false;
    if (num != nullptr) {
        ok = setRefLongitude(num->getDouble());
    }
    return ok;
}

// Sets the list of OTW model type IDs (Otm objects)
bool Otw::setSlotOtwModelTypes(const Basic::PairStream* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
       // First clear the old table
       clearOtwModelTypes();

       // Now scan the pair stream and put all Otm objects
       // into the table.
       const Basic::List::Item* item = msg->getFirstItem();
       while (item != nullptr && nOtwModelTypes < MAX_MODELS_TYPES) {
          const Basic::Pair* pair = static_cast<const Basic::Pair*>(item->getValue());
          const Otm* otwType = dynamic_cast<const Otm*>( pair->object() );
          if (otwType != nullptr) {
             // We have an Otm object, so put it in the table
             otwType->ref();
             otwModelTypes[nOtwModelTypes] = otwType;
             nOtwModelTypes++;
          }
          item = item->getNext();
       }
       ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Otw::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//==============================================================================
// Otw::OtwModelKey class
//==============================================================================
Otw::OtwModelKey::OtwModelKey(const unsigned short pid, const Basic::String* const federateName)
{
   playerID = pid;
   fName = federateName;
}

//==============================================================================
// OtwModel class
//==============================================================================

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(OtwModel,"OtwModel")
EMPTY_SERIALIZER(OtwModel)

// ---
// constructor
// ---
OtwModel::OtwModel() : player(nullptr), federateName(nullptr)
{
   STANDARD_CONSTRUCTOR()

   clear();
}

// ---
// copyData() -- copy member data
// ---
void OtwModel::copyData(const OtwModel& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) {
        player = nullptr;
    }

    setPlayer(org.player);

    state = org.state;
    ageCount = org.ageCount;
    checked = org.checked;
    typeMapper = org.typeMapper;
    rcount = org.rcount;
    hotActive = org.hotActive;

    playerID = org.playerID;

    const Basic::String* pp = org.federateName;
    federateName = pp;
}

// ---
// deleteData() -- delete member data
// ---
void OtwModel::deleteData()
{
    setPlayer(nullptr);
}

// ---
// OtwModel() -- set & ref the player pointer
// ---
void OtwModel::setPlayer(Player* const p)
{
   if (player != nullptr) {
      player->unref();
      playerID = 0;
      federateName = nullptr;
   }

   player = p;

   if (player != nullptr) {
      player->ref();
      playerID = player->getID();
      const Nib* nib = player->getNib();
      if (nib != nullptr) {
         federateName = nib->getFederateName();
      }
      else {
         federateName = nullptr;
      }
   }
}

// ---
// initialize() -- initialize the model
// ---
void OtwModel::initialize(Player* const p, const Otm** const otwModelTable, const unsigned int numModels)
{
   setPlayer(p);
   state = OtwModel::ACTIVE;
   ageCount = 999;
   hotActive = true;
   rcount = 999;
   checked = true;

   // If the OTW model table was provided, then look for a match.
   if (otwModelTable != nullptr && numModels > 0) {
      bool found = false;
      for (unsigned int i = 0; i < numModels && !found; i++) {
         const Otm* otwTypeMapper = otwModelTable[i];
         if (otwTypeMapper != nullptr) {
            if (otwTypeMapper->isMatchingPlayerType(p)) {
               // We found a match for our player in the OTW model table!
               typeMapper = otwTypeMapper;
               found = true;
            }
         }
      }
   }
}

// ---
// clear() -- clears out the model
// ---
void OtwModel::clear()
{
   setPlayer(nullptr);
   state = INACTIVE;
   ageCount = 0;
   checked = false;
   typeMapper = nullptr;
   rcount = 0;
   hotActive = false;
   playerID = 0;
   federateName = nullptr;
}


//==============================================================================
// Otm class
//==============================================================================
IMPLEMENT_SUBCLASS(Otm,"Otm")
EMPTY_SERIALIZER(Otm)

//------------------------------------------------------------------------------
// slot table for this class type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Otm)
   "formName",       // 1) Reference form name
   "typeName",       // 2) Reference type name
   "modelTypeId",    // 3) OTW entity type ID number
END_SLOTTABLE(Otm)

// Map slot table to handles
BEGIN_SLOT_MAP(Otm)
    ON_SLOT(1, setSlotRefFormName, Basic::Identifier)
    ON_SLOT(2, setSlotRefTypeName, Basic::String)
    ON_SLOT(3, setSlotTypeId,      Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Otm::Otm()
{
   STANDARD_CONSTRUCTOR()

   refFormName = nullptr;
   refTypeName = nullptr;
   typeId = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void Otm::copyData(const Otm& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      refFormName = nullptr;
      refTypeName = nullptr;
   }

   setTypeId( org.typeId );
   setSlotRefFormName( org.refFormName );
   setSlotRefTypeName( org.refTypeName );
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Otm::deleteData()
{
   setTypeId( 0 );
   setSlotRefFormName( nullptr );
   setSlotRefTypeName( nullptr );
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the OTW entity type number
bool Otm::setTypeId(const unsigned int newType)
{
   typeId = newType;
   return true;
}

// Sets the OTW entity type number
bool Otm::setSlotTypeId(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      const int i = msg->getInt();
      if (i >= 0) {
         ok = setTypeId( static_cast<unsigned int>(i) );
      }
   }
   return ok;
}

// Sets the player's form name
bool Otm::setSlotRefFormName(const Basic::Identifier* const msg)
{
   refFormName = msg;
   return true;
}

// Sets the player's type name
bool Otm::setSlotRefTypeName(const Basic::String* const msg)
{
   refTypeName = msg;
   return true;
}

//------------------------------------------------------------------------------
// isMatchingPlayerType() -- Returns true if the form & type names match
//------------------------------------------------------------------------------
bool Otm::isMatchingPlayerType(const Player* const p) const
{
   bool match = false;
   if (p != nullptr && refFormName != nullptr) {
      // first match the form name --
      if (p->isFactoryName( *refFormName ) ) {

         // The form names match!

         // By default we match, unless we have both type names to match,
         match = true;

         // Do we have both type names?
         const Basic::String* ptype = p->getType();
         if ( refTypeName != nullptr && ptype != nullptr) {

            // Then compare at most the length of our reference type name ...
            match = std::strncmp( ptype->getString(), refTypeName->getString(), refTypeName->len() ) == 0;

         }
      }
   }
   return match;
}


//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Otm::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace
