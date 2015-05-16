
#include "openeaagles/terrain/QuadMap.h"

#include "openeaagles/basic/Terrain.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/NetHandler.h"   // for byte-swapping only
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Terrain {

IMPLEMENT_ABSTRACT_SUBCLASS(QuadMap,"QuadMap")
EMPTY_SLOTTABLE(QuadMap)
EMPTY_SERIALIZER(QuadMap)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
QuadMap::QuadMap()
{
   STANDARD_CONSTRUCTOR()

   numDataFiles = 0;
   for (unsigned int i = 0; i < MAX_DATA_FILES; i++) {
      dataFiles[i] = nullptr;
   }
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void QuadMap::copyData(const QuadMap& org, const bool cc)
{
   // Our base class(s) will will copy our components, which include the
   // original QuadMap's DataFile objects.
   BaseClass::copyData(org);

   if (cc) {
      numDataFiles = 0;
      for (unsigned int i = 0; i < MAX_DATA_FILES; i++) {
         dataFiles[i] = nullptr;
      }
   }

   // Find the new DataFile objects
   findDataFiles();
}


//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void QuadMap::deleteData()
{
    clearData();
}

//------------------------------------------------------------------------------
// reset() -- Reset the simulation & players
//------------------------------------------------------------------------------
void QuadMap::reset()
{
   // Resetting our base class will reset our components,
   // which will load the elevation data files
   BaseClass::reset();

   // Now find our files
   findDataFiles();
}

//------------------------------------------------------------------------------
// Access function(s)
//------------------------------------------------------------------------------

// Has the data been loaded
bool QuadMap::isDataLoaded() const
{
   return (numDataFiles > 0);
}

unsigned int QuadMap::getNumDataFiles() const
{
    return numDataFiles;
}

const Basic::Terrain* QuadMap::getDataFile(const unsigned int i)  const
{
    if (i < MAX_DATA_FILES) return dataFiles[i];
    else return nullptr;
}

//------------------------------------------------------------------------------
// Locates an array of (at least two) elevation points (and sets valid flags if found)
// returns the number of points found within this DataFile
//------------------------------------------------------------------------------
unsigned int QuadMap::getElevations(
      LCreal* const elevations,     // The elevation array (meters)
      bool* const validFlags,       // Valid elevation flag array (true if elevation was found)
      const unsigned int n,         // Size of elevation and valdFlags arrays
      const double lat,             // Starting latitude (degs)
      const double lon,             // Starting longitude (degs)
      const LCreal direction,       // True direction (heading) angle of the data (degs)
      const LCreal maxRng,          // Range to last elevation point (meters)
      const bool interp            // Interpolate between elevation posts (if true)
   ) const
{
   unsigned int num = 0;

   // Early out tests
   if ( elevations == nullptr ||       // The elevation array wasn't provided, or
        validFlags == nullptr ||             // the valid flag array wasn't provided, or
        n < 2 ||                       // there are too few points, or
        (lat < -89.0 || lat > 89.0) || // and we're not starting at the north or south poles
        maxRng <= 0                    // the max range is less than or equal to zero
      ) return num;


   for (unsigned int i = 0; i < numDataFiles && num < n; i++) {
      num += dataFiles[i]->getElevations(elevations,validFlags, n, lat, lon, direction, maxRng, interp);
   }

   return num;
}

//------------------------------------------------------------------------------
// Locates an elevation value (meters) for a given reference point and returns
// it in 'elev'.  Function returns true if successful, otherwise 'elev' is unchanged.
//------------------------------------------------------------------------------
bool QuadMap::getElevation(
      LCreal* const elev,     // The elevation value (meters)
      const double lat,       // Reference latitude (degs)
      const double lon,       // Reference longitude (degs)
      const bool interp       // Interpolate between elevation posts (if true)
   ) const
{

   // Early out tests
   if ( !isDataLoaded() ||          // Not loaded or
        (lat < getLatitudeSW()  ||
         lat > getLatitudeNE()) ||  // wrong latitude or
        (lon < getLongitudeSW() ||
         lon > getLongitudeNE())    // wrong longitude
        ) return false;


   LCreal value = 0.0;              // the elevation (meters)
   bool found = false;
   for (unsigned int i = 0; i < numDataFiles && !found; i++) {
      found = dataFiles[i]->getElevation(&value, lat, lon, interp);
   }

   if (found) {
      *elev = value;
   }
   return found;
}


//------------------------------------------------------------------------------
// Initializes the channel array
//------------------------------------------------------------------------------
void QuadMap::findDataFiles()
{
   // Clear out the old ones
   clearData();

   // Find the DataFile objects
   {
      Basic::PairStream* subcomponents = getComponents();
      if (subcomponents != nullptr) {
         unsigned int count = 0;
         Basic::List::Item* item = subcomponents->getFirstItem();
         while (item != nullptr && count < MAX_DATA_FILES) {
            Basic::Pair* pair = static_cast<Basic::Pair*>( item->getValue() );
            Basic::Terrain* dataFile = dynamic_cast<Basic::Terrain*>( pair->object() );
            if (dataFile != nullptr && dataFile->isDataLoaded()) {
               dataFile->ref();
               dataFiles[count] = dataFile;
               count++;
            }
            item = item->getNext();
         }
         numDataFiles = count;
         subcomponents->unref();
         subcomponents = nullptr;
      }
   }

   // Find the max/min elevations and the corner points
   if (numDataFiles > 0) {
      LCreal elevMin =  999999.0;
      LCreal elevMax = -999999.0;
      double lowerLat = 90.0;
      double lowerLon = 180.0;
      double upperLat = -90.0;
      double upperLon = -180.0;
      for (unsigned int i = 0; i < numDataFiles; i++) {

         if (dataFiles[i]->getMinElevation() < elevMin)
            elevMin = dataFiles[i]->getMinElevation();

         if (dataFiles[i]->getMaxElevation() > elevMax)
            elevMax = dataFiles[i]->getMaxElevation();

         if (dataFiles[i]->getLatitudeSW() < lowerLat)
            lowerLat = dataFiles[i]->getLatitudeSW();

         if (dataFiles[i]->getLongitudeSW() < lowerLon)
            lowerLon = dataFiles[i]->getLongitudeSW();

         if (dataFiles[i]->getLatitudeNE() > upperLat)
            upperLat = dataFiles[i]->getLatitudeNE();

         if (dataFiles[i]->getLongitudeNE() > upperLon)
            upperLon = dataFiles[i]->getLongitudeNE();

      }

      setMinElevation(elevMin);
      setMaxElevation(elevMax);
      setLatitudeSW(lowerLat);
      setLongitudeSW(lowerLon);
      setLatitudeNE(upperLat);
      setLongitudeNE(upperLon);

   }
   else {
      setMinElevation(0);
      setMaxElevation(0);
      setLatitudeSW(0);
      setLongitudeSW(0);
      setLatitudeNE(0);
      setLongitudeNE(0);
   }

}

//------------------------------------------------------------------------------
// This function ignores our components and sets the data file ourself.
// This is here only to help derived classes be able to set which
// data files it wishes to use, then will also override the
// find data files so it this data does not get cleared out.
//------------------------------------------------------------------------------
bool QuadMap::setDataFile(const unsigned int i, Basic::Terrain* newDF)
{
    if (i < MAX_DATA_FILES && newDF != nullptr ) {
        if (dataFiles[i] == nullptr) {
            dataFiles[i] = newDF;
            dataFiles[i]->ref();
            // make sure we increment our # of data files, since this is a new one!
            numDataFiles++;
        }
        else {
            dataFiles[i]->unref();
            dataFiles[i] = newDF;
            dataFiles[i]->ref();
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// Load data
//------------------------------------------------------------------------------
bool QuadMap::loadData()
{
   // We're not loading data --
   // but we contain component DataFile objects that will.
   return true;
}

//------------------------------------------------------------------------------
// clear our data
//------------------------------------------------------------------------------
void QuadMap::clearData()
{
   for (unsigned int i = 0; i < MAX_DATA_FILES; i++) {
      if (dataFiles[i] != nullptr) {
         dataFiles[i]->unref();
         dataFiles[i] = nullptr;
      }
   }
   numDataFiles = 0;
}

}// end Terrain namespace
}// end Eaagles namespace
