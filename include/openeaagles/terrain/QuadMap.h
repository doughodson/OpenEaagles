//------------------------------------------------------------------------------
// Class: QuadMap
//------------------------------------------------------------------------------
#ifndef __QuadMap_H_E897ADDE_9CF0_49b5_9132_9EA8F76DA500__
#define __QuadMap_H_E897ADDE_9CF0_49b5_9132_9EA8F76DA500__

#include "openeaagles/basic/Terrain.h"

namespace Eaagles {
namespace Terrain {
   class DataFile;

//------------------------------------------------------------------------------
// Class: QuadMap
// Base classes: Basic::Object -> Basic::Component -> Basic::Terrain -> Terrain::QuadMap
//
// Description: Manage up to 4 elevation files in a 2x2 pattern
// Form name: QuadMap
// GUID: {E897ADDE-9CF0-49b5-9132-9EA8F76DA500}
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

   virtual bool isDataLoaded() const;        // Has the data been loaded

   // Locates an array of (at least two) elevation points (and sets valid flags if found)
   // returns the number of points found within this QuadMap
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
   // it in 'elev'.  Function returns true if successfull, otherwise 'elev' is unchanged.
   virtual bool getElevation(
         LCreal* const elev,           // The elevation value (meters)
         const double lat,             // Reference latitude (degs)
         const double lon,             // Reference longitude (degs)
         const bool interp = false     // Interpolate between elevation posts (default: false)
      ) const;

   // Basic::Component interface
   virtual void reset();

protected:
   virtual void findDataFiles();           // Initializes the channel array

   // Basic::Terrain protected interface
   virtual void clearData();

private:
   static const unsigned int MAX_DATA_FILES = 4;    // Only 4 files (as in Quad!)

   const Basic::Terrain* dataFiles[MAX_DATA_FILES]; // Terrain data files
   unsigned int numDataFiles;                       // Number of data files

   // Basic::Terrain private interface
   virtual bool loadData();         // Load the data file
};

} // End Terrain namspace
} // End Eaagles namespace

#endif /* __QuadMap_H_E897ADDE_9CF0_49b5_9132_9EA8F76DA500__ */


