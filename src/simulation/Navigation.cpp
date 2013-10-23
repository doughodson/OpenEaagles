//------------------------------------------------------------------------------
// Class: Navigation
//------------------------------------------------------------------------------

#include "openeaagles/simulation/Navigation.h"

#include "openeaagles/simulation/Bullseye.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Route.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Steerpoint.h"
#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// class: Navigation
//==============================================================================

IMPLEMENT_SUBCLASS(Navigation,"Navigation")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Navigation)
    "route",            // 1) Primary route
    "utc",              // 2) initial UTC time
    "feba",             // 3) FEBA: List of distance vectors [ North East ];
                        //    default unit is Nautical Miles; Example:
                        //    feba: {
                        //      [ 10 -10 ]  // in nautical miles
                        //      [ ( Kilometers 20 ) ( NauticalMiles -15 ) ]
                        //    }
    "bullseye",         // 4) Bullseye (just one for now)
END_SLOTTABLE(Navigation)

// Map slot table to handles
BEGIN_SLOT_MAP(Navigation)
    ON_SLOT(1,setSlotRoute,Route)
    ON_SLOT(2,setSlotUtc,Basic::Time)
    ON_SLOT(3,setSlotFeba,Basic::PairStream)
    ON_SLOT(4,setSlotBullseye,Bullseye)
