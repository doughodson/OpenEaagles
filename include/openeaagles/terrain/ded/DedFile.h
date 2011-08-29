//------------------------------------------------------------------------------
// Class: DedFile
//------------------------------------------------------------------------------
#ifndef __DedFile_H_6BC3E40A_6A47_4db4_9E76_53DF152FB289__
#define __DedFile_H_6BC3E40A_6A47_4db4_9E76_53DF152FB289__

#include "../DataFile.h"

namespace Eaagles {
namespace Terrain {

struct DedStdHdr;
struct DedStats;
struct DedCellHdr;

//------------------------------------------------------------------------------
// Class: DedFile
// Base classes: Basic::Object -> Basic::Component -> Terrain::Database -> Terrain::DedFile
// GUID: {6BC3E40A-6A47-4db4-9E76-53DF152FB289}
//
// Description: Manages MultiGen, Inc.'s Digitial Elevation Data (DED) file.
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

   // ---
   // Terrain::Database interface
   // ---
   virtual bool isDataLoaded() const;        // Has the data been loaded

private:
   void clearHeaders();
   bool getFileHeaders( std::istream& in );
   bool getData( std::istream& in );

   DedCellHdr** cells;     // Array of Cell headers
   DedStdHdr* stdhdr;      // Standard header
   DedStats* fstat;        // Statistics header

   // Terrain::Database private interface
   virtual bool loadData();   // Load the data file
};

} // End Terrain namspace
} // End Eaagles namespace

#endif /* __DedFile_H_6BC3E40A_6A47_4db4_9E76_53DF152FB289__ */


