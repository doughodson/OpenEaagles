//------------------------------------------------------------------------------
// Class: Navaid
//------------------------------------------------------------------------------
#ifndef __oe_dafif_Navaid_H__
#define __oe_dafif_Navaid_H__

#include "Record.h"

namespace oe {
namespace dafif {

//------------------------------------------------------------------------------
// Class: Navaid
// Description: Access to the DAFIF NAVAID database.
//------------------------------------------------------------------------------
class Navaid : public Record {
    DECLARE_SUBCLASS(Navaid,Record)

public:
   enum { RECORD_LENGTH = NAVAID_RECORD_LEN };

   enum NavaidType { INVALID = -1, ANY = 'Z',
        VOR = '1', VORTAC = '2', TACAN = '3', VOR_DME = '4', NDB = '5',
        NDB_DME = '7', DME = '9'
   };

public:
   Navaid();
   Navaid(const char* const s);

   // Returns the NAVAID type.
   NavaidType navaidType() const;

   // Returns true if 'type' is equal to the NAVAID type.
   bool isNavaidType(const NavaidType type) const;

   // Returns true if the NAVAID is any DME type
   bool isDmeType() const;

   // Returns true if the NAVAID is a directional beacon type (e.g., VOR, VORTAC, etc)
   bool isDirectionalType() const;

   // Data access functions
   int   keyCode() const;           // Navaid Key Code field.
   float frequency() const;         // Frequency (MHz).
   int   channel() const;           // Channel number.
   char  channelType() const;       // Channel type.
   float power() const;             // Power field.
   void radioClass(char rc[]) const; // Returns the Radio Class field in 'rc'.
   float range() const;             // Range field.
   float slaveVariance() const;     // Slave Variance field.

   virtual void printRecord(std::ostream& sout) const override;
   virtual void printTrueBearingRange(std::ostream& sout, const double aclat, const double aclon, const double acelev) const override;

private:
   static const Ptbl ptable;

};


//------------------------------------------------------------------------------
// inline member functions
//------------------------------------------------------------------------------

// frequency: returns the value of the frequency field
inline float Navaid::frequency() const
{
   return dsFrequency( makePointer(NA_FREQUENCY_POS) );
}

// slaveVariance: returns the value of the slave variance field
inline float Navaid::slaveVariance() const
{
   return dsSlaveVariance( makePointer(NA_SLAVE_VAR_POS) );
}

// keyCode: returns the value of the NAVAID Key Code field
inline int Navaid::keyCode() const
{
   return dsAtoln( makePointer(NA_KEY_CODE_POS), NA_KEY_CODE_LEN );
}

// channel: returns the value of the channel field
inline int Navaid::channel() const
{
   return dsAtoln( makePointer(NA_CHANNEL_POS), NA_CHANNEL_LEN );
}

// channelType: returns the value of the channel type field
inline char Navaid::channelType() const
{
   return *makePointer(NA_CHANNEL_TYPE_POS);
}

// power: returns the value of the Power field.
inline float Navaid::power() const
{
   const char* p = makePointer(NA_POWER_POS);
   double w = dsAtofn(p, NA_POWER_LEN);

   if (w != 0.0)
      return static_cast<float>(w);

   if (*p == 'U')
      return 2000.0f;

   return 50.0f;
}

// range: returns the value of the Range field.
inline float Navaid::range() const
{
   double rng = 0.0;
   const char* p = makePointer(NA_RANGE_POS);
   if (*p != 'U') {
      rng = dsAtofn(p, NA_RANGE_LEN);
   }
   return static_cast<float>(rng);
}

// radioClass: returns the value of the radio class field
inline void Navaid::radioClass(char rc[]) const
{
   dsGetString( rc, makePointer(NA_RADIO_CLASS_POS), NA_RADIO_CLASS_LEN );
}

}
}

#endif
