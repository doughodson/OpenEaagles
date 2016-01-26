//------------------------------------------------------------------------------
// Class: DedFile
//------------------------------------------------------------------------------
#ifndef __oe_terrain_DedFile_H__
#define __oe_terrain_DedFile_H__

#include "../DataFile.h"

namespace oe {
namespace terrain {

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

   // terrain::Database private interface
   bool loadData() override;   // Load the data file
};

} // End terrain namespace
} // End oe namespace

#endif
