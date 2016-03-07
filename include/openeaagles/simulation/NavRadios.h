//------------------------------------------------------------------------------
// Classes: NavRadio, TacanRadio, IlsRadio
//
// The NAV radios are being updated and are not current fully functional!!!
//------------------------------------------------------------------------------
#ifndef __oe_simulation_NavRadio_H__
#define __oe_simulation_NavRadio_H__

#include "openeaagles/simulation/Radio.h"

namespace oe {
namespace dafif { class AirportLoader; class NavaidLoader; }

namespace simulation {

//------------------------------------------------------------------------------
// Class: NavRadio
// Description: General class for all navigation radio models
// Factory name: NavRadio
//------------------------------------------------------------------------------
class NavRadio : public Radio
{
    DECLARE_SUBCLASS(NavRadio, Radio)

public:
   NavRadio();

protected:
   double getLatitude() const;
   double getLongitude() const;
   double getAltitude() const;

   dafif::NavaidLoader* getNavaidLoader();
   dafif::AirportLoader* getAirportLoader();

   // Slave our position to our ownship
   virtual bool setPosition();

   bool shutdownNotification() override;

private:
   void initData();

   base::safe_ptr<dafif::AirportLoader> apdb; // Pointer to Airport loader
   base::safe_ptr<dafif::NavaidLoader> nvdb;  // Pointer to Navaid loader

   double latitude;                 // Ownship Latitude
   double longitude;                // Ownship Longitude
   double altitude;                 // Ownship altitude HAE (ft)

};

//------------------------------------------------------------------------------
// Class: TacanRadio
// Description: TACAN navigation radio model
// Factory name: TacanRadio
//------------------------------------------------------------------------------
class TacanRadio : public NavRadio
{
   DECLARE_SUBCLASS(TacanRadio,NavRadio)

public:
   // bands
   enum Band { TCN_X_BAND = 0, TCN_Y_BAND };

   // power modes
   enum {
      PWR_TCN_OFF = BaseClass::PWR_OFF,
      PWR_TCN_REC,          // REC mode
      PWR_TCN_TRAN_REC,     // T/R mode
      PWR_TCN_AA_TRAN_REC,  // AA TR mode
   };

public:
   TacanRadio();

   Band getBand() const;
   virtual bool setBand(const Band x);

   double getRange() const;       // Range (nautical miles)
   double getBearing() const;     // Bearing (degrees)

   void updateData(const double dt = 0.0) override;

protected:
   // Get Range and Bearing - Input pointers for your outputs
   bool computeRangeBearing(bool* rngIsValid, double* range, double* grdrange, bool* bearingIsValid, double* bearing);

private:
   void initData();

   Band band;

   bool rangeIsValid;
   bool bearingIsValid;

   double range;
   double grdrange;
   double bearing;
   double destLatitude;
   double destLongitude;
   double currentMagVar;
};


//------------------------------------------------------------------------------
// Class: IlsRadio
// Description: ILS navigation radio model
// Factory name: IlsRadio
//------------------------------------------------------------------------------
class IlsRadio : public NavRadio
{
   DECLARE_SUBCLASS(IlsRadio,NavRadio)

public:
   // constants for figuring degrees to dots
   static const double GS_DEG_PER_DOT;
   static const double LOC_DEG_PER_DOT;

public:
   IlsRadio();

   //Outside Access FCNs
   double getGlideslopeDifference();
   double getLocalizerDifference();
   double getGlideslopeDifferenceDots();
   double getLocalizerDifferenceDots();

   void updateData(const double dt = 0.0) override;

protected:
   //Always find the glideslope first!!!!
   virtual bool findILSGlideslopeByFreq(double freq);
   virtual bool findLocalizerByFreq(double freq);

private:
   int timerCounter;              // For Only doing calc every so often
   bool localizerValid;
   bool glideSlopeValid;
   double range;                  // slant range to the end of the runway
   double grdrange;               // ground range to the end of the RWY
   double bearing;                // bearing of the end of the RWY
   double destLatitude;           // lat for end of RWY
   double destLongitude;          // lon for end of RWY
   double deltaGlideSlope;
   double deltaLocalizerBearing;
   double currentMagVar;          // mag var at glideslope emitter
   double acGlideSlope;
   double acLocalizerBearing;
   double ilsGlideSlope;
   double ilsLocalizerBearing;
};

} // End simulation namespace
} // End oe namespace

#endif
