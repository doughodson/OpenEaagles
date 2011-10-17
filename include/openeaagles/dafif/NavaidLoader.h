//------------------------------------------------------------------------------
// Class: NavaidLoader
// Base class:	Database -> NavaidLoader
//
// Description: DAFIF NAVAID file class (file2)
//
//
// Public member functions:
//
//    [all public members from the base classes]
//
//    NavaidLoader()
//    NavaidLoader(const char* country)
//    NavaidLoader(const char* country, const char* file)
//    NavaidLoader(const char* country, const char* file, const char* path)
//       Constructors: Load only records with 'country' code from
//       'file' in directory 'path'.
//
//    int queryByType(Navaid::NavaidType type)
//       Find 'type' NAVAID records within the search area.
//
//    int queryByFreq(const float freq)
//       Find all records within the search area with a given frequency.
//
//    int queryByChannel(const long chan)
//    int queryByChannel(const long chan, const char band)
//       Find all records within the search area with a given channel
//       number.
//
//    Navaid* navaid(const int n)
//       Returns the n'th NAVAID loaded.
//       Range: 0 .. numberOfRecords()-1
//
//    Navaid* getNavaid(const int n)
//       Get the n'th NAVAID found by last query.
//       Range: 0 .. numberFound()-1
//
//    printFreqList()
//       Prints the records loaded in frequency order.
//
//    printChannelList()
//       Prints the records loaded in channel number order.
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dafif_NavaidLoader_H__
#define __Eaagles_Dafif_NavaidLoader_H__

#include "DafifDatabase.h"
#include "Navaid.h"

namespace Eaagles {
namespace Dafif {

class NavaidLoader : public Database {
    DECLARE_SUBCLASS(NavaidLoader,Database)

public:
   NavaidLoader();
   NavaidLoader( const char* country,
      const char* file = "file2",
      const char* path = "/eaagles3rdParty/dafif/fullall/"
   );
   
   virtual Navaid* navaid(const int n);
   virtual Navaid* getNavaid(const int n);

   virtual int queryByType(const Navaid::NavaidType type);
   virtual int queryByFreq(const float freq);
   virtual int queryByChannel(const long chan, const char band = 'X');

   virtual void printFreqList(std::ostream& sout);
   virtual void printChannelList(std::ostream& sout);
   
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
   enum { NAVAID_MAX_RECORDS = 80000 };

   // Memory key used for quick Navaid record lookup
   struct NavaidKey : public Database::Key {

      float  freq;         // Frequency
      int    channel;      // TACAN channel
                           //   (+channel ==> X band)
                           //   (-channel ==> Y band)
      char   key[NA_KEY_LEN+1];           // DAFIF Key
      char   ident[NA_IDENT_LEN+1];       // Identifier
      char   countryCode[NA_CCODE_LEN+1]; // Country Code
      Navaid::NavaidType type;          // Type

      NavaidKey(const long idx, const Navaid& navaid);
      NavaidKey(const char* key);
      NavaidKey(const char* id, const char* country);
      NavaidKey(const float freq);
      NavaidKey(const long chan);

      ~NavaidKey();

      void serialize(std::ostream& sout) const;
   };

   static int il_cmp(const void* p1, const void* p2);
   static int kl_cmp(const void* p1, const void* p2);
   static int fl_cmp(const void* p1, const void* p2);
   static int cl_cmp(const void* p1, const void* p2);

private:
   NavaidKey** fl;   // List of DAFIF records in frequency order
   long  nfl;        // Number of Records in fl

   NavaidKey** cl;   // List of DAFIF records in channel order
   long  ncl;        // Number of Records in cl

};

} // End Dafif namespace
} // End Eaagles namespace

#endif
