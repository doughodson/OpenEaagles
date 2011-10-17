//------------------------------------------------------------------------------
// Class: WaypointLoader
// Base class:	DsDafifDatabase -> WaypointLoader
//
// Description: DAFIF Waypoint file class (file3)
//
//
// Public member functions:
//
//    [all public members from the base classes]
//
//    WaypointLoader()
//    WaypointLoader(const char* country)
//    WaypointLoader(const char* country, const char* file)
//    WaypointLoader(const char* country, const char* file, const char* path)
//       Constructors: Load only records with 'country' code from
//       'file' in directory 'path'.
//
//    DsWaypoint* waypoint(int n)
//       Returns the n'th waypoint loaded.
//       Range: 0 .. numberOfRecords()-1
//
//    DsWaypoint* getWaypoint(int n)
//       Get the n'th waypoint found by last query.
//       Range: 0 .. numberFound()-1
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dafif_WaypointLoader_H__
#define __Eaagles_Dafif_WaypointLoader_H__

#include "DafifDatabase.h"
#include "Waypoint.h"

namespace Eaagles {
namespace Dafif {

class WaypointLoader : public Database {
    DECLARE_SUBCLASS(WaypointLoader,Database)

public:
   WaypointLoader();
   WaypointLoader( const char* country,
      const char* file = "file3",
      const char* path = "/eaagles3rdParty/dafif/fullall/"
   );

   virtual Waypoint* waypoint(const int n);
   virtual Waypoint* getWaypoint(const int n);

   // DafifDatabase Interface
   virtual bool load(const char* code = 0);
   virtual int getRecordLength();
   virtual int getMaxRecords();
   virtual int queryByRange();
   virtual int queryByIdent(const char* id);
   virtual int queryByKey(const char* key);
   virtual void printLoaded(std::ostream& sout);
   virtual void printResults(std::ostream& sout);

protected:

   enum { WAYPOINT_MAX_RECORDS = 140000 };

   // Memory key used for quick Waypoint record lookup
   struct WaypointKey : public Database::Key {

      char   key[WP_KEY_LEN+1];           // DAFIF Key Code
      char   ident[WP_IDENT_LEN+1];       // Identifier
      char   countryCode[WP_CCODE_LEN+1]; // Country Code

      WaypointKey(const long idx, const Waypoint& waypoint);
      WaypointKey(const char* id, const char* country);
      WaypointKey(const char* key);

      ~WaypointKey();

      void serialize(std::ostream& sout) const;
   };

   static int il_cmp(const void* p1, const void* p2);
   static int kl_cmp(const void* p1, const void* p2);

};

} // End Dafif namespace
} // End Eaagles namespace

#endif
