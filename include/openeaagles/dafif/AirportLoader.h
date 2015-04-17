//------------------------------------------------------------------------------
// Class: AirportLoader
// Base class: Database -> AirportLoader
//
// Description: DAFIF Airport file class (file0)
//
// Note(s):
//
//    Standard record length is set to airport format records (1), so after
//    an airport query these records can be retrieved using getRecord(i).
//    However, after a runway query, use getRecord(n, RUNWAY_RECORD_LEN),
//    and after an ils query, use getRecord(n,ILS_RECORD_LEN).
//
//
//
// Public members:
//
//    [all public members from the base classes]
//
//    AirportLoader()
//    AirportLoader(const char* country)
//    AirportLoader(const char* country, const char* file)
//    AirportLoader(const char* country, const char* file, const char* path)
//       Constructors: Load only records with 'country' code from
//       'file' in directory 'path'.
//
//    int queryByLength(float minRwLen)
//       Find all records within the search area with a runway length
//       of at least 'minRwLen' feet.
//
//    Airport* airport(int n)
//       Returns the n'th airport loaded.
//       Range: 0 .. numberOfRecords()-1
//
//    Airport* getAirport(int n)
//       Get the n'th airport found by last query.
//       Range: 0 .. numberFound()-1
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dafif_AirportLoader_H__
#define __Eaagles_Dafif_AirportLoader_H__

#include "Database.h"
#include "Airport.h"
#include "Runway.h"
#include "Ils.h"

#ifdef ALT_ILS_FILE
#include "AIParser.h"
#endif

namespace Eaagles {
namespace Dafif {

class AirportLoader : public Database {
    DECLARE_SUBCLASS(AirportLoader,Database)

public:
   AirportLoader();
   AirportLoader(
      const char* country,
      const char* file = "file0",
      const char* path = "/eaagles3rdParty/dafif/fullall/"
   );

   virtual Airport* airport(const int n);
   virtual Airport* getAirport(const int n);
   virtual Runway* getRunway(const int n);
   virtual Ils* getIls(const int n);

   virtual int queryByLength(const float minRwLen);
   virtual int queryByType(const Airport::AirportType type);
   virtual int queryByFreq(const float freq);
   virtual int queryByChannel(const int chan);

   virtual int getNumRunwayRecords(const char* key);
   virtual int queryRunwayByNumber(const char* key, const int n);
   virtual int queryRunwayByIdent(const char* id);
   virtual int queryRunwayBySubkey(const char* subkey);
   virtual int queryRunwayByRange();
   virtual int queryRunwayByLength(const float length);
   virtual int queryRunwayByFreq(const float freq);
   virtual int queryRunwayByChannel(const int chan);

   virtual int getNumIlsRecords(const char* key);
   virtual int queryIlsByNumber(const char* key, const int n);
   virtual int queryIlsByIdent(const char* id);
   virtual int queryIlsBySubkey(const char* subkey);
   virtual int queryIlsByRange();
   virtual int queryIlsByType(const Ils::IlsType type);
   virtual int queryIlsByFreq(const float freq);
   virtual int queryIlsByChannel(const int chan);

   struct AirportKey;
   struct RunwayKey;

   bool load(const char* code = 0) override;
   int getRecordLength() override;
   int getMaxRecords() override;
   int queryByRange() override;
   int queryByIdent(const char* id) override;
   int queryByKey(const char* key) override;
   void printLoaded(std::ostream& sout) override;
   void printResults(std::ostream& sout) override;

   // Key used for quick ILS record lookup
   struct IlsKey : public Database::Key {

      RunwayKey* parent;
      char   key[ILS_KEY_LEN+1];    // DAFIF Key Code
      char   id[ILS_IDENT_LEN+1];   // DAFIF Identifier
      IlsKey* next;                 // Next ILS key in list
      float  freq;                  // ILS component frequency (MHz)
      int    chan;                  // ILS component channel#
      Ils::IlsType type;            // ILS component type

#ifdef ALT_ILS_FILE
      float  gs;                    // ILS glide slope
      float  magvar;                // magvar
      int    loc;                   // position of antenna
      int    elev;                  // elevation
      char   name[ILS_NAME_LEN+1];  // ILS name

      IlsKey(const long idx);
#endif

      IlsKey(const long idx, const Ils& ils);
      IlsKey(const char* key);

      ~IlsKey();

      void serialize(std::ostream& sout) const;
   };

   // Key used for quick Runway record lookup
   struct RunwayKey : public Database::Key {

      AirportKey* parent;
      char key[RW_KEY_LEN+1]; // DAFIF Key Code
      RunwayKey* next;        // Next runway key in list
      IlsKey* ils;            // List of ILS records
      int rwlen;              // Runway length

      RunwayKey(const long idx, const Runway& runway);
      RunwayKey(const char* key);

      ~RunwayKey();

      void serialize(std::ostream& sout) const;
   };

   // Key used for quick Airport record lookup
   struct AirportKey : public Database::Key {

      char   key[AP_KEY_LEN+1];     // DAFIF Key Code
      RunwayKey* runways;           // List of runways for this AP
      AirportKey* next;             // Next Airport key in list
      Airport::AirportType type;  // Airport component type

      AirportKey(const long idx, const Airport& airport);
      AirportKey(const char* key);

      ~AirportKey();

      void serialize(std::ostream& sout) const;
   };

#ifdef ALT_ILS_FILE    /* Alternate ILS File */
   const char* createIlsRecord(const Key* key);
#endif

protected:

   enum { AIRPORT_MAX_RECORDS = 40000 };

   int queryAirport(const Airport::AirportType type, const float minRwLen);

   int chkRwLen(const AirportKey* key, const float minRwLen);

   int chkIlsFreq(const AirportKey* key, const float freq);
   int chkRwIlsFreq(const RunwayKey* key, const float freq);

   int chkIlsChan(const AirportKey* key, const int chan);
   int chkRwIlsChan(const RunwayKey* key, const int chan);

   void makeSimpleLinkedList();

   void findGlideSlope(const RunwayKey* rwKey, const IlsKey* ilsKey);

   static int kl_cmp(const void* p1, const void* p2);

   AirportKey* firstAirport;  // first airport in linked-list

private:

#ifdef ALT_ILS_FILE    /* Alternate ILS File */
   char altIlsFile[256];
   char irec[ILS_RECORD_LEN+1];
#endif

};

} // End Dafif namespace
} // End Eaagles namespace

#endif
