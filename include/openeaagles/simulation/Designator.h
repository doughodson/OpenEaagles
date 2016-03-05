//------------------------------------------------------------------------------
// Class: Designator
//
// Description: General designator (e.g., laser)
//
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Designator_H__
#define __oe_simulation_Designator_H__

#include "openeaagles/base/Object.h"

namespace oe {
namespace simulation {

class Player;

class Designator : public base::Object
{
    DECLARE_SUBCLASS(Designator,base::Object)

public:
   // Since we're not using DIS Designator PDUs yet, use this to send
   // data over the network.
   struct Data {
      Data() : latitude(0), longitude(0), elevation(0), freq(0), power(0), code(0), padding(0) {}
      double      latitude;      // Latitude                         (degs)
      double      longitude;     // Longitude                        (degs)
      double      elevation;     // Elevation                        (m)
      LCreal      freq;          // Frequency                        (Hz)
      LCreal      power;         // Effective Radiated Power (ERP)   (Watts)
      unsigned short code;       // LASER code
      unsigned short padding;    // LASER code
   };

public:
   Designator();

   double getLatitude() const       { return data.latitude; }
   double getLongitude() const      { return data.longitude; }
   double getElevation() const      { return data.elevation; }
   LCreal getFrequency() const      { return data.freq; }
   LCreal getPower() const          { return data.power; }
   unsigned short getCode() const   { return data.code; }
   const Data* getDataBuffer() const   { return &data; }

   // set functions
   virtual void setPosition(const double lat, const double lon, const double elev) {
      data.latitude = lat;
      data.longitude = lon;
      data.elevation = elev;
   }
   virtual void setFrequency(const LCreal v)       { data.freq = v; }
   virtual void setPower(const LCreal v)           { data.power = v; }
   virtual void setCode(const unsigned short c)    { data.code = c; }

   virtual void setDataBuffer(const Data* const d) {
      if (d != 0) {
         setPosition( d->latitude, d->longitude, d->elevation );
         setFrequency( d->freq );
         setPower( d->power );
         setCode( d->code );
      }
   }

private:
   Data data;          // Designator data (see above)
};

} // End simulation namespace
} // End oe namespace

#endif
