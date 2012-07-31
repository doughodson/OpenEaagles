// Steerpoint

#include "openeaagles/simulation/Steerpoint.h"

#include "openeaagles/simulation/Actions.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Navigation.h"
#include "openeaagles/simulation/Simulation.h"

#include "openeaagles/basic/Identifier.h"
#include "openeaagles/basic/LatLon.h"
#include "openeaagles/basic/List.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/Terrain.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Times.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(Steerpoint,"Steerpoint")

//------------------------------------------------------------------------------
// Slot table
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Steerpoint)
    "stptType",         //  1) Steerpoint type          (Identifier) { ROUTE, DEST, MARK, FIX, OAP, TGT }; default: DEST
    "latitude",         //  2) Latitude                 (latLon)
    "longitude",        //  3) Longitude                (latLon)
    "xPos",             //  4) X position               (Distance)
    "yPos",             //  5) Y position               (Distance)
    "elevation",        //  6) Terrain Elevation        (Distance)
    "altitude",         //  7) Commanded altitude       (Distance)
    "airspeed",         //  8) Commanded airspeed (Kts) (Number)
    "pta",              //  9) Planned Time of Arrival  (Time)
    "sca",              // 10) Safe Clearance Altitude  (Distance)
    "description",      // 11) Description              (String)
    "magvar",           // 12) Magnetic Var @ point     (Angle)
    "next",             // 13) Next steerpoint number: by name (Identifier) or index (Number)
    "action",           // 14) Steerpoint action        (Action)
END_SLOTTABLE(Steerpoint)

