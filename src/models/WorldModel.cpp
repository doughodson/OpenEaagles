
#include "openeaagles/models/WorldModel.hpp"

#include "openeaagles/base/EarthModel.hpp"
#include "openeaagles/base/Identifier.hpp"
#include "openeaagles/base/LatLon.hpp"
#include "openeaagles/base/PairStream.hpp"
#include "openeaagles/base/Pair.hpp"

#include "openeaagles/base/util/nav_utils.hpp"

// environment models
#include "openeaagles/models/environment/AbstractAtmosphere.hpp"
#include "openeaagles/terrain/Terrain.hpp"

#include <cmath>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(WorldModel, "WorldModel")

BEGIN_SLOTTABLE(WorldModel)
   "latitude",                // 1) Ref latitude
   "longitude",               // 2) Ref longitude

   "gamingAreaRange",         // 3) Max valid range of the simulation's gaming area or zero for unlimited
                              //    (default: zero -- unlimited range)

   "earthModel",              // 4) Earth model for geodetic lat/lon (default is WGS-84)

   "gamingAreaUseEarthModel", // 5) If true, use the 'earthModel' or its WGS-84 default for flat
                              //    earth projections between geodetic lat/lon and the gaming
                              //    area's NED coordinates.  Otherwise, use a standard spherical
                              //    earth with a radius of nav::ERAD60. (default: false)

   "terrain",                 //  6) Terrain elevation database
   "atmosphere",              //  7) Atmospheric model
END_SLOTTABLE(WorldModel)

BEGIN_SLOT_MAP(WorldModel)
    ON_SLOT( 1, setSlotRefLatitude,     base::LatLon)
    ON_SLOT( 1, setSlotRefLatitude,     base::Number)

    ON_SLOT( 2, setSlotRefLongitude,    base::LatLon)
    ON_SLOT( 2, setSlotRefLongitude,    base::Number)

    ON_SLOT( 3, setSlotGamingAreaRange, base::Distance)

    ON_SLOT( 4, setSlotEarthModel,      base::EarthModel)
    ON_SLOT( 4, setSlotEarthModel,      base::String)

    ON_SLOT( 5, setSlotGamingAreaEarthModel, base::Number)

    ON_SLOT( 6, setSlotTerrain,      terrain::Terrain)
    ON_SLOT( 7, setSlotAtmosphere,   AbstractAtmosphere)
END_SLOT_MAP()

WorldModel::WorldModel()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void WorldModel::initData()
{
   base::nav::computeWorldMatrix(refLat, refLon, &wm);
}

void WorldModel::copyData(const WorldModel& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setEarthModel( org.em );

   // Center of the gaming area
   refLat = org.refLat;
   refLon = org.refLon;
   sinRlat = org.sinRlat;
   cosRlat = org.cosRlat;
   maxRefRange = org.maxRefRange;
   gaUseEmFlg = org.gaUseEmFlg;
   wm = org.wm;


   if (org.terrain != nullptr) {
      terrain::Terrain* copy = org.terrain->clone();
      setSlotTerrain( copy );
      copy->unref();
   }
   else {
      setSlotTerrain(nullptr);
   }

   if (org.atmosphere != nullptr) {
      AbstractAtmosphere* copy = org.atmosphere->clone();
      setSlotAtmosphere( copy );
      copy->unref();
   }
   else {
      setSlotAtmosphere(nullptr);
   }
}

void WorldModel::deleteData()
{
   setSlotAtmosphere( nullptr );
   setSlotTerrain( nullptr );
}

void WorldModel::reset()
{
   BaseClass::reset();

   // ---
   // First time reset of terrain database will load the data
   // ---
   if (terrain != nullptr) {
      std::cout << "Loading Terrain Data..." << std::endl;
      terrain->reset();
      std::cout << "Finished!" << std::endl;
   }

   // ---
   // Reset atmospheric model
   // ---
   if (atmosphere != nullptr) atmosphere->reset();
}

