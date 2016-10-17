//------------------------------------------------------------------------------
//        This is a limited capability:  the stabilizing gimbal can
//        only mounted on the nose or the tail or on the y-azis on
//        the left or right wing.
//------------------------------------------------------------------------------

#include "openeaagles/simulation/StabilizingGimbal.hpp"

#include "openeaagles/simulation/Player.hpp"
#include "openeaagles/simulation/Simulation.hpp"

#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/List.hpp"
#include "openeaagles/base/functors/Tables.hpp"
#include "openeaagles/base/Integer.hpp"
#include "openeaagles/base/Decibel.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Powers.hpp"
#include "openeaagles/base/osg/Vec3"

namespace oe {
namespace simulation {

IMPLEMENT_SUBCLASS(StabilizingGimbal,"StabilizingGimbal")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(StabilizingGimbal)
    "stabilizingMode",      //  4: Sets the type of stabilization we desire  (elevation, roll, horizon)
END_SLOTTABLE(StabilizingGimbal)

// Map slot table to handles
BEGIN_SLOT_MAP(StabilizingGimbal)
    ON_SLOT(1,setSlotStabilizingMode,base::String);
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
StabilizingGimbal::StabilizingGimbal()
{
   STANDARD_CONSTRUCTOR()

   // Defaults
   stabilizingMode = HORIZON;
   mountPosition = NOSE;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void StabilizingGimbal::copyData(const StabilizingGimbal& org, const bool)
{
   BaseClass::copyData(org);

   stabilizingMode = org.stabilizingMode;
   mountPosition = org.mountPosition;;
}

void StabilizingGimbal::deleteData()
{
}

//------------------------------------------------------------------------------
// dynamics() -- System class "Dynamics phase" call back
//------------------------------------------------------------------------------
void StabilizingGimbal::dynamics(const double dt)
{
   stabilizingController(dt);

   // Call BaseClass after to scan controller since the servo controller
   // is located in BaseClass.
   BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// stabilizingController() -- control the gimbal scanning
//------------------------------------------------------------------------------
void StabilizingGimbal::stabilizingController(const double dt)
{
    switch (stabilizingMode) {
        case ROLL : { rollStabilizingController(dt); }
            break;

        case ELEVATION : { elevationStabilizingController(dt); }
            break;

        case HORIZON : { rollStabilizingController(dt);  elevationStabilizingController(dt);}
            break;
    }
}

//------------------------------------------------------------------------------
// rollStabilizingController() -- Roll stabilizes the gimbal
//------------------------------------------------------------------------------
void StabilizingGimbal::rollStabilizingController(const double)
{
    if (getOwnship() == nullptr) return;

    osg::Vec3 tpos = getCmdPosition();
    if (mountPosition == NOSE){
        tpos[ROLL_IDX] = static_cast<double>(-getOwnship()->getRoll());
    }
    else if (mountPosition == TAIL){
        tpos[ROLL_IDX] = static_cast<double>(getOwnship()->getRoll());
    }
    else if (mountPosition == RIGHT_WING){
        tpos[ELEV_IDX] = static_cast<double>(-getOwnship()->getPitch());
    }
    else if (mountPosition == LEFT_WING){
        tpos[ELEV_IDX] = static_cast<double>(getOwnship()->getPitch());
    }
    setCmdPos( tpos );
}

//------------------------------------------------------------------------------
// elevationStabilizingController() -- Elevation stabilizes the gimbal
//------------------------------------------------------------------------------
void StabilizingGimbal::elevationStabilizingController(const double)
{
    if (getOwnship() == nullptr) return;

    osg::Vec3 tpos = getCmdPosition();
    if (mountPosition == NOSE){
        tpos[ELEV_IDX] = static_cast<double>(-getOwnship()->getPitch());
    }
    else if (mountPosition == TAIL){
        tpos[ELEV_IDX] = static_cast<double>(getOwnship()->getPitch());
    }
    else if (mountPosition == RIGHT_WING){
        tpos[ROLL_IDX] = static_cast<double>(-getOwnship()->getRoll());
    }
    else if (mountPosition == LEFT_WING){
        tpos[ROLL_IDX] = static_cast<double>(getOwnship()->getRoll());
    }
    setCmdPos( tpos );
}

//------------------------------------------------------------------------------
// setStabilizingMode() - sets the gimbal scan pattern
//------------------------------------------------------------------------------
bool StabilizingGimbal::setStabilizingMode(const StabilizingMode m)
{
   stabilizingMode = m;
   return true;
}

//------------------------------------------------------------------------------
// setStabilizingMode() - sets the gimbal scan pattern
//------------------------------------------------------------------------------
bool StabilizingGimbal::setMountPosition(const MountPosition m)
{
   mountPosition = m;
   return true;
}

//------------------------------------------------------------------------------
// setSlotStabilizingMode() -- calls setStabalizingMode()
//------------------------------------------------------------------------------
bool StabilizingGimbal::setSlotStabilizingMode(base::String* const msg)
{
    // set our scan mode
    bool ok = true;
    if (msg != nullptr) {
        if (*msg == "roll") ok = setStabilizingMode(ROLL);
        else if (*msg == "elevation") ok = setStabilizingMode(ELEVATION);
        else if (*msg == "horizon") ok = setStabilizingMode(HORIZON);
        else ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMountPosition() -- calls setMountPosition()
//------------------------------------------------------------------------------
bool StabilizingGimbal::setSlotMountPosition(base::String* const msg)
{
    // set our scan mode
    bool ok = true;
    if (msg != nullptr) {
        if (*msg == "nose") ok = setMountPosition(NOSE);
        else if (*msg == "tail") ok = setMountPosition(TAIL);
        else if (*msg == "left") ok = setMountPosition(LEFT_WING);
        else if (*msg == "right") ok = setMountPosition(RIGHT_WING);
        else ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
base::Object* StabilizingGimbal::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& StabilizingGimbal::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

}
}