// Map slot table to handles 
BEGIN_SLOT_MAP(Steerpoint)
    ON_SLOT( 1, setSlotSteerpointType, Basic::Identifier)

    ON_SLOT( 2, setSlotLatitude,       Basic::LatLon)
    ON_SLOT( 2, setSlotLatitude,       Basic::Number)

    ON_SLOT( 3, setSlotLongitude,      Basic::LatLon)
    ON_SLOT( 3, setSlotLongitude,      Basic::Number)

    ON_SLOT( 4, setSlotXPos,           Basic::Distance)

    ON_SLOT( 5, setSlotYPos,           Basic::Distance)

    ON_SLOT( 6, setSlotElevation,      Basic::Distance)
    ON_SLOT( 6, setSlotElevation,      Basic::Number)

    ON_SLOT( 7, setSlotCmdAltitude,    Basic::Distance)
    ON_SLOT( 7, setSlotCmdAltitude,    Basic::Number)

    ON_SLOT( 8, setSlotCmdAirspeed,    Basic::Number)

    ON_SLOT( 9, setSlotPTA,            Basic::Time)
    ON_SLOT( 9, setSlotPTA,            Basic::Number)

    ON_SLOT(10, setSlotSCA,            Basic::Distance)
    ON_SLOT(10, setSlotSCA,            Basic::Number)

    ON_SLOT(11, setSlotDescription,    Basic::String)

    ON_SLOT(12, setSlotMagVar,         Basic::Angle)
    ON_SLOT(12, setSlotMagVar,         Basic::Number)

    ON_SLOT(13, setSlotNext,           Basic::Identifier)
    ON_SLOT(13, setSlotNext,           Basic::Number)

    ON_SLOT(14, setAction,             Action)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Steerpoint::Steerpoint()
{
    STANDARD_CONSTRUCTOR()
    
    description = 0;
    action = 0;
    
    latitude = 0; 
    longitude = 0;
    elevation = 0;
    posVec.set(0,0,0);
    stptType = DEST;  
    pta = 0;  
    sca = 0; 
    magvar = 0; 
    needPosVec = true;
    needLL = true; 
    cmdAlt = 0;
    haveCmdAlt = false;
    cmdAirspeed = 0;
    haveCmdAs = false;  
    next = 0;

    initLatitude = 0;
    initLongitude = 0;
    initElev = 0;
    initPosVec.set(0,0,0);
    initMagVar = 0;
    haveInitLat = false;
    haveInitLon = false;
    haveInitPos = false;
    haveInitMagVar = false;
    haveInitElev = false;
    initCmdAlt = 0;
    haveInitCmdAlt = false;
    initCmdAirspeed = 0;
    haveInitCmdAs = false; 
    initNextStptName = 0;
    initNextStptIdx = 0;

    tbrg = 0;
    mbrg = 0;
    dst = 0;
    ttg = 0;
    xte = 0;

    tcrs = 0;
    mcrs = 0;
    tlt = 0;
    tld = 0;

    tde = 0;
    ete = 0;
    eta = 0;
    elt = 0;
    scaWarn = false;
    navDataValid = false;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Steerpoint::copyData(const Steerpoint& org, const bool cc)
{
    BaseClass::copyData(org);
    
    if (cc) {
        next = 0;
        initNextStptName = 0;
        description = 0;
        action = 0;
    }

    next = 0; // find it using 'initNextStptName' or 'initNexStptIdx'
    
    Basic::String* n = 0;
    if (org.initNextStptName != 0) n = (Basic::String*) org.initNextStptName->clone();
    initNextStptName = n;

    initNextStptIdx = org.initNextStptIdx;

    Basic::String* s = 0;
    if (org.description != 0) s = (Basic::String*) org.description->clone();
    description = s;

    Action* aa = 0;
    if (org.action != 0) aa = (Action*) org.action->clone();
    action = aa;
    
    latitude = org.latitude; 
    longitude = org.longitude;
    elevation = org.elevation;
    posVec = org.posVec;
    stptType = org.stptType;  
    pta = org.pta;  
    sca = org.sca; 
    magvar = org.magvar; 
    needPosVec = org.needPosVec;
    needLL = org.needLL; 
    cmdAlt = org.cmdAlt;
    haveCmdAlt = org.haveCmdAlt;
    cmdAirspeed = org.cmdAirspeed;
    haveCmdAs = org.haveCmdAs;

    initLatitude = org.initLatitude;
    initLongitude = org.initLongitude;
    initElev = org.initElev;
    initPosVec = org.initPosVec;
    initMagVar = org.initMagVar;
    haveInitLat = org.haveInitLat;
    haveInitLon = org.haveInitLon;
    haveInitPos = org.haveInitPos;
    haveInitMagVar = org.haveInitMagVar;
    haveInitElev = org.haveInitElev;
    initCmdAlt = org.initCmdAlt;
    haveInitCmdAlt = org.haveInitCmdAlt;
    initCmdAirspeed = org.initCmdAirspeed;
    haveInitCmdAs = org.haveInitCmdAs;

    tbrg = org.tbrg;
    mbrg = org.mbrg;
    dst = org.dst;
    ttg = org.ttg;
    xte = org.xte;

    tcrs = org.tcrs;
    mcrs = org.mcrs;
    tlt = org.tlt;
    tld = org.tld;

    tde = org.tde;
    ete = org.ete;
    eta = org.eta;
    elt = org.elt;
    scaWarn = org.scaWarn;
    navDataValid = org.navDataValid;
}


//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Steerpoint::deleteData()
{
    next = 0;
    initNextStptName = 0;
    description = 0;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Steerpoint::reset()
{
    if (haveInitElev) {
        elevation  = initElev;
        initPosVec[Player::IDOWN] = -initElev;
        posVec[Player::IDOWN] = -initElev;
    }
    
    // ---
    // Set initial positions -- give priority to lat/lon entries
    // ---
    if (haveInitLat && haveInitLon) {
        setLatitude(initLatitude);
        setLongitude(initLongitude);
        setElevation(initElev);
    }
    else if (haveInitPos) {
        setPosition(initPosVec);
    }

    if (haveInitCmdAlt) {
       setCmdAltitude( initCmdAlt );
    }

    if (haveInitCmdAs) {
       setCmdAirspeedKts( initCmdAirspeed );
    }

    BaseClass::reset();
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------

LCreal Steerpoint::getElevationFt() const
{
    return getElevationM() * Basic::Distance::M2FT;
}

const char* Steerpoint::getDescription() const
{
    const char* p = 0;
    if (description != 0) p = *description;
    return p;
}

double Steerpoint::getLatitude() const
{
    double ll = 0;
    // Make sure we have a valid lat/lon
    if (!needLL) ll = latitude;
    return ll;
}

double Steerpoint::getLongitude() const
{
    double ll = 0;
    // Make sure we have a valid lat/lon
    if (!needLL) ll = longitude;
    return ll;
}

LCreal Steerpoint::getCmdAltitudeFt() const
{
    return getCmdAltitude() * Basic::Distance::M2FT;
}

//------------------------------------------------------------------------------
// Set the ground elevation at the steerpoint from this terrain database
// Returns true if successful.
//------------------------------------------------------------------------------
bool Steerpoint::setElevation(const Basic::Terrain* const terrain, const bool interp)
{
   bool ok = false;
   if (!needLL) {
      LCreal elev = 0;
      ok = terrain->getElevation(&elev, latitude, longitude, interp);
      if (ok) setElevation(elev);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Data set functions
//------------------------------------------------------------------------------
void Steerpoint::setPosition(const LCreal x, const LCreal y, const LCreal z)
{
    posVec.set(x, y, z);
    needPosVec = false;
    needLL = true;
}

void Steerpoint::setPosition(const osg::Vec3& newPos)
{
    posVec = newPos;
    needPosVec = false;
    needLL = true;
}

void Steerpoint::setLatitude(const double v)
{
    latitude = v;
    needPosVec = true;
    needLL = false;
}

void Steerpoint::setLongitude(const double v)
{
    longitude = v;
    needPosVec = true;
    needLL = false;
}

void Steerpoint::setElevation(const LCreal x)
{
    elevation = x;
    posVec[Player::IDOWN] = -x;
}

void Steerpoint::setDescription(const Basic::String* const d)
{
    description = d;
}

void Steerpoint::setCmdAltitude(const LCreal x)
{
   cmdAlt = x;
   haveCmdAlt = true;
}

void Steerpoint::setCmdAirspeedKts(const LCreal x)
{
   cmdAirspeed = x;
   haveCmdAs = true;
}

// Sets the action to be performed
bool Steerpoint::setAction(Action* const aa)
{
   action = aa;
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool Steerpoint::setSlotSteerpointType(const Basic::Identifier* const msg)
{
    bool ok = false;
    if (msg != 0) {
        ok = true;  // assume that it's valid
        if      (*msg == "DEST"  || *msg == "dest")  setSteerpointType(DEST);
        else if (*msg == "MARK"  || *msg == "mark")  setSteerpointType(MARK);
        else if (*msg == "FIX"   || *msg == "fix")   setSteerpointType(FIX);
        else if (*msg == "OAP"   || *msg == "oap")   setSteerpointType(OAP);
        else if (*msg == "TGT"   || *msg == "tgt")   setSteerpointType(TGT);
        else {
            std::cerr << "xxx(): invalid steerpoint type: " << *msg << std::endl;
            std::cerr << " -- valid types are { DEST, MARK, FIX, OAP, TGT }" << std::endl;
            ok = false; // it's no longer ok
        }
    }
    return ok;
}

bool Steerpoint::setSlotLatitude(const Basic::LatLon* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initLatitude = *msg;
        haveInitLat = true;
        setLatitude( initLatitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLatitude(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initLatitude = msg->getDouble();
        haveInitLat = true;
        setLatitude( initLatitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLongitude(const Basic::LatLon* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initLongitude = *msg;
        haveInitLon = true;
        setLongitude( initLongitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLongitude(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initLongitude = msg->getDouble();
        haveInitLon = true;
        setLongitude( initLongitude );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotPosition(const Basic::List* const msg)
{
    bool ok = false;
    if (msg != 0) {
        LCreal values[3];
        int n = msg->getNumberList(values, 3);
        if (n == 3) {
            initPosVec[0] = values[0];
            initPosVec[1] = values[1];
            initPosVec[2] = values[2];
            haveInitPos = true;
            setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
            ok = true;
        }
    }
    return ok;
}

bool Steerpoint::setSlotXPos(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initPosVec[Player::INORTH] = Basic::Meters::convertStatic(*msg);
        haveInitPos = true;
        setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotYPos(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initPosVec[Player::IEAST] = Basic::Meters::convertStatic(*msg);
        haveInitPos = true;
        setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotElevation(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initElev = Basic::Meters::convertStatic(*msg);
        elevation  = initElev;
        initPosVec[Player::IDOWN] = -initElev;
        posVec[Player::IDOWN] = -initElev;
        haveInitElev = true;
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotElevation(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initElev = msg->getReal();
        elevation  = initElev;
        initPosVec[Player::IDOWN] = -initElev;
        posVec[Player::IDOWN] = -initElev;
        haveInitElev = true;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotPTA(const Basic::Time* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setPTA( Basic::Seconds::convertStatic(*msg) );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotPTA(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
		// assumes seconds
        setPTA( msg->getFloat() );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotSCA(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setSCA( Basic::Feet::convertStatic(*msg) );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotSCA(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
		// assumes feet
        setSCA( msg->getFloat() );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotDescription(const Basic::String* const msg)
{
    bool ok = false;
    if (msg != 0) {
        setDescription(msg);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotMagVar(const Basic::Angle* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initMagVar = (LCreal)Basic::Degrees::convertStatic(*msg);
        haveInitMagVar = true;
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotMagVar(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        // assumes degrees
        initMagVar = msg->getReal();
        haveInitMagVar = true;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAltitude(const Basic::Distance* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initCmdAlt = Basic::Meters::convertStatic(*msg);
        haveInitCmdAlt = true;
        setCmdAltitude(initCmdAlt);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAltitude(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initCmdAlt = msg->getReal();
        haveInitCmdAlt = true;
        setCmdAltitude(initCmdAlt);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAirspeed(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initCmdAirspeed = msg->getReal();
        haveInitCmdAs = true;
        setCmdAirspeedKts(initCmdAirspeed);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotNext(const Basic::Identifier* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initNextStptName = msg;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotNext(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        initNextStptIdx = msg->getInt();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// clearNavData() -- Clear the nav data
//------------------------------------------------------------------------------
void Steerpoint::clearNavData()
{
    setTrueBrgDeg(0);
    setMagBrgDeg(0);
    setDistNM(0);
    setTTG(0);
    setCrossTrackErrNM(0);
    setTrueCrsDeg(0);
    setMagCrsDeg(0);
    setLegDistNM(0);
    setLegTime(0);
    setDistEnrouteNM(0);
    setETE(0);
    setETA(0);
    setELT(0);
    navDataValid = false;
}

//------------------------------------------------------------------------------
// compute() -- Compute steerpoint data
//------------------------------------------------------------------------------
bool Steerpoint::compute(const Navigation* const nav, const Steerpoint* const from)
{
    bool ok = false;
    if (nav != 0) {
    
        // ---
        // Update Mag Var (if needed)
        // ---
        if (haveInitMagVar) {
            magvar = initMagVar;
        }
        else {
            magvar = (LCreal) nav->getMagVarDeg();
        }

        // ---
        // Make sure we have a position vector and compute lat/lon, if needed
        // ---
        if ( !isLatLonValid() && isPosVecValid() ) {
            // Compute our lat/long when we only have the Pos Vec
            double elev;
            Basic::Nav::convertPosVec2LL(nav->getRefLatitude(), nav->getRefLongitude(), posVec, &latitude, &longitude, &elev);
            elevation  = (LCreal) elev;
            needLL = false;
        }
        if ( isLatLonValid() && !isPosVecValid() ) {
            // Compute our Pos Vec when we only have the lat/lon
            Basic::Nav::convertLL2PosVec(nav->getRefLatitude(), nav->getRefLongitude(), latitude, longitude, elevation, &posVec);
            needPosVec = false;
        }

        // ## Note: at this point we need a valid lat/lon position
        
        if (isLatLonValid()) {
            
            // ---
            // Compute 'direct-to' bearing,  distance & time
            // ---
            double toBrg = 0.0;
            double toDist = 0.0;
            double toTTG = 0.0;
            Basic::Nav::gll2bd(nav->getLatitude(), nav->getLongitude(), getLatitude(), getLongitude(), &toBrg, &toDist);

            setTrueBrgDeg( LCreal(toBrg) );
            setDistNM( LCreal(toDist) );
            setMagBrgDeg( lcAepcDeg( getTrueBrgDeg() - getMagVarDeg() ) );
            
            if (nav->isVelocityDataValid() && nav->getGroundSpeedKts() > 0.0) {
                toTTG = (toDist/nav->getGroundSpeedKts()) * Basic::Time::H2S;
            }
            setTTG(LCreal(toTTG));

            // ---
            // Compute 'leg' course, distance & time, as well as enroute distance & times
            // ---
            toBrg = 0.0;
            toDist = 0.0;
            toTTG = 0.0;
            if (from != 0) {
                // When we have a 'from' steerpoint, we can compute this leg's data
                Basic::Nav::gll2bd(from->getLatitude(), from->getLongitude(), getLatitude(), getLongitude(), &toBrg, &toDist);
                setTrueCrsDeg( LCreal(toBrg) );
                setMagCrsDeg( lcAepcDeg( getTrueCrsDeg() - getMagVarDeg() ) );
                setLegDistNM( LCreal(toDist) );
                if (nav->isVelocityDataValid() && nav->getGroundSpeedKts() > 0.0) {
                    toTTG = (toDist/nav->getGroundSpeedKts()) * Basic::Time::H2S;
                }
                setLegTime( LCreal(toTTG) );
                setDistEnrouteNM( from->getDistEnrouteNM() + getLegDistNM() );
                setETE( from->getETE() + getLegTime() );
            }
            else {
                // When we don't have a 'from' steerpoint, this leg's is the same as the direct-to data
                setTrueCrsDeg( getTrueBrgDeg() );
                setMagCrsDeg( getMagBrgDeg() );
                setLegDistNM( getDistNM() );
                setLegTime( getTTG() );
                setDistEnrouteNM( getDistNM() );
                setETE( getTTG() );
            }

            // ---
            // Compute Est Time of Arrival and the PTA Early/Late time        
            // ---
            setETA( (LCreal) (getETE() + nav->getUTC()) );
            LCreal delta = getPTA() - getETA();
            if (delta >= Basic::Time::D2S) delta -= Basic::Time::D2S;
            setELT( delta );

            // ---
            // Compute Cross-track error (NM); negitive values are when the desired track
            //  to this point is left of our navigation position
            // ---
            LCreal aa = lcAepcDeg( getTrueBrgDeg() - getTrueCrsDeg() ) * (LCreal)Basic::Angle::D2RCC;
            setCrossTrackErrNM( getDistNM() * lcSin(aa) );

            // ---
            // Update our component steerpoint list (from NAV data, or 'direct-to' only)
            // ---
            Basic::PairStream* steerpoints = getComponents();
            if (steerpoints != 0) {
                Basic::List::Item* item = steerpoints->getFirstItem();
                while (item != 0) {
                    Basic::Pair* pair = (Basic::Pair*)(item->getValue());
                    Steerpoint* p = (Steerpoint*)( pair->object() );
                    p->compute(nav);
                    item = item->getNext();
                }
                steerpoints->unref();
                steerpoints = 0;
            }
        
            // ---
            // Check for safe clearance altitude warning
            // ---
            scaWarn = nav->getAltitudeFt() < getSCA();

            navDataValid = true;

        }
        else {
            navDataValid = false;
        }
    }
    return ok;
}
    
//------------------------------------------------------------------------------
// processComponets() -- process our components; make sure the are all of
//	type Steerpoint (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Steerpoint::processComponents(
      Basic::PairStream* const list,
      const std::type_info&,
      Basic::Pair* const add,
      Basic::Component* const remove
   )
{
   Basic::Component::processComponents(list,typeid(Steerpoint),add,remove);
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Steerpoint::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Steerpoint::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{

    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFormName() << std::endl;
        j = 4;
    }

    // destination
    indent(sout, i+j);
    if (stptType == DEST) sout << "stptType: dest" << std::endl;
    else if (stptType == MARK) sout << "stptType: mark" << std::endl;
    else if (stptType == FIX) sout << "stptType: fix" << std::endl;
    else if (stptType == OAP) sout << "stptType: oap" << std::endl;
    else if (stptType == TGT) sout << "stptType: tgt" << std::endl;

    // latitude (dd.dddd)
    sout.precision(15);

    indent(sout, i+j);
    sout << "latitude: " << latitude << std::endl;

    // longitude (dd.dddd)
    indent(sout, i+j);
    sout << "longitude: " << longitude << std::endl;

    // now back to normal precision
    sout.precision(4);

    // elevation (meters)
    indent(sout, i+j);
    sout << "elevation: " << initElev << std::endl;

    // altitude (meters)
    indent(sout, i+j);
    sout << "altitude: " << cmdAlt << std::endl;

    // airspeed (KTS)
    indent(sout, i+j);
    sout << "airspeed: " << cmdAirspeed << std::endl;

    // planned time of arrival (second)
    if (pta != 0) {
    indent(sout, i+j);
    sout << "pta: " << pta << std::endl;
    }
    // safe clearance altitude (meters)
    if (sca != 0) {
    indent(sout, i+j);
    sout << "sca: " << sca << std::endl;
    }
    // description
    if (description != 0) {
    indent(sout, i+j);
    sout << "description: " << "\"" << *description << "\"" << std::endl;
    }
    // magvar
    indent(sout, i+j);
    sout << "magvar: " << initMagVar << std::endl;

    //BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i + j);
        sout << ")" << std::endl;
    }

    return sout;
}

} // End Simulation namespace
} // End Eaagles namespace
