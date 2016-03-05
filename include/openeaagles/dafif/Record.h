//------------------------------------------------------------------------------
// Class: Record
//
// Description: General class used to access DAFIF record information
//
//
// Public Member Functions:
//
//    Record()
//    Record(const char* string)
//    Record(Record& dafif)
//       Constructors: (creates a copy of 'string')
//
//    setRecord(const char* string)
//       Sets this object's record string to 'string' (does not make a
//       copy of 'string').
//
//    printData(std::ostream& sout) const
//       Prints information from the DAFIF record.
//
//    int formatCode() const
//       Returns the value of the Format Code field.
//
//    int isFormatCode(const int code) const
//       Returns true if 'code' is equal to the Format Code field.
//
//    char transactionCode() const
//       Returns the character contained in the Transaction Code field.
//
//    int isTransactionCode(char c) const
//       Returns true if 'code' is equal to the Transaction Code field.
//
//    icaoCode(char code[]) const
//       Returns the value of the ICAO field in 'code'.
//
//    int isIcaoCode(const char code[]) const
//       Returns true if 'code' is equal to the ICAO Code field.
//
//    key(char key1[]) const
//       Returns the value of the record's key fields in 'key1'.
//
//    int isKey(const char key1[]) const
//       Returns true if 'key1' is equal to the record's key fields.
//
//    ident(char id[]) const
//       Returns the value of the record's identifier field in 'id'.
//
//    int isIdent(const char id[]) const
//       Returns true if 'id' is equal to the record's identifier field.
//
//    name(char name1[]) const
//       Returns the value of the name field in 'name1'.
//
//    int isName(const char name1[]) const
//       Returns true if 'name1' is equal to the Name field.
//
//    dafifType(char type[]) const
//       Returns the value of the record's Type field in 'type'.
//
//    int isDafifType(const char type[]) const
//       Returns true if 'type' is equal to the record's Type field.
//
//    countryCode(char country[]) const
//       Returns the value of the Country Code field in 'country'.
//
//    int isCountryCode(const char country[]) const
//       Returns true if 'code' is equal to the Country Code field.
//
//    int stateCode() const
//       Returns the value of the State/Province Code field.
//
//    int isStateCode(int code) const
//       Returns true if 'code' is equal to the State/Province Code field.
//
//    wac(char code[]) const
//       Returns the value of the WAC field in 'code'.
//
//    int isWac(const char code[]) const
//       Returns true if 'code' is equal to the WAC Code field.
//
//    int dafifRecordNumber() const
//       Returns the value of the Record Number field.
//
//    int isDafifRecordNumber(int num) const
//
//    cycleDate(char date[]) const
//       Returns the value of the Cycle Date field in 'date'.
//
//    int isCycleDate(const char date[]) const
//
//    double latitude() const
//    double longitude() const
//    float elevation() const
//    float magVariance() const
//       Return the values of the latitude, longitude, elevation,
//       and magnetic variance fields.
//
//
//
// Some Member Variables:
//
//    protected: static const Ptbl ptable
//       Class field position table.  Used to locate many of the common
//       DAFIF fields.
//
//    protected: const Ptbl* ptbl
//       Object's field position table.  The Record constructor sets
//       this variable to 0.  Constructors of the derived classes must
//       set this variable to their own <class>::ptable.
//
//------------------------------------------------------------------------------
#ifndef __oe_dafif_Record_H__
#define __oe_dafif_Record_H__

#include "openeaagles/base/String.h"
#include "dafifc.h"

namespace oe {
namespace dafif {

class Record : public base::String {
    DECLARE_SUBCLASS(Record,base::String)

public:
   // record types
    enum Type { INVALID = -1, ANY = 0,
        NAVAID = 2, WAYPOINT = 3,
        AIRPORT = 10, RUNWAY = 11, ILS = 12
    };

public:
   Record();
   Record(const char* const s);

   void setRecord(const char* const s);
   void setRecord(base::String* const s);

   virtual void printRecord(std::ostream&) const    {}

   int formatCode() const;
   bool isFormatCode(const int code) const;

   char transactionCode() const;
   bool isTransactionCode(const char c) const;

   void icaoCode(char code[]) const;
   bool isIcaoCode(const char code[]) const;

   void key(char k[]) const;
   bool isKey(const char k[]) const;

   virtual void ident(char id[]) const;
   bool isIdent(const char id[]) const;

   void name(char n[]) const;
   bool isName(const char name[]) const;

   void dafifType(char type[]) const;
   bool isDafifType(const char type[]) const;

   void countryCode(char country[]) const;
   bool isCountryCode(const char country[]) const;

   int stateCode() const;
   bool isStateCode(const int code) const;

   void wac(char code[]) const;
   bool isWac(const char code[]) const;

   double latitude() const;
   double longitude() const;
   float  elevation() const;
   float  magVariance() const;

   int dafifRecordNumber() const;
   bool isDafifRecordNumber(const int num) const;

   void cycleDate(char date[]) const;
   bool isCycleDate(const char date[]) const;

   virtual void printTrueBearingRange(std::ostream& sout, const double aclat, const double aclon, const double acelev) const;

   // ---
   // Common conversion and support functions
   // ---
   static float  dsChan2freq(const int chan, const char type);
   static int    dsFreq2chan(const float freq);
   static double dsAtofn(const char* const s, const int n);
   static long   dsAtoln(const char* const s, const int n);
   static void   dsGetString(char* const d, const char* const s, const int n);
   static bool   dsIsString(const char* const s, const char* const refStr);
   static double dsLatitude(const char* const p);     // convert string to latitude
   static double dsLongitude(const char* const p);    // convert string to longitude
   static float  dsElevation(const char* const p);    // format#1: convert string to elevation (to nearest foot)
   static float  dsElevation1(const char* const p);   // format#2: convert string to elevation (to tenths of feet)
   static float  dsMagVariance(const char* const p);  // convert string to magnetic variation
   static float  dsSlaveVariance(const char* const p); // convert string to slave magnetic variation
   static float  dsMagHeading(const char* const p);   // convert string to magnetic heading
   static float  dsFrequency(const char* const p);    // convert string to frequency  (MHz)


protected:
   const char* makePointer(const size_t offset) const;
   void resetData();

   // ---
   // Position Table
   // ---
   struct Ptbl {
      int key;             // Offset to key
      int keyLength;       // Length of the key field
      int ident;           // Offset to ident
      int identLength;     // Length of the ident field
      int name;            // Offset to name
      int nameLength;      // Length of the name field
      int type;            // Offset to type
      int typeLength;      // Length of the type field
      int countryCode;     // Offset to country code
      int countryLength;   // Length of the country code field
      int stateCode;       // State/Province Code
      int stateLength;     // Length of State/Province code field
      int wac;             // World Area Code
      int wacLength;       // Length of WAC field
      int latitude;        // Offset to latitude
      int longitude;       // Offset to longitude
      int elevation;       // Offset to elevation
      int magVariance;     // Offset to mag var
   };

   const Ptbl* ptbl;    // Position table; one for each instance
};

} // End dafif namespace
} // End oe namespace

#endif
