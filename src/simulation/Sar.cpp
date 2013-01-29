
#include "openeaagles/simulation/Sar.h"

#include "openeaagles/simulation/Antenna.h"
#include "openeaagles/simulation/Image.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/Number.h"
#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(Sar,"Sar")
EMPTY_SERIALIZER(Sar)

// slot table for this class type
BEGIN_SLOTTABLE(Sar)
    "chipSize",         // 1) Chip size (pixels)   <Basic::Number>
END_SLOTTABLE(Sar)

//  Map slot table to handles
BEGIN_SLOT_MAP(Sar)
    ON_SLOT( 1, setSlotChipSize,  Basic::Number)
END_SLOT_MAP()

// Default parameters
static const LCreal DEFAULT_SAR_TIME = 10.0f;

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Sar::Sar() : imgList(0)
{
   STANDARD_CONSTRUCTOR()

   nextId = 1;
   stareLatitude = 0;
   stareLongitude = 0;
   stareElevation = 0;
   resolution = 1;
   chipSize = 0;
   busy = false;
   haveLL = false;

   setReceiverEnabledFlag(false);
   setTransmitterEnableFlag(false);

   width = 0;
   height = 0;
   timer = 0;
}

//------------------------------------------------------------------------------
// copyData(), deleteData() -- copy (delete) member data
//------------------------------------------------------------------------------
void Sar::copyData(const Sar& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      imgList = 0;
      setReceiverEnabledFlag(false);
      setTransmitterEnableFlag(false);
   }

   // Copy our data
   nextId = org.nextId;
   stareLatitude = org.stareLatitude;
   stareLongitude = org.stareLongitude;
   stareElevation = org.stareElevation;
   resolution = org.resolution;
   chipSize = org.chipSize;
   busy = org.busy;
   haveLL = org.haveLL;

   width = org.width;
   height = org.height;
   timer = org.timer;

   // Copy the images
   if (imgList != 0) {
      imgList->unref();
      imgList = 0;
   }
   if (org.imgList != 0) {
      imgList = org.imgList->clone();
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void Sar::deleteData()
{
    // Clear the images
    if (imgList != 0) {
        imgList->unref();
        imgList = 0;
    }
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

// Is a SAR being imaged?
bool Sar::isImagingInProgress() const
{
    return busy || timer > 0;
}

// Is the SAR system ready?
bool Sar::isSystemReady() const
{
    return haveStarePoint() && !isImagingInProgress();
}

// Return a list of all images
Basic::PairStream* Sar::getImages()
{
    Basic::PairStream* p = imgList;
    if (p != 0) p->ref();
    return p;
}

// Returns the last image
const Image* Sar::getImage() const
{
    const Image* p = 0;
    if (imgList != 0) {
        const Basic::Pair* pair = imgList->getPosition( imgList->entries() );    // Last item
        p = dynamic_cast<const Image*>(pair->object());
        if (p != 0) p->ref();
    }
    return p;
}

//------------------------------------------------------------------------------
// setStarePoint() -- Set the stare point (lat and lon are in degrees, elev is in meters)
//------------------------------------------------------------------------------
bool Sar::setStarePoint(const double lat, const double lon, const LCreal elev)
{
    stareLatitude = lat;
    stareLongitude = lon;
    stareElevation = elev;
    haveLL = true;
    return true;
}

//-----------------------------------------------------------------------------
// Set functions
//-----------------------------------------------------------------------------

bool Sar::setSlotChipSize(const Basic::Number* const msg)
{
    bool ok = false;
    if (msg != 0) {
        int n = msg->getInt();
        if (n >= 0) {
            ok = setChipSize( n );
        }
    }
    return ok;
}

bool Sar::setResolution(const LCreal res)
{
   resolution = res;
   return true;
}

//------------------------------------------------------------------------------
// requestImage() -- Request a new image: 
//------------------------------------------------------------------------------
bool Sar::requestImage(
        const unsigned int w,           // Image width (pixels)
        const unsigned int h,           // Image height (pixels)
        const LCreal r)                 // Image Resolution (meters/pixel)
{
   bool ok = false;
   if ( isSystemReady() ) {
      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "starting new SAR (" << w << "," << h << ") at " << r << std::endl;
      }
      width = w;
      height = h;
      resolution = r;
      timer = DEFAULT_SAR_TIME;
      setTransmitterEnableFlag(true);
   }
   return ok;
}

//------------------------------------------------------------------------------
// cancel() -- Cancel the current SAR imaging   
//------------------------------------------------------------------------------
void Sar::cancel()
{
   setTransmitterEnableFlag(false);
   timer = 0;
}

//------------------------------------------------------------------------------
// process() -- 
//------------------------------------------------------------------------------
void Sar::process(const LCreal dt)
{
    BaseClass::process(dt);

    // Imaging in porgress?
   if (timer > 0) {

      // ---
      // Point the beam
      // ---
      Antenna* ant = getAntenna();
      if (ant != 0) {
         
         Simulation* s = getSimulation();
         double refLat = s->getRefLatitude();
         double refLon = s->getRefLongitude();

         osg::Vec3 pos;
         Basic::Nav::convertLL2PosVec(
            refLat, refLon,                           // Ref point (at sea level)
            getStarePointLatitude(), getStarePointLongitude(), getStarePointElevation(), 
            &pos); // x,y,z  NED

         // Platform (ownship) coord and then body
         osg::Vec3 posP = pos - getOwnship()->getPosition();
         osg::Vec3 posB = getOwnship()->getRotMat() * posP;

         // Convert to az/el
         LCreal tgt_az = 0.0;   // Angle (degs)
         LCreal tgt_el = 0.0;   // Angle (degs)
         xyz2AzEl(posB, &tgt_az, &tgt_el);

         // Command to that position
         LCreal az = tgt_az * (LCreal)Basic::Angle::D2RCC;
         LCreal el = tgt_el * (LCreal)Basic::Angle::D2RCC;

         ant->setRefAzimuth(az);
         ant->setRefElevation(el);
         ant->setScanMode(Antenna::CONICAL_SCAN);
      }

      // ---
      // Process timer
      // ---
      LCreal ttimer = timer - dt;
      if (ttimer <= 0) {

         // ### test -- Generate a test image ###
         Image* p = new Image();
         p->testImage(width,height);
         p->setImageId(getNextId());
         p->setLatitude(getStarePointLatitude());
         p->setLongitude(getStarePointLongitude());
         p->setElevation(getStarePointElevation());
         p->setOrientation(0);
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Sar:: Generating test image: resolution: " << getResolution() << std::endl;
         }
         if (getResolution() > 0) p->setResolution( getResolution() );
         else p->setResolution( 3.0f * Basic::Distance::FT2M );
         Basic::Pair* pp = new Basic::Pair("image", p);
         addImage(pp);
         // ### TEST TEST 

         // Just finished!
         ttimer = 0;
         setTransmitterEnableFlag(false);
      }
      timer = ttimer;
   }

   BaseClass::updateData(dt);
}


