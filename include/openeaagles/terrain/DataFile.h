//------------------------------------------------------------------------------
// Class: DataFile
//------------------------------------------------------------------------------
#ifndef __oe_terrain_DataFile_H__
#define __oe_terrain_DataFile_H__

#include "openeaagles/base/Terrain.h"

namespace oe {
namespace terrain {

//------------------------------------------------------------------------------
// Class: DataFile
// Description: Common terrain data file
// Factory name: DataFile
//
// Notes:
//    1) the first elevation point [0] of all arrays is at the reference point
//    2) the final elevation point [n-1] is at the maximum range
//    3) The size of all arrays, n, must contain at least 2 points (ref point & max range)
//------------------------------------------------------------------------------
class DataFile : public basic::Terrain
{
   DECLARE_SUBCLASS(DataFile, basic::Terrain)

public:
   DataFile();

   unsigned int getNumLatPoints() const;     // Number of latitude points (# of rows), or zero if the data isn't loaded
   unsigned int getNumLonPoints() const;     // Number of longitude points (# of columns), or zero if the data isn't loaded

   double getLatSpacing() const;             // Spacing between latitude points (degs), or zero if the data isn't loaded
   double getLonSpacing() const;             // Spacing between longitude points (degs), or zero if the data isn't loaded

   // Computes the nearest row index for the latitude (degs).
   // Returns true if the index is valid
   bool computerRowIndex(unsigned int* const irow, const double lat) const;

   // Computes the nearest column index for the longitude (degs)
   // Returns true if the index is valid
   bool computeColumnIndex(unsigned int* const icol, const double lon) const;

   // Computes the latitude (degs) for a given row index.
   // Returns true if the latitude is valid
   bool computeLatitude(double* const lat, const unsigned int irow) const;

   // Computes the longitude (degs) for a given column index.
   // Returns true if the longitude is valid
   bool computeLongitude(double* const lon, const unsigned int icol) const;

   // Returns the idx'th column of elevation data.
   //  There are getNumLonPoints() columns.
   //  Each columns contains getNumLatPoints() elevation points.
   //  Elevations are in meters
   const short* getColumn(const unsigned int idx) const;

   // ---
   // basic::Terrain interface
   // ---

   bool isDataLoaded() const override;

   // Locates an array of (at least two) elevation points (and sets valid flags if found)
   // returns the number of points found within this DataFile
   virtual unsigned int getElevations(
         LCreal* const elevations,     // The elevation array (meters)
         bool* const validFlags,       // Valid elevation flag array (true if elevation was found)
         const unsigned int n,         // Size of elevation and valdFlags arrays
         const double lat,             // Starting latitude (degs)
         const double lon,             // Starting longitude (degs)
         const LCreal direction,       // True direction (heading) angle of the data (degs)
         const LCreal maxRng,          // Range to last elevation point (meters)
         const bool   interp = false   // Interpolate between elevation posts (default: false)
      ) const;

   // Locates an elevation value (meters) for a given reference point and returns
   // it in 'elev'.  Function returns true if successful, otherwise 'elev' is unchanged.
   virtual bool getElevation(
         LCreal* const elev,           // The elevation value (meters)
         const double lat,             // Reference latitude (degs)
         const double lon,             // Reference longitude (degs)
         const bool interp = false     // Interpolate between elevation posts (default: false)
      ) const;

protected:
   short**  columns;                // Array of data columns (values in meters)
   double   latSpacing;             // Spacing between latitude points (degs)
   double   lonSpacing;             // Spacing between longitude points (degs)
   unsigned int nptlat;             // Number of points in latitude (i.e., number of elevations per column)
   unsigned int nptlong;            // Number of points in longitude (i.e., number of columns)
   short    voidValue;              // Value representing a void (missing) data point

   // basic::Terrain protected interface
   void clearData() override;
};

} // End terrain namespace
} // End oe namespace

#endif
