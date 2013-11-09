//------------------------------------------------------------------------------
// Class: Runway
// Base class:	Basic::Object -> Basic::String -> Record -> Runway
//
// Description: Access to the DAFIF Airport/Runway records.
//
//
// Public member functions:
//
//  [all public members from the base classes]
//
//  double latitude(WhichEnd whichEnd)
//  double longitude(WhichEnd whichEnd)
//  float elevation(WhichEnd whichEnd)
//  float slope(WhichEnd whichEnd)
//  float magHeading(WhichEnd whichEnd)
//      Return the values of the latitude, longitude, elevation, slope
//      and magnetic heading fields for 'whichEnd' of the runway.
//      WhichEnd is Runway::HIGH_END or Runway::LOW_END.
//
//  ident(char id[], WhichEnd whichEnd) 
//      Returns the value of the runway's (end) identifier field in 'id'.
//
//  int isIdent(char id[], WhichEnd whichEnd) 
//      Returns true if 'id' is equal to the runway's (end) id field.
//
//  int width() 
//      Returns the width of the runway (feet).
//
//  int length() 
//      Returns the length of the runway (feet).
//
//  void airportKey(char apKey[])
//      Returns the record key of the airport.
//
//  WhichEnd whichEnd(char rwEndId[])
//      Returns which end of the runway matches the runway end id.
//
//  Runway::recordLength
//       Length of a DAFIF Runway record pair.
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dafif_Runway_H__
#define __Eaagles_Dafif_Runway_H__

#include "Record.h"

namespace Eaagles {
namespace Dafif {

class Runway : public Record {
    DECLARE_SUBCLASS(Runway,Record)

public:
   enum { RECORD_LENGTH = RUNWAY_RECORD_LEN };

   enum WhichEnd { LOW_END = 0, HIGH_END = 1 };

public:
   Runway();
   Runway(const char* const s);

   double latitude(const WhichEnd whichEnd) const;
   double longitude(const WhichEnd whichEnd) const;
   float elevation(const WhichEnd whichEnd) const;
   float magHeading(const WhichEnd whichEnd) const;
   float slope(const WhichEnd whichEnd) const;
   
   void  ident(char ident[], const WhichEnd whichEnd) const;
   int isIdent(const char id[], const WhichEnd whichEnd) const;

   int width() const;
   int length() const;
   void airportKey(char apKey[]) const;
   void getRunwayMagHeading(const double aclat, const double aclon, const double acelev, float* magHeading1, float* magHeading2, double* trueBearing1, double* trueBearing2) const;
   void printRunwayMagHeading(std::ostream& sout, const double aclat, const double aclon, const double acelev) const;

   WhichEnd whichEnd(const char rwEndId[]) const;

   // Record Interface
   virtual void printRecord(std::ostream& sout) const;

private:
   static const Ptbl ptable;

};


//------------------------------------------------------------------------------
// inline member functions
//------------------------------------------------------------------------------


// ident: returns the runway identifier field
inline void Runway::ident(char ident[], const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      dsGetString(ident, makePointer(RW_LE_IDENT_POS), RW_XE_IDENT_LEN);
   else
      dsGetString(ident, makePointer(RW_HE_IDENT_POS), RW_XE_IDENT_LEN);
}

// isIdent: returns true if id runway the identifier field
inline int Runway::isIdent(const char id[], const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsIsString( makePointer(RW_LE_IDENT_POS), id );
   else
      return dsIsString( makePointer(RW_HE_IDENT_POS), id );
}


// latitude: returns the value of the runway end latitude field
inline double Runway::latitude(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsLatitude( makePointer(RW_LE_LATITUDE_POS) );
   else
      return dsLatitude( makePointer(RW_HE_LATITUDE_POS) );
}


// longitude: returns the value of the runway end longitude field
inline double Runway::longitude(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsLongitude( makePointer(RW_LE_LONGITUDE_POS) );
   else
      return dsLongitude( makePointer(RW_HE_LONGITUDE_POS) );
}


// elevation: returns the value of the runway end elevation field
inline float Runway::elevation(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsElevation1( makePointer(RW_LE_ELEVATION_POS) );
   else
      return dsElevation1( makePointer(RW_HE_ELEVATION_POS) );
}


// slope: returns the value of the runway end slope field
inline float Runway::slope(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return (float) dsAtoln( makePointer(RW_LE_SLOPE_POS), RW_XE_SLOPE_LEN );
   else
      return (float) dsAtoln( makePointer(RW_HE_SLOPE_POS), RW_XE_SLOPE_LEN );
}


// magHeading: returns the value of the runway end mag heading field
inline float Runway::magHeading(const WhichEnd whichEnd) const
{
   if (whichEnd == LOW_END)
      return dsMagHeading( makePointer(RW_LE_MAGHDG_POS) );
   else
      return dsMagHeading( makePointer(RW_HE_MAGHDG_POS) );
}


// width: returns the value of the runway width field
inline int Runway::width() const
{
   return dsAtoln( makePointer(RW_WIDTH_POS), RW_WIDTH_LEN );
}


// length: returns the value of the runway length field
inline int Runway::length() const
{
   return dsAtoln( makePointer(RW_LENGTH_POS), RW_LENGTH_LEN );
}


// airportKey: returns the value of the airport identifier
inline void Runway::airportKey(char apKey[]) const
{
   dsGetString( apKey, makePointer(RW_APKEY_POS), AP_KEY_LEN );
}


// whichEnd: which end of the runway are we approaching?
inline Runway::WhichEnd Runway::whichEnd(const char rwEndId[]) const
{
   Runway::WhichEnd we = Runway::HIGH_END;
   if ( !isIdent(rwEndId,we) ) we = Runway::LOW_END;
   return we;
}

} // End Dafif namespace
} // End Eaagles namespace

#endif
