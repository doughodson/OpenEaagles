//------------------------------------------------------------------------------
// Class: Ntm
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Ntm_H__
#define __oe_simulation_Ntm_H__

#include "openeaagles/simulation/NetIO.h"

namespace oe {
namespace simulation {
class Nib;

//------------------------------------------------------------------------------
// Class: Ntm
// Description: Abstract Network Type Mapper (NTM) class
//
//    This class is used to map oe player types to the entity type codes
//    that are unique to an interoperability network (i.e., DIS, HLA/FOM, etc).
//
//    The oe player type is defined by providing an example Player using
//    the 'template' slot.  On incoming entities, the 'template' player is cloned
//    to create the surrogate, "Interoperability Player" (or IPlayer) that is
//    added to the simulation players list.
//
//    Network specific classes, which are derived from this class, are used to
//    define the network specific entity types.
//
//    The NetIO class will manage two lists of these Ntm objects; one to map
//    incoming entity type codes to player types, and a second to map outgoing
//    player types to network entity codes.  These lists are provided as inputs
//    to the NetIO object using the 'inputEntityTypes' and 'outputEntityTypes'
//    slots.  For examples of these lists, see  the files 'DisIncomingEntityTypes.epp'
//    and 'DisOutgoingEntityTypes.epp' in the directory "OpenEaaglesExamples/shared/data".
//
//
// Slots:
//     template   <Player>   ! Template player (default: 0)
//
//
// Notes:
//    1) When matching a target player's type to the Ntm template players ...
//       a) Using form names, the target player's class is matched with the
//          template player's class.
//       b) Given a template player with a matching class (or base class), the
//          template player's type string (see function Player::getType) is
//          compared to the target player's type string and the template
//          player's string will match as a substring of the target player.
//
//       Example:  Give an Aircraft player with type string "F-16C"
//
//          template: ( Aircraft type: "F-16C" )   /* #1, match: exact */
//          template: ( Aircraft type: "F-16" )    /* #2, match: as substring */
//          template: ( Aircraft type: "F-16A" )   /* #3, no match: wrong type string */
//          template: ( Aircraft type: "F-16C1" )  /* #4, no match: template type string is too long. */
//          template: ( AirVehicle type: "F-16C" ) /* #5, match: matching subclass */
//          template: ( AirVehicle type: "F-16" )  /* #6, match: matching subclass; type as substring */
//          template: ( Ship type: "F-16C" )       /* #7, no match: wrong class type */
//
//          Best match is returned: #1 (best), #2, #5 and #6 (worst)
//
//------------------------------------------------------------------------------
class Ntm : public base::Object
{
    DECLARE_SUBCLASS(Ntm,base::Object)

public:
   Ntm();

   // Template player
   const Player* getTemplatePlayer() const    { return tPlayer; }

   // This network specific function is used to copy the entity
   // type codes to a target NIB object.
   virtual bool copyEntityType(Nib* const targetNib) const =0;

   // Slot functions
   virtual bool setSlotTemplatePlayer(const Player* const msg);     // Sets the template player

private:
   base::safe_ptr<const Player>  tPlayer; // Template player
};

} // End simulation namespace
}

#endif
