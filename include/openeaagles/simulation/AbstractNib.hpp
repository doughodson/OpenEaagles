
#ifndef __oe_simulation_AbstractNib_H__
#define __oe_simulation_AbstractNib_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base { class Vec3d; }
namespace simulation {
class AbstractNetIO;

//------------------------------------------------------------------------------
// Class: AbstractNib
//
// Description: Abstract Network Interface Block (Nib)
//
//    This class is used to manage the relationship between players and their
//    corresponding entities on the interoperability networks (e.g., DIS, HLA).
//
//    Each Nib object manages either an incoming network entity to player
//    relationship or an outgoing player to network entity relationship.
//    The I/O type, which is either NetIO::INPUT_NIB or NetIO::OUTPUT_NIB, is
//    set by the constructor and is accessed using the getIoType() function.
//
//    The parent NetIO class object maintains two lists of Nib objects: one to
//    manage the incoming entities and a second to manage the outgoing players.
//    To create Nib objects, NetIO uses a NIB factory function, nibFactory(),
//    which is implemented by the NetIO network specific derived class.
//
//    Each Nib maintains pointers to their parent NetIO object and to the Player
//    object that they're managing.  The remainder of the data in this Nib class
//    is common data used by most interoperability network entities.  Additional
//    entity data is added by the network specific classes derived from Nib.
//
// Factory name: AbstractNib
//------------------------------------------------------------------------------
class AbstractNib : public base::Component
{
   DECLARE_SUBCLASS(AbstractNib, base::Component)

public:
   AbstractNib();

   virtual const base::String* getFederateName() const =0;    // federate name

   virtual unsigned short getPlayerID() const =0;

   virtual AbstractNetIO* getNetIO() =0;

   // update incoming entity dead reckoning
   virtual bool updateDeadReckoning(
      const double dt,                    // delta time (sec)
      base::Vec3d* const pNewPos,         // New DR position
      base::Vec3d* const pNewAngles       // New DR orientation
   ) =0;

   // DR's velocity vector @ T0 (m/sec)  (ECEF or Body based on the DR algorithm)
   virtual const base::Vec3d& getDrVelocity() const =0;

   // DR's acceleration vector @ T0 ((m/sec)/sec) (ECEF or Body based on the DR algorithm)
   virtual const base::Vec3d& getDrAcceleration() const =0;

   // DR's angular rates @ T0 (rad/sec)  [ phi theta psi ] (Body/ECEF)
   virtual const base::Vec3d& getDrAngularVelocities() const =0;
};

}
}

#endif
