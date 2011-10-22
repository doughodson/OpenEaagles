
#include "openeaagles/simulation/NavRadios.h"

#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class: NavRadio
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(NavRadio,"NavRadio")
EMPTY_SERIALIZER(NavRadio)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
NavRadio::NavRadio()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void NavRadio::initData()
{
   apdb           = 0;
   nvdb           = 0;

   latitude       = 0;
   longitude      = 0;
   altitude       = 0;
}


//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void NavRadio::copyData(const NavRadio& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   latitude = org.latitude;
   longitude = org.longitude;
   altitude = org.altitude;

   apdb = 0;
   nvdb = 0;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void NavRadio::deleteData()
{
   apdb = 0;
   nvdb = 0;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool NavRadio::shutdownNotification()
{
   apdb = 0;
   nvdb = 0;

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

double NavRadio::getLatitude() const
{
   return latitude;
   }

double NavRadio::getLongitude() const
{
   return longitude;
}

double NavRadio::getAltitude() const
{
   return altitude;
}

Dafif::NavaidLoader* NavRadio::getNavaidLoader()
{
   // If we don't have a NAVAID loader, try to get one from our simulation
   if (nvdb == 0) {
      Simulation* sim = getSimulation();
      if (sim != 0) {
         Dafif::NavaidLoader* p = sim->getNavaids();
         if (p != 0 && p->isDbLoader()) {
            nvdb = p;
            }
         }
      }

   return nvdb;
   }

Dafif::AirportLoader* NavRadio::getAirportLoader()
{
   // If we don't have an airport loader, try to get one from our simulation
   if (apdb == 0) {
      Simulation* sim = getSimulation();
      if (sim != 0) {
         Dafif::AirportLoader* p = sim->getAirports();
         if (p != 0 && p->isDbLoader()) {
            apdb = p;
         }
            }
         }

   return apdb;
      }

//------------------------------------------------------------------------------
// set the position to that of our ownship vehicle
//------------------------------------------------------------------------------
bool NavRadio::setPosition()
{
   bool ok = false;

   const Player* p = getOwnship();
   if (p != 0) {
      latitude = p->getLatitude();
      longitude = p->getLongitude();
      altitude = p->getAltitudeFt();
      ok = true;
   }

   return ok;
}

//==============================================================================
// Class: TacanRadio
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(TacanRadio,"TacanRadio")
EMPTY_SERIALIZER(TacanRadio)

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
TacanRadio::TacanRadio()
{
     STANDARD_CONSTRUCTOR()

   initData();
}

void TacanRadio::initData()
{
     setMaxDetectRange(120.0);

   setNumberOfChannels(126);

     rangeIsValid = false;
     bearingIsValid = false;
     range = 0;
     grdrange = 0;
     bearing = 0; 
     destLatitude = 0;
     destLongitude = 0;
     currentMagVar = 0;
     band = TCN_X_BAND;

   // Set frequencies
   {
      unsigned short chan = 1;

      // channels [ 1 .. 16 ]
      while (chan <= 16) {
         setChannelFrequency(chan++, 0.0f);
}

      // channels [ 17 .. 59 ]
      while (chan < 59) {
         setChannelFrequency(chan++, (LCreal(chan) * 0.1f + 106.3f));
      }

      // channels [ 60 .. 69 ]
      while (chan <= 69) {
         setChannelFrequency(chan++, 0.0f);
      }

      // channels [ 70 .. 126 ]
      while (chan <= 126) {
         setChannelFrequency(chan++, (LCreal(chan) * 0.1f + 107.3f) );
      }
   }
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void TacanRadio::copyData(const TacanRadio& org, const bool cc)
{
     BaseClass::copyData(org);
   if (cc) initData();

     rangeIsValid = org.rangeIsValid;
     bearingIsValid = org.bearingIsValid;
     range = org.range;
     grdrange = org.grdrange;
     bearing = org.bearing; 
     destLatitude = org.destLatitude;
     destLongitude = org.destLongitude;
     currentMagVar = org.currentMagVar;
     band = org.band;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void TacanRadio::deleteData()
{
}    

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void TacanRadio::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);

    //Must have ownship
    if (getOwnship() == 0) return;
    
   // Set our position to that of our ownship vehicle
   setPosition();                

   // 

        //Get Range and Bearing...
   //bool rangeBearingTestResult = getRangeBearing(&rangeIsValid,&range,&grdrange,&bearingIsValid,&bearing);
    }

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

TacanRadio::Band TacanRadio::getBand() const
{
   return band;
    }

double TacanRadio::getRange() const
{
    return range; 
};

double TacanRadio::getBearing() const
{
    return bearing;
};

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setBand() - set the X or Y band
bool TacanRadio::setBand(const Band x)
{
   band = x;
    return true;
}


//------------------------------------------------------------------------------
// computeRangeBearing
//------------------------------------------------------------------------------
bool TacanRadio::computeRangeBearing(bool *rngIsValid, double *range, double *grdrange, bool *bearingIsValid, double *bearing)
{
   bool ok = false;

   //*rngIsValid = false;
   //*range = 0.0;
   //*grdrange = 0.0;
   //*bearingIsValid = false;
   //*bearing = 0.0;

   //if (getNavaidLoader() != 0 ) {
   //   if ( getNavaidLoader()->requestDbInUse() ) {
   //      getNavaidLoader()->setArea(getLatitude(), getLongitude(), getMaxDetectRange());

   //      *rngIsValid = false;
   //      *range = 0;
   //      *bearingIsValid = false;
   //      *bearing = 0.0;

   //      int found = getNavaidLoader()->queryByChannel(getChannel());

   //      if (found > 0) {
   //         int i = 0;

   //         Dafif::Navaid* nav = getNavaidLoader()->getNavaid(i);

   //         if (nav != 0) {
               //nav->getTrueBearingRange(getLatitude(), getLongitude(), getAltitude(), bearingIsValid, bearing, rngIsValid, range, grdrange);
   //            currentMagVar = nav->magVariance();
   //            nav->unref();
   //            ok = true;
   //         }
   //      }
   //      getNavaidLoader()->clearDbInUse();
   //   }
   //}

   return ok;
}

//==============================================================================
// Class: IlsRadio
//==============================================================================
IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(IlsRadio,"IlsRadio")
EMPTY_SERIALIZER(IlsRadio)

const LCreal IlsRadio::GS_DEG_PER_DOT = 0.25f;
const LCreal IlsRadio::LOC_DEG_PER_DOT = 1.25f;

//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
IlsRadio::IlsRadio()
{
    STANDARD_CONSTRUCTOR()

    setMaxDetectRange(35.0);

    timerCounter = 0;
    destLatitude = 0;
    destLongitude = 0;
    currentMagVar = 0;
    localizerValid = false;
    glideSlopeValid = false; 
    range = 0;
    grdrange = 0;
    bearing = 0; 
    ilsGlideSlope = 0;
    acGlideSlope = 0;
    deltaGlideSlope = 0;
    ilsLocalizerBearing = 0;
    acLocalizerBearing = 0;
    deltaLocalizerBearing = 0;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IlsRadio::copyData(const IlsRadio& org, const bool)
{
    BaseClass::copyData(org);

    timerCounter = org.timerCounter;
    destLatitude = org.destLatitude;
    destLongitude = org.destLongitude;
    currentMagVar = org.currentMagVar;
    localizerValid = org.localizerValid;
    glideSlopeValid = org.glideSlopeValid; 
    range = org.range;
    grdrange = org.grdrange;
    bearing = org.bearing; 
    ilsGlideSlope = org.ilsGlideSlope;
    acGlideSlope = org.acGlideSlope;
    deltaGlideSlope = org.deltaGlideSlope;
    ilsLocalizerBearing = org.ilsLocalizerBearing;
    acLocalizerBearing = org.acLocalizerBearing;
    deltaLocalizerBearing = org.deltaLocalizerBearing;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IlsRadio::deleteData()
{
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void IlsRadio::updateData(const LCreal dt)
{
   //Must have ownship
    if (getOwnship() == 0) return;
    
    // Set our position to that of our ownship vehicle
    setPosition();                

    //Do not update data every time...Hard code delay length...
    if(timerCounter==0){
        //Get Glideslope and Runway orientation:
        glideSlopeValid = findILSGlideslopeByFreq(getFrequency());
        localizerValid = findLocalizerByFreq(getFrequency());
        
        //Get test results - make sure dest LL do not change as plane flies
        Basic::Nav::gbd2ll(getLatitude(),getLongitude(),bearing,grdrange,&destLatitude,&destLongitude);
        //Test for bad result...
        if((glideSlopeValid==false)|(localizerValid==false)){
            //std::cerr << "No ILS In Range..." << std::endl;
        }
    }
    else if(timerCounter==30){
        timerCounter=0; //Make sure the lookup occurs on the next cycle
    }
    else{
        timerCounter++; //Do not do another DB lookup - wait for some time
    }

    BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// Outside FCNs
//------------------------------------------------------------------------------
LCreal IlsRadio::getGlideslopeDifference(void)
{
    return deltaGlideSlope;
}

LCreal IlsRadio::getGlideslopeDifferenceDots() 
{
    return deltaGlideSlope / GS_DEG_PER_DOT;
}

LCreal IlsRadio::getLocalizerDifference(void)
{
    return deltaLocalizerBearing;
}

LCreal IlsRadio::getLocalizerDifferenceDots()
{
    return deltaLocalizerBearing / LOC_DEG_PER_DOT;
}

//------------------------------------------------------------------------------
//Find ILS Glideslope By Freq - ALWAYS FIND GLIDESLOPE FIRST THEN LOCALIZER!!!
//------------------------------------------------------------------------------
bool IlsRadio::findILSGlideslopeByFreq(LCreal freq)
{
   //Reset glideSlopeValid so only the first valid glide slope is returned...
   glideSlopeValid = false;

   if (getAirportLoader() != 0 && freq > 0) {
      if (getAirportLoader()->requestDbInUse()) {
         //Set the active area:
         getAirportLoader()->setArea(getLatitude(), getLongitude(), getMaxDetectRange());
         //See what the results are:
         int found = getAirportLoader()->queryIlsByFreq((float)freq);
         
         //Sort through the results and check the type - assume the closest ones are correct
         //"found" list is already sorted from closest to farthest away:
         for (int i = 0; i < found; i++) {
            Dafif::Ils* p = getAirportLoader()->getIls(i);
            //Debug Prints:
            //p->printRecord(std::cout);
            //Get Glideslope data here:
            if((!glideSlopeValid)&&(p->isIlsType(Dafif::Ils::GLIDESLOPE))){
               //Glideslope should not effect the bearing data from the localizer
               currentMagVar = p->magVariance();
               float ilsGS(0), acGS(0), delGS(0);
               p->getGlideSlopeData(getLatitude(),getLongitude(),getAltitude(),&ilsGS,&acGS,&delGS);
               ilsGlideSlope = ilsGS;
               acGlideSlope= acGS;
               deltaGlideSlope = delGS;
               glideSlopeValid = true;
            }
            //Toast the object:
            p->unref();
         }
         getAirportLoader()->clearDbInUse();
      }
   }
   return glideSlopeValid;
}

//------------------------------------------------------------------------------
//Find Localizer by freq - ALWAYS FIND GLIDESLOPE FIRST THEN FIND LOCALIZER!!!
//------------------------------------------------------------------------------
bool IlsRadio::findLocalizerByFreq(LCreal freq)
{
   localizerValid = false;
   ////Temp Variables...Will use to distiguish low and high end of runway...
   //LCreal magHeading1 = 0;
   //LCreal magHeading2 = 0;
   //double trueBearing1 = 0;
   //double trueBearing2 = 0;

   //if (getAirportLoader() != 0 && freq > 0) {
   //   if (getAirportLoader()->requestDbInUse()) {
   //      //Set the active area to search
   //      getAirportLoader()->setArea(getLatitude(), getLongitude(), getMaxDetectRange());
   //      //Find a list that matches freq
   //      int found = getAirportLoader()->queryRunwayByFreq(freq);
   //      //Debug Display:
   //      //std::cout << "RWY Loc Found = " << found << std::endl;
   //      //Run through the list and find the closest at the appropreate angle:
   //      for (int i = 0; i < found; i++) {
   //            Dafif::Runway* p = getAirportLoader()->getRunway(i);
   //            //Get the RWY orientation and bearing for both ends of the runway:
   //            p->getRunwayMagHeading(getLatitude(),getLongitude(),getAltitude(),&magHeading1,&magHeading2,&trueBearing1,&trueBearing2);
   //            //Just for kicks find how far away we are from the end of the runway:
               //p->getTrueBearingRange(getLatitude(),getLongitude(),getAltitude(),&bearing,&range,&grdrange);
   //            
   //            //Need a test to see if we want the low end or high end here:
   //            acLocalizerBearing = (LCreal)trueBearing1;
   //            //If localizer is from -270 - 90 we know the plane is south and will use low end of RWY
   //            if((acLocalizerBearing>0)&(acLocalizerBearing<=90)|(acLocalizerBearing>270)){
   //               //Use the Low End of RWY
   //               //Mag Var must be already calculated by glideslope marker - not available from localizer(p) here (argg)
   //               ilsLocalizerBearing = magHeading1+currentMagVar;
   //               deltaLocalizerBearing = ilsLocalizerBearing-acLocalizerBearing;
   //            }
   //            else{
   //               //Use the High End of RWY
   //               //Mag Var must be already calculated by glideslope marker - not available from localizer(p) here (argg)
   //               ilsLocalizerBearing = magHeading2+currentMagVar;
   //               deltaLocalizerBearing = ilsLocalizerBearing-acLocalizerBearing;
   //            }
               
   //            localizerValid = true;
   //            //Delete the object
   //            p->unref();
   //      }
   //   getAirportLoader()->clearDbInUse();
   //   }
   //}
   return localizerValid;
}

} // End Simulation namespace
} // End Eaagles namespace
