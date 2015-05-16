
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <cmath>

#include "openeaagles/terrain/DataFile.h"
#include "openeaagles/basic/NetHandler.h"   // for byte-swapping only
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Terrain {

IMPLEMENT_ABSTRACT_SUBCLASS(DataFile,"DataFile")
EMPTY_SLOTTABLE(DataFile)
EMPTY_SERIALIZER(DataFile)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DataFile::DataFile()
{
   STANDARD_CONSTRUCTOR()

   columns = nullptr;

   latSpacing = 0;
   lonSpacing = 0;
   nptlat = 0;
   nptlong = 0;
   voidValue = -32767; // default void (missing) elevation value
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void DataFile::copyData(const DataFile& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      columns = nullptr;
      nptlat = 0;
      nptlong = 0;
   }

   voidValue = org.voidValue;

   nptlat = org.nptlat;
   nptlong = org.nptlong;

   latSpacing = org.latSpacing;
   lonSpacing = org.lonSpacing;

   if (org.columns != nullptr && org.nptlat > 0 && org.nptlong > 0) {

      // Allocate memory space for the elevation data and copy the data
      columns = new short*[nptlong];
      for (unsigned int i = 0; i < nptlong; i++) {
         if (org.columns[i] != nullptr) {
            columns[i] = new short[nptlat];
            for (unsigned int j = 0; j < nptlat; j++) {
               columns[i][j] = org.columns[i][j];
            }
         }
         else {
            columns[i] = nullptr;
         }
      }

   } // end columns check

}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void DataFile::deleteData()
{
    clearData();
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

// Number of latitude points (# of rows), or zero if the data isn't loaded
unsigned int DataFile::getNumLatPoints() const
{
   unsigned int v = 0;
   if (isDataLoaded()) {
      v = nptlat;
   }
   return v;
}

// Number of longitude points (# of columns), or zero if the data isn't loaded
unsigned int DataFile::getNumLonPoints() const
{
   unsigned int v = 0;
   if (isDataLoaded()) {
      v = nptlong;
   }
   return v;
}

// Spacing between latitude points (degs), or zero if the data isn't loaded
double DataFile::getLatSpacing() const
{
   double v = 0.0;
   if (isDataLoaded()) {
      v = latSpacing;
   }
   return v;
}

// Spacing between longitude points (degs), or zero if the data isn't loaded
double DataFile::getLonSpacing() const
{
   double v = 0.0;
   if (isDataLoaded()) {
      v = lonSpacing;
   }
   return v;
}

const short* DataFile::getColumn(const unsigned int idx) const
{
   const short* p = nullptr;
   if (isDataLoaded() && idx < getNumLonPoints()) {
      p = columns[idx];
   }
   return p;
}

// Has the data been loaded
bool DataFile::isDataLoaded() const
{
   return (columns != nullptr);
}


//------------------------------------------------------------------------------
// Locates an array of (at least two) elevation points (and sets valid flags if found)
// returns the number of points found within this DataFile
//------------------------------------------------------------------------------
unsigned int DataFile::getElevations(
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
        validFlags == nullptr ||       // the valid flag array wasn't provided, or
        n < 2 ||                       // there are too few points, or
        (lat < -89.0 || lat > 89.0) || // and we're not starting at the north or south poles
        maxRng <= 0                    // the max range is less than or equal to zero
      ) return num;


   // Upper limit points
   double maxLatPoint = static_cast<double>(nptlat-1);
   double maxLonPoint = static_cast<double>(nptlong-1);

   // Starting points
   double pointsLat = (lat - getLatitudeSW()) / latSpacing;
   double pointsLon = (lon - getLongitudeSW()) / lonSpacing;

   // Spacing between points (in each direction)
   double deltaPoint = maxRng / (n - 1);
   double dirR = direction * Basic::Angle::D2RCC;
   double deltaNorth = deltaPoint * std::cos(dirR) * Basic::Distance::M2NM;  // (NM)
   double deltaEast  = deltaPoint * std::sin(dirR) * Basic::Distance::M2NM;
   double deltaLat = deltaNorth/60.0;
   double deltaLon = deltaEast/(60.0 * std::cos(lat * Basic::Angle::D2RCC));
   double deltaPointsLat = deltaLat / latSpacing;
   double deltaPointsLon = deltaLon / lonSpacing;

   // ---
   // Loop for the number of points in the arrays;
   // ---
   for (unsigned int i = 0; i < n; i++) {

      if ( !validFlags[i] &&                                // Not already found and
          (pointsLat >= 0 && pointsLat <= maxLatPoint) &&   // and within latitude range and
          (pointsLon >= 0 && pointsLon <= maxLonPoint) ) {  // and within longitude range ...

            // We're within our data limits
            LCreal value = 0;          // the elevation (meters)

            // ---
            // Interpolating between elevation posts?
            // ---
            if (interp) {
               // Yes ---

               // South-west corner post is [icol][irow]
               unsigned int irow = static_cast<unsigned int>(pointsLat);
               unsigned int icol = static_cast<unsigned int>(pointsLon);
               if (irow > (nptlat-2)) irow = (nptlat-2);
               if (icol > (nptlong-2)) icol = (nptlong-2);

               // delta from s-w corner post
               LCreal deltaLat = static_cast<LCreal>(pointsLat - static_cast<double>(irow));
               LCreal deltaLon = static_cast<LCreal>(pointsLon - static_cast<double>(icol));

               // Get the elevations at each corner
               LCreal elevSW = static_cast<LCreal>(columns[icol][irow]);
               LCreal elevNW = static_cast<LCreal>(columns[icol][irow+1]);
               LCreal elevSE = static_cast<LCreal>(columns[icol+1][irow]);
               LCreal elevNE = static_cast<LCreal>(columns[icol+1][irow+1]);

               // Interpolate the west point
               LCreal westPoint = elevSW + (elevNW - elevSW) * deltaLat;

               // Interpolate the east point
               LCreal eastPoint = elevSE + (elevNE - elevSE) * deltaLat;

               // Interpolate between the west and east points
               value = westPoint + (eastPoint - westPoint) * deltaLon;
            }

            else {
               // No -- just use the nearest post

               // Nearest post
               unsigned int irow = static_cast<unsigned int>(pointsLat + 0.5);
               unsigned int icol = static_cast<unsigned int>(pointsLon + 0.5);
               if (irow >= nptlat) irow = (nptlat-1);
               if (icol >= nptlong) icol = (nptlong-1);

               // Get the elevation post at the current indices.
               value = static_cast<LCreal>(columns[icol][irow]);
            }

            // Pass the elevation value and valid flag to the user's arrays
            elevations[i] = value;
            validFlags[i] = true;
            num++;

      } // end lat/lon point checks

      // Update our location within our data array
      pointsLat += deltaPointsLat;
      pointsLon += deltaPointsLon;

   } // end loop

   return num;
}

//------------------------------------------------------------------------------
// Locates an elevation value (meters) for a given reference point and returns
// it in 'elev'.  Function returns true if successful, otherwise 'elev' is unchanged.
//------------------------------------------------------------------------------
bool DataFile::getElevation(
      LCreal* const elev,     // The elevation value (meters)
      const double lat,       // Reference latitude (degs)
      const double lon,       // Reference longitude (degs)
      const bool interp       // Interpolate between elevation posts (if true)
   ) const
{
   LCreal value = 0;          // the elevation (meters)

   // Early out tests
   if ( !isDataLoaded() ||          // Not loaded or
        (lat < getLatitudeSW()  ||
         lat > getLatitudeNE()) ||  // wrong latitude or
        (lon < getLongitudeSW() ||
         lon > getLongitudeNE())    // wrong longitude
        ) return false;

   // ---
   // Compute the lat and lon points
   // ---

   double pointsLat = (lat - getLatitudeSW()) / latSpacing;
   if (pointsLat < 0) pointsLat = 0;

   double pointsLon = (lon - getLongitudeSW()) / lonSpacing;
   if (pointsLon < 0) pointsLon = 0;

   // ---
   // Interpolating between elevation posts?
   // ---
   if (interp) {
      // Yes ---

      // South-west corner post is [icol][irow]
      unsigned int irow = static_cast<unsigned int>(pointsLat);
      unsigned int icol = static_cast<unsigned int>(pointsLon);
      if (irow > (nptlat-2)) irow = (nptlat-2);
      if (icol > (nptlong-2)) icol = (nptlong-2);

      // delta from s-w corner post
      LCreal deltaLat = static_cast<LCreal>(pointsLat - static_cast<double>(irow));
      LCreal deltaLon = static_cast<LCreal>(pointsLon - static_cast<double>(icol));

      // Get the elevations at each corner
      LCreal elevSW = static_cast<LCreal>(columns[icol][irow]);
      LCreal elevNW = static_cast<LCreal>(columns[icol][irow+1]);
      LCreal elevSE = static_cast<LCreal>(columns[icol+1][irow]);
      LCreal elevNE = static_cast<LCreal>(columns[icol+1][irow+1]);

      // Interpolate the west point
      LCreal westPoint = elevSW + (elevNW - elevSW) * deltaLat;

      // Interpolate the east point
      LCreal eastPoint = elevSE + (elevNE - elevSE) * deltaLat;

      // Interpolate between the west and east points
      value = westPoint + (eastPoint - westPoint) * deltaLon;
   }

   else {
      // No -- just use the nearest post

      // Nearest post
      unsigned int irow = static_cast<unsigned int>(pointsLat + 0.5f);
      unsigned int icol = static_cast<unsigned int>(pointsLon + 0.5f);
      if (irow >= nptlat) irow = (nptlat-1);
      if (icol >= nptlong) icol = (nptlong-1);

      // Get the elevation post at the current indices.
      value = static_cast<LCreal>(columns[icol][irow]);
   }

   // ---
   // Return the elevation value to the user
   // ---
   *elev = value;

   return true;
}

//------------------------------------------------------------------------------
// Computes the nearest row index for the latitude (degs).
// Returns true if the index is valid
//------------------------------------------------------------------------------
bool DataFile::computerRowIndex(unsigned int* const irow, const double lat) const
{
   // Early out tests
   if (  (lat < getLatitudeSW() ||
          lat > getLatitudeNE()) ||    // the latitude's out of range, or
         (irow == nullptr) ||                // the 'irow' pointer wasn't provided, or
         !isDataLoaded()               // the data isn't loaded
      ) return false;

   // Locate row (latitude) index
   double points = (lat - getLatitudeSW()) / latSpacing;
   if (points < 0) points = 0;

   unsigned int idx = static_cast<unsigned int>(points + 0.5);
   if (idx >= nptlat) {
      idx = nptlat - 1;
   }

   *irow = idx;
   return true;
}

//------------------------------------------------------------------------------
// Computes the nearest column index for the longitude (degs)
// Returns true if the index is valid
//------------------------------------------------------------------------------
bool DataFile::computeColumnIndex(unsigned int* const icol, const double lon) const
{
   // Early out tests
   if (  (lon < getLongitudeSW() ||
          lon > getLongitudeNE()) ||      // the longitude's out of range, or
         (icol == nullptr) ||                   // the 'icol' pointer wasn't provided, or
         !isDataLoaded()                  // the data isn't loaded
      ) return false;

   // Locate column (longitude) index
   double points = (lon - getLongitudeSW()) / lonSpacing;
   if (points < 0) {
      points = 0.0;
   }
   unsigned int idx = static_cast<unsigned int>(points + 0.5);
   if (idx >= nptlong) {
      idx = nptlong - 1;
   }
   *icol = idx;
   return true;
}

//------------------------------------------------------------------------------
// Computes the latitude (degs) for a given row index.
// Returns true if the latitude is valid
//------------------------------------------------------------------------------
bool DataFile::computeLatitude(double* const lat, const unsigned int irow) const
{
   // Early out tests
   if ( (irow >= nptlat) ||   // the index isn't within range, or
         (lat == nullptr)      ||   // the latitude pointer wasn't provided, or
         !isDataLoaded()      // the data isn't loaded
      ) return false;

   *lat = getLatitudeSW() + static_cast<double>(irow) * latSpacing;
   return true;
}

//------------------------------------------------------------------------------
// Computes the longitude (degs) for a given column index.
// Returns true if the longitude is valid
//------------------------------------------------------------------------------
bool DataFile::computeLongitude(double* const lon, const unsigned int icol) const
{
   // Early out tests
   if ( (icol >= nptlong) ||  // the index isn't within range, or
         (lon == nullptr) ||  // the longitude pointer wasn't provided, or
         !isDataLoaded()      // the data isn't loaded
      ) return false;

   *lon = getLongitudeSW() + static_cast<double>(icol) * lonSpacing;
   return true;
}

//------------------------------------------------------------------------------
// clear our data
//------------------------------------------------------------------------------
void DataFile::clearData()
{
   // Delete the columns of data
   if (columns != nullptr) {
      // Delete the columns of data
      for (unsigned int i = 0; i < nptlong; i++) {
         if (columns[i] != nullptr) {
            delete[] columns[i];
            columns[i] = nullptr;
         }
      }
      // Delete the array of pointers to the columns of data
      delete[] columns;
      columns = nullptr;
   }

   nptlat = 0;
   nptlong = 0;

   setLatitudeSW(0);
   setLongitudeSW(0);
   setLatitudeNE(0);
   setLongitudeNE(0);

   setMinElevation(0);
   setMaxElevation(0);
}

}// end Terrain namespace
}// end Eaagles namespace
