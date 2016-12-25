
#ifndef __oe_dafif_Database_H__
#define __oe_dafif_Database_H__

#include "openeaagles/base/Object.hpp"
#include "dafifc.hpp"

namespace oe {
namespace base { class FileReader; class String; }
namespace dafif {

//------------------------------------------------------------------------------
// Class: Database
//
// Description: Abstract class used to access DAFIF databases; both file
//              loaders and network clients.
//
//  Notes:
//   1) Many of the member functions of this DAFIF database manager class and
//      its derived classes read files.  The time required to read these files
//      can not be determined.  Therefore, do not access the DAFIF database
//      classes from a time critical thread.
//
//   2) Many of the member functions work together to query and retrieve data
//      records.  One a single thread (single user) application, this is not
//      a problem.  However, on a multi-thread (multi-user) application, the
//      in-use member functions should be used to protect the data during a
//      data query and retrieval operation.
//      Example --
//           if (requestDbInUse()) {
//               // We have exclusive use of the database ...
//               setArea(lat,lon,rng);   // sets the ref area, as needed
//               queryByIdent();         // query for the required records
//               ....                    // access the records
//               clearDbInse();          // free the database
//           }
//
//
// Public member functions:
//
//    bool requestDbInUse()
//       Requests the exclusive use of the database.  Returns true if use is
//       granted, or false if the database is already in-use.
//
//    bool clearDbInUse()
//       Clears the database in-use flag.  Returns the previous state of the
//       in-use flag.
//
//    int recordSize()
//       Returns the number of characters in this DAFIF record.
//
//    int numberOfRecords() const
//       Returns the number of records loaded from the file.
//
//    int numberFound() const
//       Returns the number of records found by the last query.
//
//    setArea(float lat, float lon)
//    setArea(float lat, float lon, float mrng)
//       Sets the search area (ref point) for queries to lat and lon.
//       If mrng is provided, then query results are limited to mrng.
//
//    getArea(double* lat, double* lon)
//    getArea(double* lat, double* lon, double* mrng)
//       Return the search area (ref point).
//
//    setQueryLimit()
//    setQueryLimit(int mrec)
//       If mrec is provided, then the limit on records found by a
//       query is set to mrec.  If mrec is not provided, then there
//       is no limit on the number of records found by a query.
//
//    int getQueryLimit()
//       Returns the limit on records found by a query.  Zero is
//       returned if no limit exists on the number of records found
//       by a query.
//
//    int queryByRange()
//       Find all records within the search area set by setArea()
//
//    int queryByIdent(const char* id)
//       Find all records within the search area with identifier 'id'.
//
//    int queryByIcao(const char* code)
//       Find all records within the search area with ICAO code.
//
//    int queryByKey(const char* key)
//       Find the record with 'key'.
//
//    const char* record(int n)
//    const char* record(int n, int size)
//       Returns a pointer to n'th record loaded.  This points to an
//       internal buffer and is only valid until the next file access.
//       Range: 0 .. numberOfRecords()-1
//
//    const char* getRecord(int n)
//    const char* getRecord(int n, int size)
//       Get a pointer to the n'th record found by last query.  This
//       points to an internal buffer that is only valid until the
//       next file access.   Range: 0 .. numberFound()-1
//
//    printLoaded(std::ostream& sout)
//       Prints the records loaded.
//
//    printResults(std::ostream& sout)
//       Prints the records found by the last query.
//
//    printIcaoList(std::ostream& sout)
//       Prints the records in the ICAO sorted list
//
//------------------------------------------------------------------------------
class Database : public base::Object
{
   DECLARE_SUBCLASS(Database, base::Object)

public:
   Database();

   bool requestDbInUse()               { return ( (dbInUse && dbLoaded) ? false : (dbInUse = true)); }
   bool clearDbInUse()                 { return (dbInUse ? !(dbInUse = false) : false); }
   bool isDbLoader() const             { return dbLoaded; }

   virtual int numberOfRecords();
   virtual int numberFound();

   virtual void getArea(double* const lat, double* const lon);
   virtual void getArea(double* const lat, double* const lon, double* const mrng);
   virtual void setArea(const double lat, const double lon, const double mrng = 0);

   virtual void setQueryLimit(const int mrec = 0);
   virtual int  getQueryLimit();

   virtual const char* record(const int n, const int size = 0);
   virtual const char* getRecord(const int n, const int size = 0);

   virtual int getRecordLength() = 0;
   virtual int getMaxRecords() = 0;

   virtual bool load(const char* code = 0) = 0;

   virtual int queryByRange() = 0;
   virtual int queryByIdent(const char* id) = 0;
   virtual int queryByIcao(const char* code);
   virtual int queryByKey(const char* key) = 0;

   virtual void printLoaded(std::ostream& sout);
   virtual void printResults(std::ostream& sout);
   virtual void printIcaoList(std::ostream& sout);

   const char* getPathname() const;
   const char* getFilename() const;
   bool setPathname(const char* path);
   bool setFilename(const char* file);

   // Memory key used for quick dafif record lookup
   struct Key {
      int      idx;     // Database index
      int      size;    // Size of record

      double   lat;     // Latitude
      double   lon;     // Longitude

      double   rng2;    // Range squared to ref point (nm**2)

      char icao[ICAO_CODE_LEN+1]; // ICAO code

      Key(const int idx);
      Key(const float lat, const float lon);
      Key(const char* code);
      ~Key();
      void serialize(std::ostream& sout) const;
   };

protected:
   bool setSlotPathname(base::String* const msg);
   bool setSlotFilename(base::String* const msg);

   bool openDatabaseFile();

   const char* dbGetRecord(const Key* key, const int size = 0);

   int sQuery(Key** key, Key** base, size_t n,
               int (*cmp)(const void*, const void*));

   int mQuery(Key** key, Key** base, size_t n,
               int (*cmp)(const void*, const void*));

   void expandResults(Key** key, Key** keyPtr,
                        int (*cmp)(const void*, const void*),
                        Key** base, size_t n);

   void createIcaoList();

   int rangeSort();     // Sort results by range; first compute range and then
                        // uses rangeSort2() to sort.

   int rangeSort2();    // Sort results by range; range must have already
                        // been computed.

   double range2(const double lat, const double lon) const;

   static int rlqs(const void* p1, const void* p2);
   static int ol_cmp(const void* p1, const void* p2);

   static void stripSpaces(char buff[], const int n);
   static void fillSpaces(char buff[], const int n);

   base::FileReader* db; // The database (loaders)
   long ncache;      // Number of keys alloced

   Key** rl;         // List of DAFIF records in the database
   long  nrl;        // Number of Records in rl

   Key** ol;         // List of DAFIF records in ICAO code order
   long  nol;        // Number of Records in ol

   Key** ql;         // query list -- results of query (usually sorted
                     //   by range)
   int   nql;        // Number of record found
   int   qlimit;     // Query limit (zero if no limit)

   double refLat;    // Ref point latitude
   double refLon;    // Ref point longitude
   double coslat;    // cos(ref point latitude)
   double mrng;      // max range (nm)

   bool dbInUse;     // Database In-Use flag
   bool dbLoaded;    // Database has been loader
};

}
}

#endif
