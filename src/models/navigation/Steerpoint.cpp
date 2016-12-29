
#include "openeaagles/models/navigation/Steerpoint.hpp"

#include "openeaagles/models/navigation/Navigation.hpp"
#include "openeaagles/models/players/Player.hpp"
#include "openeaagles/models/Actions.hpp"
#include "openeaagles/models/Simulation.hpp"

#include "openeaagles/terrain/ITerrain.hpp"

#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/LatLon.hpp"
#include "openeaagles/base/List.hpp"
#include "openeaagles/base/Nav.hpp"
#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/String.hpp"
#include "openeaagles/base/units/Angles.hpp"
#include "openeaagles/base/units/Distances.hpp"
#include "openeaagles/base/units/Times.hpp"

#include <cmath>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(Steerpoint, "Steerpoint")

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
    ON_SLOT( 1, setSlotSteerpointType, base::Identifier)

    ON_SLOT( 2, setSlotLatitude,       base::LatLon)
    ON_SLOT( 2, setSlotLatitude,       base::Number)

    ON_SLOT( 3, setSlotLongitude,      base::LatLon)
    ON_SLOT( 3, setSlotLongitude,      base::Number)

    ON_SLOT( 4, setSlotXPos,           base::Distance)

    ON_SLOT( 5, setSlotYPos,           base::Distance)

    ON_SLOT( 6, setSlotElevation,      base::Distance)
    ON_SLOT( 6, setSlotElevation,      base::Number)

    ON_SLOT( 7, setSlotCmdAltitude,    base::Distance)
    ON_SLOT( 7, setSlotCmdAltitude,    base::Number)

    ON_SLOT( 8, setSlotCmdAirspeed,    base::Number)

    ON_SLOT( 9, setSlotPTA,            base::Time)
    ON_SLOT( 9, setSlotPTA,            base::Number)

    ON_SLOT(10, setSlotSCA,            base::Distance)
    ON_SLOT(10, setSlotSCA,            base::Number)

    ON_SLOT(11, setSlotDescription,    base::String)

    ON_SLOT(12, setSlotMagVar,         base::Angle)
    ON_SLOT(12, setSlotMagVar,         base::Number)

    ON_SLOT(13, setSlotNext,           base::Identifier)
    ON_SLOT(13, setSlotNext,           base::Number)

    ON_SLOT(14, setAction,             Action)
END_SLOT_MAP()

Steerpoint::Steerpoint()
{
    STANDARD_CONSTRUCTOR()

    description = nullptr;
    action = nullptr;

    latitude = 0.0;
    longitude = 0.0;
    elevation = 0.0;
    posVec.set(0,0,0);
    stptType = DEST;
    pta = 0.0;
    sca = 0.0;
    magvar = 0.0;
    needPosVec = true;
    needLL = true;
    cmdAlt = 0.0;
    haveCmdAlt = false;
    cmdAirspeed = 0.0;
    haveCmdAs = false;
    next = nullptr;

    initLatitude = 0.0;
    initLongitude = 0.0;
    initElev = 0.0;
    initPosVec.set(0,0,0);
    initMagVar = 0.0;
    haveInitLat = false;
    haveInitLon = false;
    haveInitPos = false;
    haveInitMagVar = false;
    haveInitElev = false;
    initCmdAlt = 0.0;
    haveInitCmdAlt = false;
    initCmdAirspeed = 0.0;
    haveInitCmdAs = false;
    initNextStptName = nullptr;
    initNextStptIdx = 0;

    tbrg = 0.0;
    mbrg = 0.0;
    dst = 0.0;
    ttg = 0.0;
    xte = 0.0;

    tcrs = 0.0;
    mcrs = 0.0;
    tlt = 0.0;
    tld = 0.0;

    tde = 0.0;
    ete = 0.0;
    eta = 0.0;
    elt = 0.0;
    scaWarn = false;
    navDataValid = false;
}

void Steerpoint::copyData(const Steerpoint& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        next = nullptr;
        initNextStptName = nullptr;
        description = nullptr;
        action = nullptr;
    }

    next = nullptr; // find it using 'initNextStptName' or 'initNexStptIdx'

    {
       base::String* n = nullptr;
       if (org.initNextStptName != nullptr) n = org.initNextStptName->clone();
       initNextStptName = n;
       if (n != nullptr) n->unref();
    }

    initNextStptIdx = org.initNextStptIdx;

    {
       base::String* s = nullptr;
       if (org.description != nullptr) s = org.description->clone();
       description = s;
       if (s != nullptr) s->unref();
    }

    {
       Action* aa = nullptr;
       if (org.action != nullptr) aa = org.action->clone();
       action = aa;
       if (aa != nullptr) aa->unref();
    }

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

