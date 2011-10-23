//------------------------------------------------------------------------------
// Class: Ils
// Base class: Basic::Object -> Basic::String -> Record -> Ils
//
// Description: Access to the DAFIF ILS records.
//
//
// Public member functions:
//
//	[all public members from the base classes]
//
//	Ils::IlsType ilsType() const
//	    Returns the ils type.
//
//	int isIlsType(Ils::IlsType type) const
//	    Returns true if 'type' is equal to the ils type.
//
//	float frequency() const
//		Returns the frequency (MHz).
//
//	int channel() const
//		Returns the channel number.
//
//	float glideSlopeAngle() const
//		Returns the value of the Glide Slope Angle field.
//
//	airportKey(char apKey[]) const
//		Gets the record key of the airport.
//
//	runwayIdent(char rwId[]) const
//		Gets the runway identifier.
//
//	runwayEndIdent(char rwEndId[]) const
//		Gets the runway end identifier.
//
//	Ils::recordLength
//		Length of a DAFIF ILS record pair.
//
//------------------------------------------------------------------------------
#ifndef __Eaagles_Dafif_Ils_H__
#define __Eaagles_Dafif_Ils_H__

#include "Record.h"

namespace Eaagles {
namespace Dafif {

class Ils : public Record {

    DECLARE_SUBCLASS(Ils,Record)

public:

   Ils();
   Ils(const char* const s);

   enum { RECORD_LENGTH = ILS_RECORD_LEN };

   enum IlsType { INVALID = -1, ANY = 'Y',
		  LOCATOR = 'L', DME = 'D', LOCALIZER = 'Z', GLIDESLOPE = 'G',
		  BACKCOURSE_MARKER = 'B', INNER_MARKER = 'I', MIDDLE_MARKER = 'M',
	 	  OUTER_MARKER = 'O', UNKNOWN = 'U'
		  };

   virtual IlsType ilsType() const;
   virtual int isIlsType(const IlsType type) const;

   float frequency() const;
   int   channel() const;
   float glideSlopeAngle() const;
   void  airportKey(char apKey[]) const;
   void  runwayIdent(char rwId[]) const;
   void  runwayEndIdent(char rwEndId[]) const;
   void  getGlideSlopeData(const double aclat, const double aclon, const double acelev, float* ilsGlideSlope, float* acGlideSlope, float* deltaGlideSlope) const;
   void  printGlideSlopeData(std::ostream& sout, const double aclat, const double aclon, const double acelev) const;


   // Record Interface
   virtual void printRecord(std::ostream& sout) const;

private:
   static const Ptbl ptable;

};


//------------------------------------------------------------------------------
// inline member functions
//------------------------------------------------------------------------------

// frequency: returns the value of the frequency field
inline float Ils::frequency() const
{
   return dsFrequency( makePointer(ILS_FREQUENCY_POS) );
}

// channel: returns the value of the channel field
inline int Ils::channel() const
{
   return dsAtoln( makePointer(ILS_CHANNEL_POS), ILS_CHANNEL_LEN );
}


inline float Ils::glideSlopeAngle() const
{
   return (float) (dsAtofn( makePointer(ILS_GSA_POS), ILS_GSA_LEN) / 100.0);
}


// airportKey: returns the value of the airport identifier
inline void Ils::airportKey(char apKey[]) const
{
   dsGetString( apKey, makePointer(ILS_APKEY_POS), AP_KEY_LEN );
}


// runwayIdent: returns the value of the runway identifier
inline void Ils::runwayIdent(char rwKey[]) const
{
   dsGetString( rwKey, makePointer(ILS_RW_ID_POS), ILS_RW_ID_LEN );
}


// runwayEndIdent: returns the value of the runway end identifier
inline void Ils::runwayEndIdent(char rwEndKey[]) const
{
   dsGetString( rwEndKey,
		makePointer(ILS_RW_ID_POS+AP_KEY_LEN), RW_XE_IDENT_LEN );
}

} // End Dafif namespace
} // End Eaagles namespace

#endif