//------------------------------------------------------------------------------
// getNextId() -- Get the next image ID and increment the counter
//------------------------------------------------------------------------------
int Sar::getNextId()
{
    return nextId++;
}

//------------------------------------------------------------------------------
// Set chip size (square -- pixels)
//------------------------------------------------------------------------------
bool Sar::setChipSize(const unsigned int pixels)
{
    chipSize = pixels;   
    return true;
}

//------------------------------------------------------------------------------
// addImage() -- Add an image to the end of the list of images 
//------------------------------------------------------------------------------
bool Sar::addImage(Basic::Pair* const newImage)
{
    bool ok = false;
    if (newImage != 0) {
        if (imgList == 0) {
            imgList = new Basic::PairStream();
        }
        imgList->put(newImage);
    }

    return ok;
}

//------------------------------------------------------------------------------
// xyz2AzEl() -- converts relative position vector to azimuth and elevation (degs)
//------------------------------------------------------------------------------
void Sar::xyz2AzEl(const LCreal x, const LCreal y, const LCreal z, LCreal* const az, LCreal* const el)
{
   // Compute azimuth (degs)
   if (az != 0) {
      *az = lcAtan2(y, x) * (LCreal)Basic::Angle::R2DCC;
   }

   if (el != 0) {
      LCreal r = lcSqrt(x * x + y * y);
      *el = lcAtan2(-z, r) * (LCreal)Basic::Angle::R2DCC;
   }
}

void Sar::xyz2AzEl(const osg::Vec3& vec, LCreal* const az, LCreal* const el)
{
   xyz2AzEl(vec.x(), vec.y(), vec.z(), az, el);
}
//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* Sar::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

} // End Simulation namespace
} // End Eaagles namespace