END_SLOT_MAP()


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Navigation::Navigation() : rm()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Navigation::initData()
{
   priRoute = 0;
   initRoute = 0;
   bull = 0;

   latitude = 0;
   longitude = 0;
   altitude = 0;
   posValid = false;
   heading = 0;
   pitch = 0;
   roll = 0;
   attValid = false;
   velVec.set(0,0,0);
   accelVec.set(0,0,0);
   gs = 0;
   tas = 0;
   tk = 0;
   velValid = false;
   magvar = 0;
   mhdg = 0;   
   magVarValid = 0;
   windDirD = 0;   
   windSpdKts = 0;   
   windsValid = false;
   navStrValid = false;
   tbrg = 0;
   mbrg = 0;
   dst = 0;
   ttg = 0;
   tcrs = 0;
   mcrs = 0;
   xte = 0;
   eta = 0; 

   utc = 0;
   initUTC = 0;
   utcValid = false;

   // No FEBA
   feba = 0;
   nFeba = 0;

   refLat = 0;
   refLon = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Navigation::copyData(const Navigation& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   if (org.priRoute != 0) {
      Route* p = org.priRoute->clone();
      priRoute = p;
      p->container(this);
      p->unref();  // SPtr<> has it
   }
   else priRoute = 0;

   if (org.initRoute != 0) {
      Route* p = org.initRoute->clone();
      initRoute = p;
      p->unref();  // SPtr<> has it
   }
   else initRoute = 0;

   if (org.bull != 0) {
      Bullseye* b = org.bull->clone();
      bull = b;
      b->container(this);
      b->unref();  // SPtr<> has it
   }
   else bull = 0;

   latitude = org.latitude;
   longitude = org.longitude;
   altitude = org.altitude;
   posValid = org.posValid;
   heading = org.heading;
   pitch = org.pitch;
   roll = org.roll;
   attValid = org.attValid;
   velVec = org.velVec;
   accelVec = org.accelVec;
   rm = org.rm;
   gs = org.gs;
   tas = org.tas;
   tk = org.tk;
   velValid = org.velValid;
   magvar = org.magvar;
   mhdg = org.mhdg;   
   magVarValid = org.magVarValid;
   windDirD = org.windDirD;
   windSpdKts = org.windSpdKts;  
   windsValid = org.windsValid;
   navStrValid = org.navStrValid;
   tbrg = org.tbrg;
   mbrg = org.mbrg;
   dst = org.dst;
   ttg = org.ttg;
   tcrs = org.tcrs;
   mcrs = org.mcrs;
   xte = org.xte;
   eta = org.eta; 
   utc = org.utc;
   initUTC = org.initUTC;
   utcValid = org.utcValid;
   refLat = org.refLat;
   refLon = org.refLon;

   // Copy Feba
   setFeba(org.feba, org.nFeba);
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Navigation::deleteData()
{
    priRoute = 0;
    initRoute = 0;
    bull = 0;

    // Delete the FEBA
    setFeba(0, 0);
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Navigation::reset()
{
   BaseClass::reset();

   // And not nothin' valid
   posValid = false;
   attValid = false;
   velValid = false;
   magVarValid = false;
   navStrValid = false;

   // Except UTC
   utc = initUTC;
   utcValid = true;

   // Reset the route to the initial route and reset
   if (priRoute != 0) {
      priRoute->container(0);
      priRoute = 0;
   }
   if (initRoute != 0) {
      priRoute = initRoute->clone();
      priRoute->unref();  // SPtr<> has it
   }
   if (priRoute != 0) {
      priRoute->container(this);
      priRoute->event(RESET_EVENT);
   }

   // Reset our bullseye
   if (bull != 0) bull->event(RESET_EVENT);

   // Set the reference center of our gaming area
   const Simulation* sim = getSimulation();
   if (sim != 0) {
      refLat = sim->getRefLatitude();
      refLon = sim->getRefLongitude();     
   }
}

//------------------------------------------------------------------------------
// updateData() -- update Non-time critical stuff here
//------------------------------------------------------------------------------
void Navigation::updateData(const LCreal dt)
{
   // ---
   // Update the BaseClass and our primary route
   // ---
   if (priRoute != 0) priRoute->updateData(dt);
}

//------------------------------------------------------------------------------
// Process phase
//------------------------------------------------------------------------------
void Navigation::process(const LCreal dt)
{
   BaseClass::process(dt);

   // ---
   // Update our position, attitude and velocities
   // ---
   if (getOwnship() != 0) {
      velValid = updateSysVelocity();
      posValid = updateSysPosition();
      attValid = updateSysAttitude();
      magVarValid = updateMagVar();
   }
   else {
      posValid = false;
      attValid = false;
      velValid = false;
      magVarValid = false;
   }

   // Update UTC
   double v = utc + dt;
   if (v >= Basic::Time::D2S) v = (v - Basic::Time::D2S);
   setUTC(v);

   // ---
   // Update our primary route
   // ---
   if (priRoute != 0) priRoute->tcFrame(dt);

   // Update our bullseye
   if (bull != 0) bull->compute(this);

   // ---
   // Update our navigational steering data
   // ---
   updateNavSteering();
}


//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Is system position valid?
bool Navigation::isPositionDataValid() const
{
   return posValid;
}

// Returns system latitude (degs)
double Navigation::getLatitude() const
{
   return latitude;
}

// Returns system longitude (degs)
double Navigation::getLongitude() const
{
   return longitude;
}

// Returns system altitude (ft)
double Navigation::getAltitudeFt() const
{
    return getAltitudeM() * Basic::Distance::M2FT;
}

// Returns system altitude (m)
double Navigation::getAltitudeM() const
{
   return altitude;
}

// Is system attitude valid?
bool Navigation::isAttitudeDataValid() const
{
   return attValid;
}

// Returns system pitch (degs)
double Navigation::getPitchDeg() const
{
   return pitch;
}

// Returns system roll  (degs)
double Navigation::getRollDeg() const
{
   return roll;
}

// Returns system true heading (degs)
double Navigation::getHeadingDeg() const
{
   return heading;
}

// Returns directional cosines
const osg::Matrixd& Navigation::getRotMat() const
{
   return rm;
}

// Is the magnetic variation valid?
bool Navigation::isMagVarValid() const
{
   return magVarValid;
}

// Returns the magnetic variation
double Navigation::getMagVarDeg() const
{
   return magvar;
}

// Returns the mag heading
double Navigation::getMagHeadingD() const
{
   return Basic::Angle::aepcdDeg(getHeadingDeg() + getMagVarDeg());
}

// Are the winds valid?
bool Navigation::areWindsValid() const
{
   return windsValid;
}

// Returns the wind 'from' direction (degs)
LCreal Navigation::getWindDirD() const
{
   return windDirD;
}

// Returns the wind speed (kts)
LCreal Navigation::getWindSpeedKts() const
{
   return windSpdKts;
}

// Is the UTC time valid?
bool Navigation::isUtcDataValid() const
{
   return utcValid;
}

// Returns UTC time (sec)
double Navigation::getUTC() const
{
   return utc;
}

// Is system velociy valid?
bool Navigation::isVelocityDataValid() const
{
   return velValid;
}

// Returns ground speed (kts)
LCreal Navigation::getGroundSpeedKts() const
{
   return gs;
}

// Returns true airspeed (kts)
LCreal Navigation::getTrueAirspeedKts() const
{
   return tas;
}

// Returns true ground track (degs)
LCreal Navigation::getGroundTrackDeg() const
{
   return tk;
}

// Returns velocity vector (m/s)
const osg::Vec3d& Navigation::getVelocity() const
{
   return velVec;
}

// Returns acceleration vector (m/s/s)
const osg::Vec3d& Navigation::getAcceleration() const
{
   return accelVec;
}

// Is system steering data valid?
bool Navigation::isNavSteeringValid() const
{
   return navStrValid;
}

// Returns true bearing to dest (degs)
LCreal Navigation::getTrueBrgDeg() const
{
   return tbrg;
}

// Returns mag bearing to dest (degs)
LCreal Navigation::getMagBrgDeg() const
{
   return mbrg;
}

// Returns distance to dest (NM)
LCreal Navigation::getDistNM() const
{
   return dst;
}

// Returns true course to dest (degs)
LCreal Navigation::getTrueCrsDeg() const
{
   return tcrs;
}

// Returns mag course to dest (degs)
LCreal Navigation::getMagCrsDeg() const
{
   return mcrs;
}

// Returns cross track error (NM)
LCreal Navigation::getCrossTrackErrorNM() const
{
   return xte;
}

// Returns Time-To-Go to dest (sec)
LCreal Navigation::getTTG() const
{
   return ttg;
}

// Returns Est-Time-of-Arrival at dest (sec)
LCreal Navigation::getETA() const
{
   return eta;
}

// Returns the ref latitude (degs)
double Navigation::getRefLatitude() const
{
   return refLat;
}

// Returns the ref longitude (degs)
double Navigation::getRefLongitude() const
{
   return refLon;
}

// Returns the primary route (non-const version)
Route* Navigation::getPriRoute()
{
   return priRoute;
}

// Returns the primary route (const version)
const Route* Navigation::getPriRoute() const
{
   return priRoute;
}

// Returns the current bullseye (non-const version)
Bullseye* Navigation::getBullseye()
{
   return bull;
}

// Returns the current bullseye (const version)
const Bullseye* Navigation::getBullseye() const
{
   return bull;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool Navigation::setPosition(const double lat, const double lon, const double alt)
{
   latitude  =  lat;
   longitude =  lon;
   altitude  =  alt;
   posValid  =  true;
   return true;
}

bool Navigation::setPosition(const bool flg)
{
   posValid  =  flg;
   return true;
}

bool Navigation::setAttitude(const double roll0, const double pitch0, const double thdg0)
{
   roll = roll0;
   pitch = pitch0;
   heading = thdg0;
   Basic::Nav::computeRotationalMatrixDeg(roll0, pitch0, thdg0, &rm);

   if (magVarValid) mhdg = Basic::Angle::aepcdDeg(heading - magvar);
   else mhdg = heading;

   attValid = true;

   return true;
}

bool Navigation::setAttitude(const bool flg)
{
   attValid = flg;
   return true;
}

bool Navigation::setMagVar(const double mvDeg)
{
   magvar = mvDeg;
   if (attValid) mhdg = Basic::Angle::aepcdDeg(heading - magvar);
   magVarValid = true;
   return true;
}

bool Navigation::setMagVar(const bool flg)
{
   magVarValid = flg;
   return true;
}

bool Navigation::setWinds(const LCreal dirDeg, const LCreal speedKts)
{
   windDirD = dirDeg;
   windSpdKts = speedKts;
   windsValid = true;
   return true;
}

bool Navigation::setWinds(const bool flg)
{
   windsValid = flg;
   return true;
}

bool Navigation::setUTC(const double time)
{
   utc = time;
   utcValid = true;
   return true;
}

bool Navigation::setUTC(const bool flg)
{
   utcValid = flg;
   return true;
}

bool Navigation::setGroundSpeedKts(const LCreal kts)
{
   gs = kts;
   return true;
}

bool Navigation::setTrueAirspeedKts(const LCreal kts)
{
   tas = kts;
   return true;
}

bool Navigation::setGroundTrackDeg(const LCreal degs)
{
   tk = degs;
   return true;
}

bool Navigation::setVelocity(const osg::Vec3d& v)
{
   velVec = v;
   return true;
}

bool Navigation::setAcceleration(const osg::Vec3d& v)
{
   accelVec = v;
   return true;
}

bool Navigation::setVelocityDataValid(const bool flg)
{
   velValid = flg;
   return true;
}

bool Navigation::setTrueBrgDeg(const LCreal v)
{
   tbrg = v;
   return true;
}

bool Navigation::setMagBrgDeg(const LCreal v)
{
   mbrg = v;
   return true;
}

bool Navigation::setDistNM(const LCreal v)
{
   dst = v;
   return true;
}

bool Navigation::setTrueCrsDeg(const LCreal v)
{
   tcrs = v;
   return true;
}

bool Navigation::setMagCrsDeg(const LCreal v)
{
   mcrs = v;
   return true;
}

bool Navigation::setCrossTrackErrorNM(const LCreal v)
{
   xte = v;
   return true;
}

bool Navigation::setTTG(const LCreal v)
{
   ttg = v;
   return true;
}

bool Navigation::setETA(const LCreal v)
{
   eta = v;
   return true;
}

bool Navigation::setNavSteeringValid(const bool flg)
{
   navStrValid = flg;
   return true;
}


//------------------------------------------------------------------------------
// Default navigation methods
//------------------------------------------------------------------------------

// (default) System position (using truth data from ownship)
bool Navigation::updateSysPosition()
{
    bool ok = false;
    if (getOwnship() != 0) {

       // -- convert ownship's position vector to lat/lon/alt
       double lat0 = 0;
       double lon0 = 0;
       double alt0 = 0;
       ok = Basic::Nav::convertPosVec2LL(refLat, refLon, getOwnship()->getPosition(), &lat0, &lon0, &alt0);

       setPosition(lat0, lon0, alt0);
    }
    return ok;
}

// (default) System attitude function  (using truth data from ownship)
bool Navigation::updateSysAttitude()
{
    bool ok = false;
    if (getOwnship() != 0) {
       setAttitude(
            getOwnship()->getRollD(),
            getOwnship()->getPitchD(),
            getOwnship()->getHeadingD()
       );
       ok = true;
    }
    return ok;
}

// Mag var data
bool Navigation::updateMagVar()
{
   // No default magnetic variation
   setMagVar(false);
   return false;
}

// (default) Velocity data function (using truth data from ownship)
bool Navigation::updateSysVelocity()
{
   bool ok = false;
   if (getOwnship() != 0) {
      setVelocity( getOwnship()->getVelocity() );
      setAcceleration( getOwnship()->getAcceleration() );
      setGroundSpeedKts( getOwnship()->getGroundSpeedKts() );
      setTrueAirspeedKts( getOwnship()->getTotalVelocityKts() );
      setGroundTrackDeg( getOwnship()->getGroundTrackD() );
      setVelocityDataValid( true );
      ok = true;
   }
   else {
      setVelocityDataValid( false );
   }
   return ok;
}

// (default) Nav steering function (pull data from the 'to' steerpoint)
bool Navigation::updateNavSteering()
{
   if (getPriRoute() != 0) {
      const Steerpoint* to = getPriRoute()->getSteerpoint();
      if (to != 0) {
         if (to->isNavDataValid()) {
            setTrueBrgDeg( to->getTrueBrgDeg() );
            setMagBrgDeg( to->getMagBrgDeg() );
            setDistNM( to->getDistNM()) ;
            setTrueCrsDeg( to->getTrueCrsDeg() );
            setMagCrsDeg( to->getMagCrsDeg() );
            setTTG( to->getTTG() );
            setETA( to->getETA() );
            setCrossTrackErrorNM( to->getCrossTrackErrNM() );
            setNavSteeringValid( true );
         }
         else {
            setNavSteeringValid( false );
         }

      }
   }
   return isNavSteeringValid();
}

//------------------------------------------------------------------------------
// getFeba() -- FEBA [ North East ] (Nautical Miles)
//------------------------------------------------------------------------------
int Navigation::getFeba(osg::Vec2* const points, const int max) const
{
    int n = 0;
    if (points != 0 && max > 0 && feba != 0 && nFeba > 0) {

        // Number of points; limited by 'max'
        n = nFeba;
        if (n > max) n = max;

        // Copy the FEBA points
        for (int i = 0; i < n; i++) {
            points[i] = feba[i];
        }
    }
    return n;
}

//------------------------------------------------------------------------------
// getFeba() -- FEBA [ North East ] (Nautical Miles)
//------------------------------------------------------------------------------
bool Navigation::setFeba(osg::Vec2* const points, const int n)
{
    // First delete any old FEBA lines
    if (feba != 0) delete[] feba;
    feba = 0;
    nFeba = 0;

    if (points != 0 && n >= 2) { // Need at least two points
        nFeba = n;
        feba = new osg::Vec2[nFeba];
        for (int i = 0; i < nFeba; i++) {
            feba[i] = points[i];
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// setRoute() - sets a new route on the fly
//------------------------------------------------------------------------------
bool Navigation::setRoute(Route* const msg)
{
   // we are a new route, but our last new route wasn't the original route
   if (priRoute != 0) priRoute->container(0);
   priRoute = msg;
   if (priRoute != 0) priRoute->container(this);
   return true;
}


//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool Navigation::setSlotRoute(const Route* const msg)
{
   initRoute = msg;

   if (priRoute != 0) {
      priRoute->container(0);
      priRoute = 0;
   }
   if (initRoute != 0) {
      priRoute = initRoute->clone();
      priRoute->unref();  // SPtr<> has it
   }
   if (priRoute != 0) {
      priRoute->container(this);
   }
   return true;
}

bool Navigation::setSlotUtc(const Basic::Time* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initUTC = Basic::Seconds::convertStatic( *msg );
        ok = true;
    }
    return ok;
}

// setSlotFeba() --- Sets the FEBA points
bool Navigation::setSlotFeba(const Basic::PairStream* const msg)
{
    bool ok = true;

    if (msg != 0) {
        // allocate space for the points
        int max = msg->entries();
        osg::Vec2* tmpFeba = new osg::Vec2[max];

        // Get the points from the pair stream
        int np = 0;
        const Basic::List::Item* item = msg->getFirstItem();
        while (item != 0 && np < max && ok) {
            bool validFlg = false;
            const Basic::Pair* p = dynamic_cast<const Basic::Pair*>(item->getValue());
            if (p != 0) {
                const Basic::Object* obj2 = p->object();
                const Basic::List* msg2 = dynamic_cast<const Basic::List*>(obj2);
                if (msg2 != 0) {
                    LCreal values[2];
                    int n = 0;

                    { // Get the north (first) distance
                        const Basic::Number* pNum = 0;
                        const Basic::Pair* pair2 = dynamic_cast<const Basic::Pair*>(msg2->getPosition(1));
                        if (pair2 != 0) pNum = dynamic_cast<const Basic::Number*>(pair2->object());
                        else pNum = dynamic_cast<const Basic::Number*>(msg2->getPosition(1));

                        if (pNum != 0) {
                            const Basic::Distance* pDist = dynamic_cast<const Basic::Distance*>(pNum);
                            if (pDist != 0) {
                                values[n++] = Basic::NauticalMiles::convertStatic(*pDist);
                            }
                            else {
                                values[n++] = pNum->getReal();
                            }
                        }
                    }

                    { // Get the east (second) distance
                        const Basic::Number* pNum = 0;
                        const Basic::Pair* pair2 = dynamic_cast<const Basic::Pair*>(msg2->getPosition(2));
                        if (pair2 != 0) pNum = dynamic_cast<const Basic::Number*>(pair2->object());
                        else pNum = dynamic_cast<const Basic::Number*>(msg2->getPosition(2));

                        if (pNum != 0) {
                            const Basic::Distance* pDist = dynamic_cast<const Basic::Distance*>(pNum);
                            if (pDist != 0) {
                                values[n++] = Basic::NauticalMiles::convertStatic(*pDist);
                            }
                            else {
                                values[n++] = pNum->getReal();
                            }
                        }
                    }

                    if (n == 2) {
                        tmpFeba[np++].set(values[0],values[1]);
                        validFlg = true;
                    }
                }
            }

            if (!validFlg) {
                std::cerr << "Navigation::setSlotFeba: Invalid FEBA coordinate!" << std::endl;
                ok = false;
            }
            item = item->getNext();
        }

        // If we have a valid FEBA, then save it
        if (ok) {
            setFeba(tmpFeba, np);
        }
    }

    return ok;
}
bool Navigation::setSlotBullseye(Bullseye* const msg)
{
   if (bull != 0) {
      bull->container(0);
   }
   bull = msg;
   if (bull != 0) {
      bull->container(this);
   }
   return true;
}

// serializer
std::ostream& Navigation::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    // primary route
    if (priRoute != 0) {
        indent(sout,i+j);
        sout << "route: " << std::endl;
        priRoute->serialize(sout,(i+j),slotsOnly);
    }

    //BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Navigation::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace
