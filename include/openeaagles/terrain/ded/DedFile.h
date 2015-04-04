//------------------------------------------------------------------------------
// Class: DedFile
//------------------------------------------------------------------------------
#ifndef __Eaagles_Terrain_DedFile_H__
#define __Eaagles_Terrain_DedFile_H__

#include "../DataFile.h"

namespace Eaagles {
namespace Terrain {

struct DedStdHdr;
struct DedStats;
struct DedCellHdr;

//------------------------------------------------------------------------------
// Class: DedFile
// Description: Manages MultiGen, Inc.'s Digital Elevation Data (DED) file.
// See FORMAT.DED for the original MultiGen documentation.
//
// - CAUTION: We can look at the other Cell Headers, but the only data
//   being made available from Columns are from Cell #0.
//------------------------------------------------------------------------------
class DedFile : public DataFile
{
   DECLARE_SUBCLASS(DedFile,DataFile)

public:
   DedFile();

   // Dump this data to an output stream
   void dump(std::ostream& sout) const;

   bool isDataLoaded() const override;

private:
   void clearHeaders();
   bool getFileHeaders( std::istream& in );
   bool getData( std::istream& in );

   DedCellHdr** cells;     // Array of Cell headers
   DedStdHdr* stdhdr;      // Standard header
   DedStats* fstat;        // Statistics header

   // Terrain::Database private interface
   bool loadData() override;   // Load the data file
};

} // End Terrain namespace
} // End Eaagles namespace

#endif
