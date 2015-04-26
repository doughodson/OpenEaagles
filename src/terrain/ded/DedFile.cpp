
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <cstring>

#include "openeaagles/terrain/ded/DedFile.h"
#include "openeaagles/basic/NetHandler.h"   // for byte-swapping only

namespace Eaagles {
namespace Terrain {

IMPLEMENT_SUBCLASS(DedFile,"DedFile")
EMPTY_SLOTTABLE(DedFile)
EMPTY_SERIALIZER(DedFile)

//------------------------------------------------------------------------------
// MultiGen structure: standard file header block (128 bytes)
//------------------------------------------------------------------------------
struct DedStdHdr {
   uint32_t len;        // size of header block in byte
   char id[4];          // file identification code "SSYS"
   char part[8];        // software part no.
   char rev[8];         // software revision no.
   char create[26];     // creation date/time
   char update[26];     // last update date/time
   char spare[52];      // spare

   DedStdHdr() {
      len = 0;
      for (unsigned int i = 0; i <  4; i++) { id[i]     = '0'; }
      for (unsigned int i = 0; i <  8; i++) { part[i]   = '0'; rev[i]    = '0'; }
      for (unsigned int i = 0; i < 26; i++) { create[i] = '0'; update[i] = '0'; }
      for (unsigned int i = 0; i < 52; i++) { spare[i]  = '0'; }
   }

   DedStdHdr(const DedStdHdr& org) {
      len = org.len;
      for (unsigned int i = 0; i <  4; i++) { id[i]     = org.id[i]; }
      for (unsigned int i = 0; i <  8; i++) { part[i]   = org.part[i];   rev[i]    = org.rev[i]; }
      for (unsigned int i = 0; i < 26; i++) { create[i] = org.create[i]; update[i] = org.update[i]; }
      for (unsigned int i = 0; i < 52; i++) { spare[i]  = org.spare[i]; }
   }
};

//------------------------------------------------------------------------------
// MultiGen structure: Statistics record (32 bytes)
//------------------------------------------------------------------------------
struct DedStats {
   uint32_t  ncell;        // no. of cell in this file
   int16_t minz, maxz;     // min/max z of the entire file
   char  spare[24];        // spare

   DedStats() {
      ncell = 0;
      minz = 0;
      maxz = 0;
      for (unsigned int i = 0; i < 24; i++) { spare[i]  = '0'; }
   }

   DedStats(const DedStats& org) {
      ncell = org.ncell;
      minz = org.minz;
      maxz = org.maxz;
      for (unsigned int i = 0; i < 24; i++) { spare[i]  = org.spare[i]; }
   }
};

//------------------------------------------------------------------------------
// MultiGen structure: Header for each DMA cell on disk (40 bytes)
//------------------------------------------------------------------------------
struct DedCellHdr {
   float latstart, latend;    // lat start end
   float longstart, longend;  // long start end
   float deltalat, deltalong; // post spacing in seconds * 10.0
   float nptlat, nptlong;     // no. of points in lat or long
   float deltax, deltay;      // actual distance between points in feet

   DedCellHdr() {
      latstart  = 0.0;
      latend    = 0.0;
      longstart = 0.0;
      longend   = 0.0;
      deltalat  = 0.0;
      deltalong = 0.0;
      nptlat    = 0.0;
      nptlong   = 0.0;
      deltax    = 0.0;
      deltay    = 0.0;
   }

