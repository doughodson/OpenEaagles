
#ifndef __oe_dafif_WaypointLoader_H__
#define __oe_dafif_WaypointLoader_H__

#include "Database.hpp"
#include "Waypoint.hpp"

namespace oe {
namespace dafif {

//------------------------------------------------------------------------------
// Class: WaypointLoader
// Base class: Database -> WaypointLoader
//
// Description: DAFIF Waypoint file class (file3)
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
class WaypointLoader : public Database
{
   DECLARE_SUBCLASS(WaypointLoader,Database)

public:
   WaypointLoader();
   WaypointLoader( const char* country,
      const char* file = "file3",
      const char* path = "/data/dafif/fullall/"
   );

   virtual Waypoint* waypoint(const int n);
   virtual Waypoint* getWaypoint(const int n);

   virtual bool load(const char* code = nullptr) override;
   virtual int getRecordLength() override;
   virtual int getMaxRecords() override;
   virtual int queryByRange() override;
   virtual int queryByIdent(const char* id) override;
   virtual int queryByKey(const char* key) override;
   virtual void printLoaded(std::ostream& sout) override;
   virtual void printResults(std::ostream& sout) override;

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

}
}

#endif