bool WorldModel::shutdownNotification()
{
   // ---
   // Shutdown our baseclass, which will notify our components
   // ---
   BaseClass::shutdownNotification();

   // ---
   // Tell the environments ...
   // ---
   if (atmosphere != nullptr) atmosphere->event(SHUTDOWN_EVENT);
   if (terrain != nullptr) terrain->event(SHUTDOWN_EVENT);

   return true;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Returns a pointer to the EarthModel
const base::EarthModel* WorldModel::getEarthModel() const
{
   return em;
}

// Gaming area using the earth model?
bool WorldModel::isGamingAreaUsingEarthModel() const
{
   return gaUseEmFlg;
}

// Returns the reference latitude
double WorldModel::getRefLatitude() const
{
   return refLat;
}

// Returns the reference longitude
double WorldModel::getRefLongitude() const
{
   return refLon;
}

// Returns the sine of the reference latitude
double WorldModel::getSinRefLat() const
{
   return sinRlat;
}

// Returns the cosine of the reference latitude
double WorldModel::getCosRefLat() const
{
   return cosRlat;
}

// Max valid range (meters) of the gaming area or zero if there's no limit.
double WorldModel::getMaxRefRange() const
{
   return maxRefRange;
}

// World transformation matrix:
const base::Matrixd& WorldModel::getWorldMat() const
{
   return wm;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

bool WorldModel::setEarthModel(const base::EarthModel* const msg)
{
   if (em != nullptr) {
      em->unref();
      em = nullptr;
   }

   if (msg != nullptr) {
      em = msg->clone();
   }

   return true;
}

bool WorldModel::setGamingAreaUseEarthModel(const bool flg)
{
   gaUseEmFlg = flg;
   return true;
}

// Sets Ref latitude
bool WorldModel::setRefLatitude(const double v)
{
   bool ok = (v <= 90.0 && v >= -90.0);
   if (ok) {
      // Set the latitude and compute the world matrix
      refLat = v;
      const double r = base::angle::D2RCC * refLat;
      sinRlat = std::sin(r);
      cosRlat = std::cos(r);
      base::nav::computeWorldMatrix(refLat, refLon, &wm);
   }
   return ok;
}

// Sets Ref longitude
bool WorldModel::setRefLongitude(const double v)
{
   bool ok = (v <= 180.0 && v >= -180.0);
   if (ok) {
      // Set the longitude and compute the world matrix
      refLon = v;
      base::nav::computeWorldMatrix(refLat, refLon, &wm);
   }
   return ok;
}

// Sets the max range (meters) of the gaming area or zero if there's no limit.
bool WorldModel::setMaxRefRange(const double v)
{
   bool ok = (v >= 0);
   if (ok) maxRefRange = v;
   return ok;
}

//------------------------------------------------------------------------------
// Set Slot routines
//------------------------------------------------------------------------------

bool WorldModel::setSlotRefLatitude(const base::LatLon* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLatitude(msg->getDouble());
    }
    return ok;
}

bool WorldModel::setSlotRefLatitude(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLatitude(msg->getDouble());
    }
    return ok;
}

bool WorldModel::setSlotRefLongitude(const base::LatLon* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLongitude(msg->getDouble());
    }
    return ok;
}

bool WorldModel::setSlotRefLongitude(const base::Number* const msg)
{
    bool ok = false;
    if (msg != nullptr) {
        ok = setRefLongitude(msg->getDouble());
    }
    return ok;
}

bool WorldModel::setSlotGamingAreaRange(const base::Distance* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setMaxRefRange( base::Meters::convertStatic(*msg) );
   }
   return ok;
}

bool WorldModel::setSlotEarthModel(const base::EarthModel* const msg)
{
   return setEarthModel(msg);
}

bool WorldModel::setSlotEarthModel(const base::String* const msg)
{
   bool ok = false;
   if (msg != nullptr && msg->len() > 0) {
      const base::EarthModel* p = base::EarthModel::getEarthModel(*msg);
      if (p != nullptr) {
         // found the earth model
         ok = setEarthModel(p);
      }
      else {
         // Earth model not found by name
         std::cerr << "simulation::setSlotEarthModel() earth model not found: " << *msg << std::endl;
      }
   }
   else {
      // clear the earth model with a empty string
      ok = setEarthModel(nullptr);
   }
   return ok;
}

bool WorldModel::setSlotGamingAreaEarthModel(const base::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setGamingAreaUseEarthModel(msg->getBoolean());
   }
   return ok;
}

std::ostream& WorldModel::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        indent(sout,i);
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

   // ref lat/lon position
   {
      double v = getRefLatitude();
      indent(sout,i+j);
      sout << "latitude: ( LatLon";

      sout << " direction: ";
      if (v >= 0) { sout << "n"; }
      else { sout << "s"; v = -v; }

      sout << " degrees: " << v;
      sout << " )" << std::endl;
   }
   {
      double v = getRefLongitude();

      indent(sout,i+j);
      sout << "longitude: ( LatLon";

      sout << " direction: ";
      if (v >= 0) { sout << "e"; }
      else { sout << "w"; v = -v; }

      sout << " degrees: " << v;
      sout << " )" << std::endl;
   }

    BaseClass::serialize(sout,i+j,true);

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

// returns the terrain elevation database
const terrain::Terrain* WorldModel::getTerrain() const
{
   return terrain;
}

terrain::Terrain* WorldModel::getTerrain()
{
   return terrain;
}

// returns the atmosphere model
AbstractAtmosphere* WorldModel::getAtmosphere()
{
   return atmosphere;
}

// returns the atmospheric model
const AbstractAtmosphere* WorldModel::getAtmosphere() const
{
   return atmosphere;
}

bool WorldModel::setSlotTerrain(terrain::Terrain* const msg)
{
   if (terrain != nullptr) terrain->unref();
   terrain = msg;
   if (terrain != nullptr) terrain->ref();
   return true;
}

bool WorldModel::setSlotAtmosphere(AbstractAtmosphere* const msg)
{
   if (atmosphere != nullptr) atmosphere->unref();
   atmosphere = msg;
   if (atmosphere != nullptr) atmosphere->ref();
   return true;
}

}
}