   DedCellHdr(const DedCellHdr& org) {
      latstart  = org.latstart;
      latend    = org.latend;
      longstart = org.longstart;
      longend   = org.longend;
      deltalat  = org.deltalat;
      deltalong = org.deltalong;
      nptlat    = org.nptlat;
      nptlong   = org.nptlong;
      deltax    = org.deltax;
      deltay    = org.deltay;
   }
};

//------------------------------------------------------------------------------
// Some parameters
//------------------------------------------------------------------------------
static const double M_TO_FT      = 3.28084;
static const double MIN_TO_FT    = 6076.0/60.0;       // 1 minute = 6076 ft based on nautical def.
static const double DEG_TO_RAD   = 0.017453292;
static const char* const SS_STDID = "SSYS";
static const char* const PARTNO  = "DMA1";
static const char* const REVNO   = "V1.0";
static const LCreal NUM_SECS_PER_DEG_10 = 36000.0;    // # seconds in a degree * 10.0

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DedFile::DedFile()
{
   STANDARD_CONSTRUCTOR()

   stdhdr = nullptr;
   fstat = nullptr;
   cells = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void DedFile::copyData(const DedFile& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      stdhdr = nullptr;
      fstat = nullptr;
      cells = nullptr;
   }

   clearHeaders();

   // Copy the headers and data (in order)
   if (org.stdhdr != nullptr) {

      // First the standard header
      stdhdr = new DedStdHdr(*org.stdhdr);

      if (org.fstat != nullptr) {
         // Next, the stats header
         fstat = new DedStats(*org.fstat);

         if (cells != nullptr) {
            // Now the Cells

            // allocated and clear the cell pointers
            cells = new DedCellHdr*[fstat->ncell];
            for (unsigned int i = 0; i < fstat->ncell; i++) {
               // Copy the cell headers
               if (org.cells[i] != nullptr) cells[i] = new DedCellHdr(*org.cells[i]);
               else cells[i] = nullptr;
            }

         } // end cells check
      } // end fstat check
   } // end stdhdr check

}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void DedFile::deleteData()
{
   clearHeaders();
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

// Has the data been loaded
bool DedFile::isDataLoaded() const
{
   return BaseClass::isDataLoaded() && (cells != nullptr);
}

//------------------------------------------------------------------------------
// Load the data file
//------------------------------------------------------------------------------
bool DedFile::loadData()
{
   std::string filename;
   const char* p = getPathname();
   if (p != 0) {
      filename += p;
      filename += '/';
   }
   p = getFilename();
   if (p != 0) filename += p;

   // Open DED file
   std::ifstream in;
   // in.open(filename.c_str(), _IOSbinary);
   in.open(filename.c_str(), std::ios::binary);
   if ( in.fail() ) {
      if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "DedFile::loadData() ERROR, could not open file: " << filename << std::endl;
      }
      return false;
   }

   // Input the file headers */
   bool ok = getFileHeaders(in);

   if ( ok ) {
      // Read in the data
      ok = getData(in);
   }

   // Close the DED file
   in.close();

   return ok;
}

//------------------------------------------------------------------------------
// getFileHeaders() -- read DED file headers; returns true if all went well.
//------------------------------------------------------------------------------
bool DedFile::getFileHeaders( std::istream& in )
{
   bool ok = true;

   // Read file header
   stdhdr = new DedStdHdr();
   in.read( reinterpret_cast<char*>(stdhdr), sizeof(DedStdHdr) );
   if (in.fail() || in.gcount() < sizeof(DedStdHdr)) {
      if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "DedFile::getFileHeaders: invalid standard header.";
      }
      ok = false;
   }

   if (ok) {

      // Byte-swap
      //{
      //   long lTemp = 0;
      //   Basic::NetHandler::fromNetOrder(&lTemp, stdhdr->len);
      //   stdhdr->len = lTemp;
      //}

      // Check ID of input file
      if ( std::strncmp(stdhdr->id,SS_STDID,4) != 0 ) {
         char id[5];
         lcStrncpy(id,5,stdhdr->id,4);
         id[4] = '\0';
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "DedFile::getFileHeaders: invalid id:";
         std::cerr << " should be " << SS_STDID << "; ";
         std::cerr << " is " << id;
         std::cerr << std::endl;
         }
         ok = false;
      }

      // Check Part# of input file
      if ( std::strncmp(stdhdr->part,PARTNO,8) != 0 ) {
         char part[10];
         lcStrncpy(part,10,stdhdr->part,8);
         part[8] = '\0';
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "DedFile::getFileHeaders: invalid part number:";
         std::cerr << " should be " << PARTNO << "; ";
         std::cerr << " is " << part;
         std::cerr << std::endl;
         }
         ok = false;
      }

