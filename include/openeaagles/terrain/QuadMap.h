//------------------------------------------------------------------------------
// Class: QuadMap
//------------------------------------------------------------------------------
#ifndef __Eaagles_Terrain_QuadMap_H__
#define __Eaagles_Terrain_QuadMap_H__

#include "openeaagles/basic/Terrain.h"

namespace Eaagles {
namespace Terrain {
   class DataFile;

//------------------------------------------------------------------------------
// Class: QuadMap
// Description: Manage up to 4 elevation files in a 2x2 pattern
// Factory name: QuadMap
//------------------------------------------------------------------------------
class QuadMap : public Basic::Terrain
{
   DECLARE_SUBCLASS(QuadMap,Basic::Terrain)

public:
   QuadMap();

    unsigned int getNumDataFiles() const;
    const Basic::Terrain* getDataFile(const unsigned int i) const;
    bool setDataFile(const unsigned int i, Basic::Terrain* newDF);

   // ---
   // Basic::Terrain interface
   // ---

   bool isDataLoaded() const override;

   // Locates an array of (at least two) elevation points (and sets valid flags if found)
   // returns the number of points found within this QuadMap
   unsigned int getElevations(
         LCreal* const elevations,     // The elevation array (meters)
         bool* const validFlags,       // Valid elevation flag array (true if elevation was found)
         const unsigned int n,         // Size of elevation and valdFlags arrays
         const double lat,             // Starting latitude (degs)
         const double lon,             // Starting longitude (degs)
         const LCreal direction,       // True direction (heading) angle of the data (degs)
         const LCreal maxRng,          // Range to last elevation point (meters)
         const bool   interp = false   // Interpolate between elevation posts (default: false)
      ) const override;

   // Locates an elevation value (meters) for a given reference point and returns
   // it in 'elev'.  Function returns true if successful, otherwise 'elev' is unchanged.
   bool getElevation(
         LCreal* const elev,           // The elevation value (meters)
         const double lat,             // Reference latitude (degs)
         const double lon,             // Reference longitude (degs)
         const bool interp = false     // Interpolate between elevation posts (default: false)
      ) const override;

   void reset() override;

protected:
   virtual void findDataFiles();           // Initializes the channel array

   void clearData() override;

private:
   static const unsigned int MAX_DATA_FILES = 4;    // Only 4 files (as in Quad!)

   const Basic::Terrain* dataFiles[MAX_DATA_FILES]; // Terrain data files
   unsigned int numDataFiles;                       // Number of data files

   bool loadData() override;
};

} // End Terrain namespace
} // End Eaagles namespace

#endif
