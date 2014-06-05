
#include "openeaagles/vehicles/RacModel.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/osg/Matrix"
#include "openeaagles/basic/osg/Vec3"
#include "openeaagles/basic/osg/Quat"

namespace Eaagles {
namespace Vehicle {

IMPLEMENT_SUBCLASS(RacModel,"RacModel")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(RacModel)
    "minSpeed",    // 1 Minimum Velocity        (kts)
    "speedMaxG",   // 2 Velocity we reach max G (kts)
    "maxg",        // 3 Max G's (at "speedMaxG" or above)
    "maxAccel",    // 4 Maximum Acceleration    (m/s/s)
    "cmdAltitude", // 5 Command Altitude
    "cmdHeading",  // 6 Command Heading
    "cmdSpeed",    // 7 Command speed 
END_SLOTTABLE(RacModel)

//------------------------------------------------------------------------------
// Slot Map
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(RacModel)
    ON_SLOT( 1, setSlotMinSpeed, Basic::Number)
    ON_SLOT( 2, setSlotSpeedMaxG, Basic::Number)
    ON_SLOT( 3, setSlotMaxG,Basic::Number)
    ON_SLOT( 4, setSlotMaxAccel, Basic::Number)
    ON_SLOT( 5, setSlotCmdAltitude, Basic::Distance)
    ON_SLOT( 6, setSlotCmdHeading, Basic::Angle)
    ON_SLOT( 7, setSlotCmdVelocity, Basic::Number)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
RacModel::RacModel()
{
   STANDARD_CONSTRUCTOR()

   vpMin = 0.0;
   vpMaxG = 250.0;
   gMax = 4.0;
   maxAccel = 10.0;

   cmdAltitude = -9999.0;
   cmdHeading = -9999.0;
   cmdVelocity = -9999.0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void RacModel::copyData(const RacModel& org, const bool)
{
   BaseClass::copyData(org);

   vpMin = org.vpMin;
   vpMaxG = org.vpMaxG;
   gMax = org.gMax;
   maxAccel = org.maxAccel;

   cmdAltitude = org.cmdAltitude;
   cmdHeading = org.cmdHeading;
   cmdVelocity = org.cmdVelocity;
}

EMPTY_DELETEDATA(RacModel)


//------------------------------------------------------------------------------
// reset() -- 
//------------------------------------------------------------------------------
void RacModel::reset()
{
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void RacModel::dynamics(const LCreal dt)
{
    updateRAC(dt);
    BaseClass::dynamics(dt);
}


//------------------------------------------------------------------------------
// Get Vehicle data
//------------------------------------------------------------------------------
LCreal RacModel::getGload() const
{
   return -1;
}

LCreal RacModel::getMach() const
{
   return 0.5;
}

LCreal RacModel::getAngleOfAttack() const
{
   return 0.0;
}

LCreal RacModel::getSideSlip() const
{
   return 0.0;
}

LCreal RacModel::getFlightPath() const
{
   const Simulation::Player* pp = static_cast<const Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   if (pp == 0) return 0;
   return static_cast<LCreal>(pp->getPitchR());
}

LCreal RacModel::getCalibratedAirspeed() const
{
   const Simulation::Player* pp = static_cast<const Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   if (pp == 0) return 0;
   return pp->getTotalVelocityKts();
}


//------------------------------------------------------------------------------
// Autopilot controls
//------------------------------------------------------------------------------
bool RacModel::isHeadingHoldOn() const
{
   return true;
}

double RacModel::getCommandedHeadingD() const
{
   return cmdHeading;
}

// setHeadingHoldOn() --   Enable/Disable heading hold
bool RacModel::setHeadingHoldOn(const bool)
{
   return true;
}

// setCommandedHeadingD() --   Sets commanded heading (true: degs)
bool RacModel::setCommandedHeadingD(const double degs, const double, const double)
{
   cmdHeading = degs;
   return true;
}


bool RacModel::isVelocityHoldOn() const
{
   return true;
}

double RacModel::getCommandedVelocityKts() const
{
   return cmdVelocity;
}
                                                                       
// setVelocityHoldOn() --   Enable/Disable velocity hold
bool RacModel::setVelocityHoldOn(const bool)
{
   return true;
}

// setCommandedVelocityKts() --   Sets commanded velocity (kts)
bool RacModel::setCommandedVelocityKts(const double v, const double vNps)
{
   cmdVelocity = v;
   return true;
}

bool RacModel::isAltitudeHoldOn() const
{
   return true;
}

double RacModel::getCommandedAltitude() const
{
   return cmdAltitude;
}
                                                                             
// setAltitudeHoldOn() --   Enable/Disable altitude hold
bool RacModel::setAltitudeHoldOn(const bool)
{
   return true;
}

// setCommandedAltitude() --   Sets commanded altitude (meters)
bool RacModel::setCommandedAltitude(const double m, const double, const double)
{
   cmdAltitude = m;
   return false;
}

//------------------------------------------------------------------------------
// updateRAC -- update Robot Aircraft
//------------------------------------------------------------------------------
void RacModel::updateRAC(const LCreal dt)
{
   // Get our Player (must have one!)
   Simulation::Player* pp = static_cast<Simulation::Player*>( findContainerByType(typeid(Simulation::Player)) );
   if (pp == 0) return;

   // Acceleration of Gravity (M/S)
   LCreal g = ETHG * Basic::Distance::FT2M;

   // Set default commanded values
   if (cmdAltitude < -9000.0)
       cmdAltitude = static_cast<LCreal>(pp->getAltitudeM());
   if (cmdHeading < -9000.0)
       cmdHeading = static_cast<LCreal>(pp->getHeadingD());
   if (cmdVelocity < -9000.0)
       cmdVelocity = pp->getTotalVelocityKts();
  
   // ---
   // Compute delta altitude; commanded vertical velocity and
   // commanded flight path angle
   // ---

   // Max altitude rate 6000 ft /min converted to M/S
   double maxAltRate = (3000.0 / 60.0) * Basic::Distance::FT2M;

   // commanded vertical velocity is delta altitude limited to max rate
   double cmdAltRate = (cmdAltitude - pp->getAltitudeM());
   if (cmdAltRate > maxAltRate) cmdAltRate = maxAltRate;
   else if (cmdAltRate < -maxAltRate) cmdAltRate = -maxAltRate;

   // Compute commanded flight path angle (gamma)
   double cmdPitch = 0;
   LCreal vt = pp->getTotalVelocity();
   if (vt > 0) {
      cmdPitch = asin( cmdAltRate/vt );
   }


   // ---
   // Compute Max G
   // ---
   LCreal gmax = gMax;                     // Max g,s
   if(pp->getTotalVelocity() < vpMaxG) {
      gmax = 1.0f + (gMax - 1.0f) * (pp->getTotalVelocity() - vpMin) / (vpMaxG - vpMin);
   }

   // ---
   // Computer max turn rate, max/min pitch rates
   // ---
   LCreal ra_max = gmax * g / pp->getTotalVelocity();    // Turn rate base on vp and g,s (rad/sec) 
   LCreal qa_max = ra_max;                           // Max pull up pitch rate (rad/sec) 
   LCreal qa_min = -qa_max;                          // Max pushover pitch rate (rad/sec)
   if(gmax > 2.0) {
      // Max yaw rate (rad/sec)
      qa_min = -( 2.0f * g / pp->getTotalVelocity());
   }

   // ---
   // Get old angular values 
   // ---
   const osg::Vec3 oldRates = pp->getAngularVelocities();
   //LCreal pa1 = oldRates[Simulation::Player::IROLL];
   LCreal qa1 = oldRates[Simulation::Player::IPITCH];
   LCreal ra1 = oldRates[Simulation::Player::IYAW];

   // ---
   // Find pitch rate and update pitch
   // ---
   double qa = Basic::Angle::aepcdRad(cmdPitch - pp->getPitchR()) * 0.1;
   if(qa > qa_max) qa = qa_max;
   if(qa < qa_min) qa = qa_min;

   // Find turn rate
   double ra = Basic::Angle::aepcdRad((cmdHeading  * Basic::Angle::D2RCC) - pp->getHeadingR()) * 0.1;
   if(ra > ra_max) ra = ra_max;
   if(ra < -ra_max) ra = -ra_max;

   // Damage 
   double dd = pp->getDamage();
   if (dd > 0.5) {
      ra += (dd - 0.5) * ra_max;
      qa -= (dd - 0.5) * qa_max;
   }

   // Using Pitch rate, integrate pitch
   double newTheta = static_cast<LCreal>(pp->getPitch() + (qa + qa1) * dt / 2.0);

   // Use turn rate integrate heading
   double newPsi = static_cast<LCreal>(pp->getHeading() + (ra + ra1) * dt / 2.0);
   if(newPsi > 2.0*PI) newPsi -= 2.0*PI;
   if(newPsi < 0.0) newPsi += 2.0*PI;

   // Roll angle is proportional to max turn rate - filtered
   double pa = 0.0;
   double newPhi = 0.98 * pp->getRollR() + 0.02 * (ra / ra_max * (Basic::Angle::D2RCC * 60.0));

   // Find Acceleration
   double cmdVelMPS = cmdVelocity * (Basic::Distance::NM2M / 3600.0);
   double vpdot = (cmdVelMPS - pp->getTotalVelocity()) * 0.05;
   if(vpdot > maxAccel)  vpdot = maxAccel;
   if(vpdot < -maxAccel) vpdot = -maxAccel;

   // Compute new velocity (body coordinates)
   double newVP = pp->getTotalVelocity() + vpdot * dt;

   // Set our angular values
   pp->setEulerAngles(newPhi, newTheta, newPsi);
   pp->setAngularVelocities(pa, qa, ra);

   // Set our velocity vector (body coordinates)
   pp->setVelocityBody( LCreal(newVP), 0.0, 0.0);

   // Set our acceleration vector (body coordinates)
   pp->setAccelerationBody( LCreal(vpdot), 0.0, 0.0);
}


//------------------------------------------------------------------------------
// slot methods
//------------------------------------------------------------------------------

bool RacModel::setSlotMinSpeed(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       vpMin = msg->getReal();
       ok = true;
    }
    return ok;
}
bool RacModel::setSlotSpeedMaxG(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       vpMaxG = msg->getReal();
       ok = true;
    }
    return ok;
}
bool RacModel::setSlotMaxG(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       gMax = msg->getReal();
       ok = true;
    }
    return ok;
}
bool RacModel::setSlotMaxAccel(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       maxAccel = msg->getReal();
       ok = true;
    }
    return ok;
}
bool RacModel::setSlotCmdAltitude(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
       double value = Basic::Meters::convertStatic( *msg );
       cmdAltitude = value;
       ok = true;
    }
    return ok;
}
bool RacModel::setSlotCmdHeading(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
       double value  = Basic::Degrees::convertStatic( *msg );
       cmdHeading = value;
       ok = true;
    }
    return ok;
}
bool RacModel::setSlotCmdVelocity(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
       double value = msg->getReal();
       cmdVelocity = value;
       ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// getSlotByIndex() for Graphic
//------------------------------------------------------------------------------
Basic::Object* RacModel::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& RacModel::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    indent(sout,i+j);
    sout << "minSpeed: " << vpMin << std::endl;
    
    indent(sout,i+j);
    sout << "speedMaxG: " << vpMaxG << std::endl;

    indent(sout,i+j);
    sout << "maxg: " << gMax << std::endl;

    indent(sout,i+j);
    sout << "maxAccel: " << maxAccel << std::endl;

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Vehicle namespace
} // End Eaagles namespace