      if ( std::strncmp(stdhdr->rev,REVNO,8) != 0 ) {    // Check Rev of input file
         char rev[10];
         lcStrncpy(rev,10,stdhdr->rev,8);
         rev[8] = '\0';
         if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "DedFile::getFileHeaders: invalid revision number:";
         std::cerr << " should be " << REVNO << "; ";
         std::cerr << " is " << rev;
         std::cerr << std::endl;
         }
         ok = false;
      }

      if (ok) {
         // Read file header
         fstat = new DedStats();
         in.read( reinterpret_cast<char*>(fstat), sizeof(DedStats) );
         if (in.fail() || in.gcount() < sizeof(DedStats)) {
            if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "DedFile::getFileHeaders: invalid statistics header.";
            }
            ok = false;
         }

         if (ok) {
            {
               // Byte-swap
               uint32_t lTemp = 0;
               int16_t sTemp = 0;
               Basic::NetHandler::fromNetOrder(&lTemp, fstat->ncell);
               fstat->ncell = lTemp;
               Basic::NetHandler::fromNetOrder(&sTemp, fstat->maxz);
               fstat->maxz = sTemp;
               Basic::NetHandler::fromNetOrder(&sTemp, fstat->minz);
               fstat->minz = sTemp;
            }

            #ifdef PRINT
            std::cout
               << " ncells:" << fstat.ncell
               << " minz:" << fstat.minz*M_TO_FT
               << " maxz:" << fstat.maxz*M_TO_FT
               << std::endl;
            #endif

            // allocated and clear the cell pointers
            cells = new DedCellHdr*[fstat->ncell];
            for (unsigned int i = 0; i < fstat->ncell; i++) {
               cells[i] = nullptr;
            }

            // Read the cell headers
            for (unsigned int i = 0; i < fstat->ncell && ok; i++) {
               cells[i] = new DedCellHdr();
               in.read( reinterpret_cast<char*>(cells[i]), sizeof(DedCellHdr) );
               if (in.fail() || in.gcount() < sizeof(DedCellHdr)) {
                   if (isMessageEnabled(MSG_ERROR)) {
                       std::cerr << "DedFile::getFileHeaders: invalid cell headers.";
                       std::cerr << " in.fail:" << in.fail() << " in.gcount:" << in.gcount() << " < size?:" << sizeof(DedCellHdr) << std::endl;
                   }
                  ok = false;
               }

               // Byte-swap
               if (ok) {
                  float fTemp = 0.0;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->latstart);
                  cells[i]->latstart = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->latend);
                  cells[i]->latend = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->longstart);
                  cells[i]->longstart = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->longend);
                  cells[i]->longend = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->deltalat);
                  cells[i]->deltalat = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->deltalong);
                  cells[i]->deltalong = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->nptlat);
                  cells[i]->nptlat = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->nptlong);
                  cells[i]->nptlong = fTemp;
                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->deltax);
                  cells[i]->deltax = fTemp;

                  Basic::NetHandler::fromNetOrder(&fTemp, cells[i]->deltay);
                  cells[i]->deltay = fTemp;

                  #ifdef PRINT
                  std::cout << i << ":cell.latstart:" << cells[i]->latstart << " cell.latend:" << cells[i]->latend << std::endl;
                  std::cout << i << ":cell.longstart:" << cells[i]->longstart << " cell.longend:" << cells[i]->longend << std::endl;
                  std::cout << i << ":cell.deltalat:" << cells[i]->deltalat << " cell.deltalong:" << cells[i]->deltalong << std::endl;
                  std::cout << i << ":cell.nptlat:" << cells[i]->nptlat << " cell.nptlong:" << cells[i]->nptlong << std::endl;
                  std::cout << i << ":cell.deltax:" << cells[i]->deltax << " cell.deltay:" << cells[i]->deltay << std::endl;
                  #endif
               }

            }

            // Pass our data to the baseclass
            if (ok && cells[0] != nullptr) {
               if (cells[0]->latstart <= cells[0]->latend) {
                  setLatitudeNE( cells[0]->latend );
                  setLatitudeSW( cells[0]->latstart );
               }
               else {
                  setLatitudeNE( cells[0]->latstart );
                  setLatitudeSW( cells[0]->latend );
               }
               if (cells[0]->longstart <= cells[0]->longend) {
                  setLongitudeNE( cells[0]->longend );
                  setLongitudeSW( cells[0]->longstart );
               }
               else {
                  setLongitudeNE( cells[0]->longstart );
                  setLongitudeSW( cells[0]->longend );
               }
               nptlat = static_cast<unsigned int>( cells[0]->nptlat  );
               nptlong = static_cast<unsigned int>( cells[0]->nptlong );
               latSpacing = cells[0]->deltalat  / NUM_SECS_PER_DEG_10;
               lonSpacing = cells[0]->deltalong / NUM_SECS_PER_DEG_10;
            }
         }
      }
   }

   // ---
   // If we failed, clean up
   // ---
   if (!ok) clearHeaders();

   return ok;
}