void Steerpoint::deleteData()
{
    next = nullptr;
    initNextStptName = nullptr;
    description = nullptr;
}

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

double Steerpoint::getElevationFt() const
{
    return getElevationM() * base::Distance::M2FT;
}

const char* Steerpoint::getDescription() const
{
    const char* p = nullptr;
    if (description != nullptr) p = *description;
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

double Steerpoint::getCmdAltitudeFt() const
{
    return getCmdAltitude() * base::Distance::M2FT;
}

//------------------------------------------------------------------------------
// Set the ground elevation at the steerpoint from this terrain database
// Returns true if successful.
//------------------------------------------------------------------------------
bool Steerpoint::setElevation(const terrain::ITerrain* const terrain, const bool interp)
{
   bool ok = false;
   if (!needLL) {
      double elev = 0;
      ok = terrain->getElevation(&elev, latitude, longitude, interp);
      if (ok) setElevation(elev);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Data set functions
//------------------------------------------------------------------------------
void Steerpoint::setPosition(const double x, const double y, const double z)
{
    posVec.set(x, y, z);
    needPosVec = false;
    needLL = true;
}

void Steerpoint::setPosition(const osg::Vec3d& newPos)
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

void Steerpoint::setElevation(const double x)
{
    elevation = x;
    posVec[Player::IDOWN] = -x;
}

void Steerpoint::setDescription(const base::String* const d)
{
    description = d;
}

void Steerpoint::setCmdAltitude(const double x)
{
   cmdAlt = x;
   haveCmdAlt = true;
}

void Steerpoint::setCmdAirspeedKts(const double x)
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
bool Steerpoint::setSlotSteerpointType(const base::Identifier* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = true;  // assume that it's valid
        if      (*msg == "DEST"  || *msg == "dest")  setSteerpointType(DEST);
        else if (*msg == "MARK"  || *msg == "mark")  setSteerpointType(MARK);
        else if (*msg == "FIX"   || *msg == "fix")   setSteerpointType(FIX);
        else if (*msg == "OAP"   || *msg == "oap")   setSteerpointType(OAP);
        else if (*msg == "TGT"   || *msg == "tgt")   setSteerpointType(TGT);
        else if (*msg == "IP"    || *msg == "ip")    setSteerpointType(IP);
        else {
            std::cerr << "xxx(): invalid steerpoint type: " << *msg << std::endl;
            std::cerr << " -- valid types are { DEST, MARK, FIX, OAP, TGT }" << std::endl;
            ok = false; // it's no longer ok
        }
    }
    return ok;
}

bool Steerpoint::setSlotLatitude(const base::LatLon* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initLatitude = *msg;
        haveInitLat = true;
        setLatitude( initLatitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLatitude(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initLatitude = msg->getDouble();
        haveInitLat = true;
        setLatitude( initLatitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLongitude(const base::LatLon* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initLongitude = *msg;
        haveInitLon = true;
        setLongitude( initLongitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLongitude(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initLongitude = msg->getDouble();
        haveInitLon = true;
        setLongitude( initLongitude );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotPosition(const base::List* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        double values[3];
        const int n = msg->getNumberList(values, 3);
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

bool Steerpoint::setSlotXPos(const base::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initPosVec[Player::INORTH] = base::Meters::convertStatic(*msg);
        haveInitPos = true;
        setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotYPos(const base::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initPosVec[Player::IEAST] = base::Meters::convertStatic(*msg);
        haveInitPos = true;
        setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotElevation(const base::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initElev = base::Meters::convertStatic(*msg);
        elevation  = initElev;
        initPosVec[Player::IDOWN] = -initElev;
        posVec[Player::IDOWN] = -initElev;
        haveInitElev = true;
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotElevation(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initElev = msg->getReal();
        elevation  = initElev;
        initPosVec[Player::IDOWN] = -initElev;
        posVec[Player::IDOWN] = -initElev;
        haveInitElev = true;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotPTA(const base::Time* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        setPTA( base::Seconds::convertStatic(*msg) );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotPTA(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        // assumes seconds
        setPTA( msg->getFloat() );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotSCA(const base::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        setSCA( base::Feet::convertStatic(*msg) );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotSCA(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        // assumes feet
        setSCA( msg->getFloat() );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotDescription(const base::String* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        setDescription(msg);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotMagVar(const base::Angle* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initMagVar = static_cast<double>(base::Degrees::convertStatic(*msg));
        haveInitMagVar = true;
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotMagVar(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        // assumes degrees
        initMagVar = msg->getReal();
        haveInitMagVar = true;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAltitude(const base::Distance* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initCmdAlt = base::Meters::convertStatic(*msg);
        haveInitCmdAlt = true;
        setCmdAltitude(initCmdAlt);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAltitude(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initCmdAlt = msg->getReal();
        haveInitCmdAlt = true;
        setCmdAltitude(initCmdAlt);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAirspeed(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initCmdAirspeed = msg->getReal();
        haveInitCmdAs = true;
        setCmdAirspeedKts(initCmdAirspeed);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotNext(const base::Identifier* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        initNextStptName = msg;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotNext(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
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
    if (nav != nullptr) {

        // ---
        // Update Mag Var (if needed)
        // ---
        if (haveInitMagVar) {
            magvar = initMagVar;
        }
        else {
            magvar = static_cast<double>(nav->getMagVarDeg());
        }

        // ---
        // Make sure we have a position vector and compute lat/lon, if needed
        // ---
        if ( !isLatLonValid() && isPosVecValid() ) {
            // Compute our lat/lon when we only have the Pos Vec
            double elev = 0.0;
            base::Nav::convertPosVec2LL(nav->getRefLatitude(), nav->getRefLongitude(), posVec, &latitude, &longitude, &elev);
            elevation  = static_cast<double>(elev);
            needLL = false;
        }
        if ( isLatLonValid() && !isPosVecValid() ) {
            // Compute our Pos Vec when we only have the lat/lon
            base::Nav::convertLL2PosVec(nav->getRefLatitude(), nav->getRefLongitude(), latitude, longitude, elevation, &posVec);
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
            base::Nav::gll2bd(nav->getLatitude(), nav->getLongitude(), getLatitude(), getLongitude(), &toBrg, &toDist);

            setTrueBrgDeg( static_cast<double>(toBrg) );
            setDistNM( static_cast<double>(toDist) );
            setMagBrgDeg( base::Angle::aepcdDeg( getTrueBrgDeg() - getMagVarDeg() ) );

            if (nav->isVelocityDataValid() && nav->getGroundSpeedKts() > 0.0) {
                toTTG = (toDist/nav->getGroundSpeedKts()) * base::Time::H2S;
            }
            setTTG(static_cast<double>(toTTG));

            // ---
            // Compute 'leg' course, distance & time, as well as enroute distance & times
            // ---
            toBrg = 0.0;
            toDist = 0.0;
            toTTG = 0.0;
            if (from != nullptr) {
                // When we have a 'from' steerpoint, we can compute this leg's data
                base::Nav::gll2bd(from->getLatitude(), from->getLongitude(), getLatitude(), getLongitude(), &toBrg, &toDist);
                setTrueCrsDeg( static_cast<double>(toBrg) );
                setMagCrsDeg( base::Angle::aepcdDeg( getTrueCrsDeg() - getMagVarDeg() ) );
                setLegDistNM( static_cast<double>(toDist) );
                if (nav->isVelocityDataValid() && nav->getGroundSpeedKts() > 0.0) {
                    toTTG = (toDist/nav->getGroundSpeedKts()) * base::Time::H2S;
                }
                setLegTime( static_cast<double>(toTTG) );
                setDistEnrouteNM( from->getDistEnrouteNM() + getLegDistNM() );
                setETE( from->getETE() + getLegTime() );
            }
            else {
                // When we don't have a 'from' steerpoint, this leg is the same as the direct-to data
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
            setETA( static_cast<double>(getETE() + nav->getUTC()) );
            double delta = getPTA() - getETA();
            if (delta >= base::Time::D2S) delta -= base::Time::D2S;
            setELT( delta );

            // ---
            // Compute Cross-track error (NM); negative values are when the desired track
            //  to this point is left of our navigation position
            // ---
            double aa = base::Angle::aepcdDeg( getTrueBrgDeg() - getTrueCrsDeg() ) * static_cast<double>(base::Angle::D2RCC);
            setCrossTrackErrNM( getDistNM() * std::sin(aa) );

            // ---
            // Update our component steerpoint list (from NAV data, or 'direct-to' only)
            // ---
            base::PairStream* steerpoints = getComponents();
            if (steerpoints != nullptr) {
                base::List::Item* item = steerpoints->getFirstItem();
                while (item != nullptr) {
                    base::Pair* pair = static_cast<base::Pair*>(item->getValue());
                    Steerpoint* p = static_cast<Steerpoint*>(pair->object());
                    p->compute(nav);
                    item = item->getNext();
                }
                steerpoints->unref();
                steerpoints = nullptr;
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
// type Steerpoint (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Steerpoint::processComponents(
      base::PairStream* const list,
      const std::type_info&,
      base::Pair* const add,
      base::Component* const remove
   )
{
   base::Component::processComponents(list,typeid(Steerpoint),add,remove);
}

std::ostream& Steerpoint::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{

    int j = 0;
    if ( !slotsOnly ) {
        //indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
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
    if (description != nullptr) {
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

}
}