//------------------------------------------------------------------------------
// getData() -- read DED file data  (Cell #0 only!)
//------------------------------------------------------------------------------
bool DedFile::getData( std::istream& in )
{
   bool ok = true;

   // Get number of rows (n) and columns (m) */
   const unsigned int N = nptlat;  // Number of elevations to be read per column
   const unsigned int M = nptlong; // Number of columns

   if (N > 0 && M > 0) {

      // Allocate memory space for the elevation data
      columns = new short*[M];
      for (unsigned int i = 0; i < M; i++) {
         columns[i] = new short[N];
      }

      // reset min/max elevations
      LCreal minElev0 = 99999.0;
      LCreal maxElev0 = 0.0;

      // Read in the data
      const int NUM_BYTES_IN_COL = sizeof(short) * N;
      for (unsigned int i = 0; i < M && ok; i++) {

         // Read the data
         in.read( (char* )columns[i], NUM_BYTES_IN_COL);

         if (in.fail() || in.gcount() < NUM_BYTES_IN_COL) {
            // Read failed!
            if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "DedFile::getData: unable to read data. fail:" << in.fail()
               << " gcount:" << in.gcount()
               << " < BYTES:" << NUM_BYTES_IN_COL
               << std::endl;
            }
            ok = false;
         }
         else {
            // Successful: Byte-swap
            short sTemp = 0;
            for (unsigned int j = 0; j < N; j++ ) {
               Basic::NetHandler::fromNetOrder(&sTemp, columns[i][j]);
               columns[i][j] = sTemp;

               // Check for min/max
               if (columns[i][j] < minElev0) {
                  minElev0 = columns[i][j];
               }
               if (columns[i][j] > maxElev0) {
                  maxElev0 = columns[i][j];
               }
            }
         }

      }
      if (ok) {
         setMinElevation(minElev0);
         setMaxElevation(maxElev0);
      }
   }

   // ---
   // If we failed, clean up
   // ---
   if (!ok) clearData();

   return ok;
}

//------------------------------------------------------------------------------
// clear our headers
//------------------------------------------------------------------------------
void DedFile::clearHeaders()
{
   // Delete the cell headers
   if (cells != nullptr && fstat != nullptr) {
      for (unsigned int i = 0; i < fstat->ncell; i++) {
         if (cells[i] != nullptr) delete cells[i];
      }
      delete cells;
      cells = nullptr;
   }

   // Delete the Statistics header
   if (fstat != nullptr) {
      delete fstat;
      fstat = nullptr;
   }

   // Delete the standard header
   if (stdhdr != nullptr) {
      delete stdhdr;
      stdhdr = nullptr;
   }
}

//------------------------------------------------------------------------------
// Dump this data to an output stream
//------------------------------------------------------------------------------
void DedFile::dump(std::ostream& sout) const
{
   if (isDataLoaded()) {

      sout << "File header:" << std::endl;
      sout << " len     = " << stdhdr->len    << std::endl;
      sout << " id      = " << stdhdr->id     << std::endl;
      sout << " part    = " << stdhdr->part   << std::endl;
      sout << " rev     = " << stdhdr->rev    << std::endl;
      sout << " create  = " << stdhdr->create << std::endl;
      sout << " update  = " << stdhdr->update << std::endl;

      sout << std::endl;
      sout << "File statistics:" << std::endl;
      sout << " ncell = " << fstat->ncell  << std::endl;
      sout << " minz  = " << fstat->minz  << std::endl;
      sout << " maxz  = " << fstat->maxz  << std::endl;

      sout << std::endl;
      sout << "Cell header:" << std::endl;
      sout << " start = " << cells[0]->latstart << ", " << cells[0]->longstart  << std::endl;
      sout << " end   = " << cells[0]->latend   << ", " << cells[0]->longend    << std::endl;
      sout << " d_ll  = " << cells[0]->deltalat << ", " << cells[0]->deltalong  << std::endl;
      sout << " npt   = " << cells[0]->nptlat   << ", " << cells[0]->nptlong    << std::endl;
      sout << " d_ft  = " << cells[0]->deltax   << ", " << cells[0]->deltay     << std::endl;

      for (unsigned int ix = 0; ix < static_cast<unsigned int>(cells[0]->nptlong); ix++) {
         sout << std::endl;
         sout << "ix = " << ix << std::endl << " ";
         unsigned int i = 0;
         const short* colm = columns[ix];
         for (unsigned int iy = 0; iy < static_cast<unsigned int>(cells[0]->nptlat); iy++) {
            sout << " " << std::setw(6) << colm[iy];
            i++;
            if (i >= 10) {
               sout << std::endl << " ";
               i = 0;
            }
         }
         if (i > 0)  sout << std::endl;
      }
   }
}

}// end Terrain namespace
}// end Eaagles namespace
