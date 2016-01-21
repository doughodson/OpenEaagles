
//==============================================================================
// Class: IrSensor
//==============================================================================

// NOTE:  This sensor is expected to act in concert with an OnboardComputer and a
// AngleOnlyTrackManager.

// The order of the components in the input data file is significant. If the
// computer is listed before the sensor, it will complete its process() before the
// sensor does, resulting in it not receiving inputs until the next frame. If listed
// afterwards, it will complete its process after the sensor, and will accept inputs
// in the same frame in which they were generated.

#include "openeaagles/simulation/IrAtmosphere.h"
#include "openeaagles/simulation/IrSeeker.h"
#include "openeaagles/simulation/AngleOnlyTrackManager.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/IrSensor.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Angles.h"

#include <cmath>

namespace oe {
namespace Simulation {

//IMPLEMENT_SUBCLASS(IrSensor,"IrSensor")
IMPLEMENT_PARTIAL_SUBCLASS(IrSensor,"IrSensor")
EMPTY_SERIALIZER(IrSensor)

// Slot table
BEGIN_SLOTTABLE(IrSensor)
   "lowerWavelength",   // 1: Lower wavelength limit       (microns)
   "upperWavelength",   // 2: Upper wavelength limit       (microns)
   "nei",               // 3: Noise Equivalent Irradiance  (watts/str-cm^2)
   "threshold",         // 4: Signal to Noise Threshold
   "IFOV",              // 5: Instantaneous Field of View  (steradians)
   "sensorType",        // 6: Sensor Type                  (Contrast, Hot Spot)
   //"FOR",             // 7: Field of Regard
   //"azimuthBin",      // 7: azimuthBin
   //"elevationBin",    // 8: elevationBin
   "maximumRange",      // 7: Maximum Range
   "trackManagerName",  // 8: Name of the requested Track Manager (Basic::String)
END_SLOTTABLE(IrSensor)

//  Map slot table
BEGIN_SLOT_MAP(IrSensor)
   ON_SLOT(1,setSlotLowerWavelength,Basic::Number)
   ON_SLOT(2,setSlotUpperWavelength,Basic::Number)
   ON_SLOT(3,setSlotNEI,Basic::Number)
   ON_SLOT(4,setSlotThreshold,Basic::Number)
   ON_SLOT(5,setSlotIFOV,Basic::Number)
   ON_SLOT(6,setSlotSensorType,Basic::String)
   //ON_SLOT(7,setSlotFieldOfRegard,Basic::Number)
   //ON_SLOT(7,setSlotAzimuthBin,Basic::Number)
   //ON_SLOT(8,setSlotElevationBin,Basic::Number)
   ON_SLOT(7,setSlotMaximumRange,Basic::Number)
   ON_SLOT(8,setSlotTrackManagerName,Basic::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructors, destructor, copy operator and clone()
//------------------------------------------------------------------------------
IrSensor::IrSensor() : storedMessagesQueue(MAX_EMISSIONS), storedMessagesLock(0), tmName(nullptr), trackManager(nullptr)
{
   STANDARD_CONSTRUCTOR()

   lowerWavelength = 0.0;
   upperWavelength = 0.0;
   nei = 0.0;
   threshold = 0.0;
   ifov = 0.0;
   ifovTheta = 0.0;
   sensorType = HOTSPOT;
   //fieldOfRegard = 0;
   //fieldOfRegardTheta = 0;
   //azimuthBin = 0.0f;
   //elevationBin = 0.0f;
   maximumRange = 0.0;
}

IrSensor::~IrSensor()
{
   STANDARD_DESTRUCTOR()
}

IrSensor::IrSensor(const IrSensor& org) : storedMessagesQueue(MAX_EMISSIONS), storedMessagesLock(0)
{
    STANDARD_CONSTRUCTOR()
    copyData(org, true);
}

IrSensor* IrSensor::clone() const
{
    return new IrSensor(*this);
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void IrSensor::copyData(const IrSensor& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
       tmName = nullptr;
       trackManager = nullptr;
   }

   lowerWavelength = org.lowerWavelength;
   upperWavelength = org.upperWavelength;
   nei = org.nei;
   threshold = org.threshold;
   ifov = org.ifov;
   ifovTheta = org.ifovTheta;
   sensorType = org.sensorType;
   //fieldOfRegard = org.fieldOfRegard;
   //fieldOfRegardTheta = org.fieldOfRegardTheta;
   //azimuthBin = org.azimuthBin;
   //elevationBin = org.elevationBin;
   maximumRange = org.maximumRange;

   if (org.tmName != nullptr) {
      Basic::String* copy = org.tmName->clone();
      setTrackManagerName( copy );
      copy->unref();
   }
   else {
      setTrackManagerName(nullptr);
   }

   // do not copy data.
   clearTracksAndQueues();
}


//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void IrSensor::deleteData()
{
   setTrackManager(nullptr);
   setTrackManagerName(nullptr);
   clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool IrSensor::shutdownNotification()
{
   setTrackManager(nullptr);
   clearTracksAndQueues();
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void IrSensor::reset()
{
   BaseClass::reset();

   // ---
   // Do we need to find the track manager?
   // ---
   lcLock(storedMessagesLock);
   if (getTrackManager() == nullptr && getTrackManagerName() != nullptr && getOwnship() != nullptr) {
      // We have a name of the track manager, but not the track manager itself
      const char* name = *getTrackManagerName();

      // Get the named track manager from the onboard computer
      OnboardComputer* obc = getOwnship()->getOnboardComputer();
      if (obc != nullptr) {
         setTrackManager( obc->getTrackManagerByName(name) );
      }

      if (getTrackManager() == nullptr) {
          // The assigned track manager was not found!
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "IrSensor::reset() ERROR -- track manager, " << name << ", was not found!" << std::endl;
          }
         setTrackManagerName(nullptr);
      }
   }
   lcUnlock(storedMessagesLock);
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void IrSensor::updateData(const LCreal dt)
{
    BaseClass::updateData(dt);

   // ---
   // Do we need to find the track manager?
   // ---
   if (getTrackManager() == nullptr && getTrackManagerName() != nullptr && getOwnship() != nullptr) {
      // We have a name of the track manager, but not the track manager itself
      const char* name = *getTrackManagerName();

      // Get the named track manager from the onboard computer
      OnboardComputer* obc = getOwnship()->getOnboardComputer();
      if (obc != nullptr) {
         setTrackManager( obc->getTrackManagerByName(name) );
      }

      if (getTrackManager() == nullptr) {
         // The assigned track manager was not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSensor::reset() ERROR -- track manager, " << name << ", was not found!" << std::endl;
         }
         setTrackManagerName(nullptr);
      }
   }
}

//------------------------------------------------------------------------------
// transmit() -- send radar emissions
//------------------------------------------------------------------------------
void IrSensor::transmit(const LCreal dt)
{
   BaseClass::transmit(dt);

   // In transmit (request IR) mode and have a IrSeeker
   IrSeeker* seeker = dynamic_cast<IrSeeker*>( getSeeker() );
   if (seeker != nullptr && isQuerying()) {
      // Send the emission to the other player
      IrQueryMsg* irQuery = new IrQueryMsg();
      if (irQuery != nullptr) {
         irQuery->setLowerWavelength(getLowerWavelength());
         irQuery->setUpperWavelength(getUpperWavelength());
         irQuery->setInstantaneousFieldOfView(getIFOV());
         irQuery->setSendingSensor(this);
         irQuery->setNEI(getNEI());
         irQuery->setMaxRangeNM(getMaximumRange()* Basic::Distance::M2NM);
         seeker->irRequestSignature(irQuery);
         irQuery->unref();
      } // If irQuery not null
      else {
            if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "Error: IrQueryMsg memory failure in IrSensor.cpp in IrSensor::transmit" << std::endl;
            }
      }
   }
}

// this is called by the IrSeeker in the transmit frame, once for each target that returns a query
bool IrSensor::calculateIrQueryReturn(IrQueryMsg* const msg)
{
   Player* ownship = getOwnship();
   IrAtmosphere* atmos = nullptr;
   LCreal totalSignal = 0.0;
   LCreal totalBackground = 0.0;

   if (msg->getSendingSensor() != this) {
      // this should not happen
   }

   if (ownship != nullptr) {
      Simulation* sim = ownship->getSimulation();
      if (sim)
         atmos = sim->getIrAtmosphere();
   }

   if (atmos == nullptr) {
      // assume simple signature
      totalSignal = msg->getSignatureAtRange();
   }
   else {
      atmos->calculateAtmosphereContribution(msg, &totalSignal, &totalBackground);
   }

   if (totalSignal > 0.0) {

      const LCreal targetRange = msg->getRange();
      const LCreal rangeSquared =  targetRange * targetRange;
      const LCreal reflectorArea = msg->getProjectedArea();
      const LCreal ifov = msg->getInstantaneousFieldOfView();

      // The relevant amount of background area is the field of view multiplied by the range squared

      const LCreal backgroundArea =  ifov * rangeSquared;

      // The seeker detects by comparing the amount of signal present
      // with the target to the amount of signal there would be without the target.
      // The amount of signal with the target represents the signal power plus
      // that part of the background radiation that is not blocked by the target.
      // If the target is larger than the field of view than it is all of background
      // power in the field of view. Use this as the default value.

     // noiseBlockedByTarget is irradiance, in units of watts/m^2

      LCreal noiseBlockedByTarget = totalBackground * ifov;

      // If the target is smaller than the field of view, it is the background power
      // in the effective field of view represented by the target, i.e. the
      // target area divided by the range squared.
      if (reflectorArea < backgroundArea) {
         // these two are equivalent
         // noiseBlockedByTarget *=  (reflectorArea/backgroundArea)
          noiseBlockedByTarget = (totalBackground * reflectorArea) / rangeSquared;
      }

      // attenuatedPower is irradiance, in watts/m^2
      const LCreal attenuatedPower = totalSignal / rangeSquared;

      // signalAboveNoise is the signal that the detector sees minus what it would see with
      // only the background radiation, and is just the amount of power subtracted by how much
      // background power is being blocked.
      // = (attenuatedPower + totalBackground*ifov - noiseBlockedByTarget) - totalBackground*ifov
      // signalAboveNoise is irradiance, in watts/m^2

      LCreal signalAboveNoise = attenuatedPower - noiseBlockedByTarget;

      // only Contrast seekers take absolute value in this equation.
      // Hotspot does not.

      if (signalAboveNoise < 0.0 &&
               (msg->getSendingSensor()->getSensorType() == IrSensor::CONTRAST)) {
         signalAboveNoise = -signalAboveNoise;
      }

      const LCreal nei = msg->getNEI();

      // Determine the ratio between the signal above the noise as compared to the level of
      // radiation that would create a response at the same level as the sensor's internal noise.
      // if NEI is in watts/m^2, then SNR will be dimensionless.
      // if NEI is in watts/cm^2, then need to correct by 10^4.

      const LCreal signalToNoiseRatio = signalAboveNoise / nei;
      const LCreal backgroundNoiseRatio = noiseBlockedByTarget / nei;
      //LCreal signalToNoiseThreshold = msg->getSendingSensor()->getThreshold();

      // allow all signals to be returned; threshold test will be applied in process()
      {
         IrQueryMsg* outMsg = new IrQueryMsg();
         outMsg->setTarget(msg->getTarget());
         outMsg->setGimbalAzimuth( static_cast<LCreal>(msg->getGimbal()->getAzimuth()) );
         outMsg->setGimbalElevation( static_cast<LCreal>(msg->getGimbal()->getElevation()) );
         outMsg->setAzimuthAoi(msg->getAzimuthAoi());
         outMsg->setElevationAoi(msg->getElevationAoi());

         osg::Vec3 los = msg->getLosVec();

         {
            // This is for non-ownHdgOnly-stabilized gimbal angles
            osg::Vec4 los0( los.x(), los.y(), los.z(), 0.0 );
            osg::Vec4 los_vec = ownship->getRotMat() * los0;
            LCreal ra = lcSqrt(los_vec.x() * los_vec.x() + los_vec.y()*los_vec.y());
            LCreal az = lcAtan2(los_vec.y(), los_vec.x());
            LCreal el = lcAtan2(-los_vec.z(), ra);
            outMsg->setRelativeAzimuth(az);
            outMsg->setRelativeElevation(el);
         }

         outMsg->setLosVec(msg->getLosVec());
         outMsg->setTgtLosVec( -msg->getLosVec() );
         outMsg->setPosVec(msg->getTarget()->getPosition());
         outMsg->setVelocityVec(msg->getTarget()->getVelocity());
         outMsg->setAccelVec(msg->getTarget()->getAcceleration());

         LCreal angleAspect1 = outMsg->getPosVec().y() *
                                 outMsg->getVelocityVec().x() -
                                 outMsg->getPosVec().x() *
                                 outMsg->getVelocityVec().y();

         LCreal angleAspect2 = outMsg->getPosVec().x() *
                                 outMsg->getVelocityVec().x() +
                                 outMsg->getPosVec().y() *
                                 outMsg->getVelocityVec().y();

         outMsg->setAngleAspect(lcAtan2(-angleAspect1,angleAspect2));

         outMsg->setSignalToNoiseRatio(signalToNoiseRatio);
         outMsg->setBackgroundNoiseRatio(backgroundNoiseRatio);
         outMsg->setSendingSensor(msg->getSendingSensor());

         // probably unnecessary - should be default val
         outMsg->setQueryMergeStatus(IrQueryMsg::NOT_MERGED);   // FAB

         msg->getSendingSensor()->addStoredMessage(outMsg);
      }
      //else   // FAB - debug
      //{
      // int x = 0;
      // x = x +1;
      // }
   } //totalSignal > 0
 //  else   // FAB - debug
 //  {
    //   int x = 0;
    //   x = x +1;
    //}

   return true;
}


void IrSensor::process(const LCreal dt)
{
   BaseClass::process(dt);

   int numRecords = storedMessagesQueue.entries();
   if (numRecords > 0) {
      AngleOnlyTrackManager* tm = static_cast<AngleOnlyTrackManager*>(getTrackManager());
      if (tm != nullptr) {
         lcLock(storedMessagesLock);
         numRecords = storedMessagesQueue.entries();

         // Send on all messages EXCEPT those with signal below threshold and those merged
         // into another signal. Those will simply be ignored and unreferenced.

         for (int i=0; i < numRecords; i++) {
            IrQueryMsg* msg = storedMessagesQueue.get();
            if (msg->getQueryMergeStatus() != IrQueryMsg::MERGED_OUT) {
               if (msg->getSignalToNoiseRatio() > getThreshold())
                  tm->newReport(msg, msg->getSignalToNoiseRatio());
            }
            msg->unref();
         }
         lcUnlock(storedMessagesLock);
      }
   }
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setLowerWavelength() - Sets the lower wavelength (microns; must be greater than or equal to 0)
bool IrSensor::setLowerWavelength(const LCreal w)
{
   bool ok = false;
   if (w >= 0) {
      lowerWavelength = w;
      ok = true;
   }
   return ok;

}

// setUpperWavelength() - Sets the upper wavelength (microns; must be greater than 0)
bool IrSensor::setUpperWavelength(const LCreal w)
{
   bool ok = false;
   if (w > 0) {
      upperWavelength = w;
      ok = true;
   }
   return ok;
}

// setNEI() - Sets the Noise Equivalent Irradiance  (watts/str-cm^2); must be greater than or equal to 0)
bool IrSensor::setNEI(const LCreal n)
{
   bool ok = false;
   if (n >= 0) {
      nei = n;
      ok = true;
   }
   return ok;
}

// setThreshold() - Sets the Signal to Noise Threshold
bool IrSensor::setThreshold(const LCreal t)
{
   bool ok = false;
   if (t >= 0) {
      threshold = t;
      ok = true;
   }
   return ok;
}

// setIFOV() - Sets the Instantaneous Field of View  (steradians)
// also sets ifovTheta (radians) -- planar angle.
// FAB - solid angle = 2 * pi * (1-std::cos(theta/2)) -- formula below actually returns ifovtheta/2
//   - but all IR code that references ifovtheta actually wants ifovtheta/2 anyway
bool IrSensor::setIFOV(const LCreal i)
{
   bool ok = false;
   if (i >= 0) {
      ifov = i;
      //calculate planar angle and set it as well.
      ifovTheta =  static_cast<LCreal>(std::acos ((1 - (ifov / (2.0 * PI)))));
      ok = true;
   }
   return ok;
}


// setSensorType() - Sets the Instantaneous Field of View  (steradians)
bool IrSensor::setSensorType(const SensorType st)
{
   sensorType = st;
   return true;
}

// setFieldOfRegard() - Sets the Instantaneous Field of View  (steradians)
// FAB - solid angle = 2 * pi * (1-std::cos(theta/2)) -- formula below actually returns fieldOfRegardTheta/2
//     - but all IR code that references fieldOfRegardTheta actually wants fieldOfRegardTheta/2 anyway
//bool IrSensor::setFieldOfRegard(const LCreal fov)
//{
//   bool ok = false;
//   if (fov > 0) {
//      fieldOfRegard = fov;
//      //calculate planar angle and set it as well.
//      fieldOfRegardTheta =  (LCreal) acos ((1 - (fieldOfRegard / (2.0 * PI))));
//      ok = true;
//   }
//   return ok;
//}

//// setAzimuthBin() - Sets the lower Azimuth Bin
//bool IrSensor::setAzimuthBin(const LCreal w)
//{
//   azimuthBin = w;
//   return true;
//}
//
//// setElevationBin() - Sets the lower Elevation Bin
//bool IrSensor::setElevationBin(const LCreal w)
//{
//   elevationBin = w;
//   return true;
//}
//
//bool IrSensor::setSlotAzimuthBin(const Basic::Number* const msg)
//{
//   LCreal value = 0.0f;
//
//   const Basic::Angle* a = dynamic_cast<const Basic::Angle*>(msg);
//   if (a != 0) {
//       Basic::Radians r;
//       value = (LCreal)r.convert(*a);
//   }
//   else if (msg != 0) {
//      value = msg->getReal();
//   }
//
//   setAzimuthBin(value);
//
//   return true;
//}

bool IrSensor::setMaximumRange(const LCreal w)
{
   maximumRange = w;
   return true;
}

bool IrSensor::setSlotMaximumRange(const Basic::Number* const msg)
{
   LCreal value = 0.0;

   const Basic::Distance* d = dynamic_cast<const Basic::Distance*>(msg);
   if (d != nullptr) {
       Basic::Meters m;
       value = static_cast<LCreal>(m.convert(*d));
   }
   else if (msg != nullptr) {
      value = msg->getReal();
   }

   setMaximumRange(value);
   return true;
}

//bool IrSensor::setSlotElevationBin(const Basic::Number* const msg)
//{
//   LCreal value = 0.0f;
//
//   const Basic::Angle* a = dynamic_cast<const Basic::Angle*>(msg);
//   if (a != 0) {
//       Basic::Radians r;
//       value = (LCreal)r.convert(*a);
//   }
//   else if (msg != 0) {
//      value = msg->getReal();
//   }
//
//   setElevationBin(value);
//
//   return true;
//}

// setSlotLowerWavelength() - Sets lower wavelength
bool IrSensor::setSlotLowerWavelength(const Basic::Number* const msg)
{
   LCreal value = 0.0;
   bool ok = false;

   const Basic::Distance* d = dynamic_cast<const Basic::Distance*>(msg);
   if (d != nullptr) {
       Basic::MicroMeters mm;
       value = static_cast<LCreal>(mm.convert(*d));
   }
   else if (msg != nullptr) {
      value = msg->getReal();
   }
   ok = setLowerWavelength(value);

   if (!ok) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "IrSensor::setSlotLowerWavelength: Error setting Lower Wavelength!" << std::endl;
      }
   }
   return ok;
}

// setSlotUpperWavelength() - Sets upper wavelength
bool IrSensor::setSlotUpperWavelength(const Basic::Number* const msg)
{
   bool ok = false;
   LCreal value = 0.0;

   const Basic::Distance* d = dynamic_cast<const Basic::Distance*>(msg);
   if (d != nullptr) {
       Basic::MicroMeters mm;
       value = static_cast<LCreal>(mm.convert(*d));
   }
   else if (msg != nullptr) {
      value = msg->getReal();
   }
   ok = setUpperWavelength(value);
   if (!ok) {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "IrSensor::setUpperWavelength: Error setting Upper Wavelength!" << std::endl;
      }
   }
   return ok;
}

// setSlotNEI() - Sets Noise Equivalent Irradiance
bool IrSensor::setSlotNEI(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      LCreal x = msg->getReal();
      ok = setNEI(x);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSensor::setNEI: Error setting Noise Equivalent Irradiance!" << std::endl;
         }
      }
   }
   return ok;
}

// setSlotThreshold() - Sets Signal to Noise Threshold
bool IrSensor::setSlotThreshold(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      LCreal x = msg->getReal();
      ok = setThreshold(x);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSensor::setSlotThreshold: Error setting Signal to Noise Threshold!" << std::endl;
         }
      }
   }
   return ok;
}

// setSlotIFOV() - Sets Instantaneous Field of View
bool IrSensor::setSlotIFOV(const Basic::Number* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      LCreal x = msg->getReal();
      ok = setIFOV(x);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSensor::setSlotIFOV: Error setting IFOV!" << std::endl;
         }
      }
   }
   return ok;
}

// setSlotSensorType() -- Sets the Sensor Type
bool IrSensor::setSlotSensorType(const Basic::String* const msg)
{
   bool ok = false;

   if (msg != nullptr) {
      if (*msg == "contrast") ok = setSensorType(CONTRAST);
      else if (*msg == "hot spot") ok = setSensorType(HOTSPOT);
      if (!ok) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IrSensor::setSlotSensorType: Error setting Sensor Type!" << std::endl;
         }
      }
   }
   return ok;
}

// setSlotFieldOfRegard() - Sets the field of regard
//bool IrSensor::setSlotFieldOfRegard(const Basic::Number* const msg)
//{
//   bool ok = false;
//   if (msg != 0) {
//      LCreal x = msg->getReal();
//      ok = setFieldOfRegard(x);
//      if (!ok) {
//                if (isMessageEnabled(MSG_ERROR)) {
//                    std::cerr << "IrSensor::setSlotFieldOfRegard: Error setting Field of Regard!" << std::endl;
//                }
//      }
//   }
//
//   return ok;
//}
// setSlotTrackManagerName() -- sets the name of the track manager;
// we'll lookup the actual track manager in reset() later
bool IrSensor::setSlotTrackManagerName(Basic::String* const v)
{
    return setTrackManagerName(v);
}
//------------------------------------------------------------------------------
// setTrackManagerName() -- Sets the track manager's name
//------------------------------------------------------------------------------
bool IrSensor::setTrackManagerName(Basic::String* name)
{
    if (tmName != nullptr) {
        tmName->unref();
    }
    tmName = name;
    if (tmName != nullptr) {
        tmName->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// setTrackManager() -- Sets the track manager
//------------------------------------------------------------------------------
bool IrSensor::setTrackManager(TrackManager* tm)
{
    if (trackManager != nullptr) {
        trackManager->unref();
    }
    trackManager = tm;
    if (trackManager != nullptr) {
        trackManager->ref();
    }
    return true;
}
//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* IrSensor::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

// Returns the requested track manager's name
const Basic::String* IrSensor::getTrackManagerName() const
{
   return tmName;
}

// Returns our current track manager
TrackManager* IrSensor::getTrackManager()
{
   return trackManager;
}

// Returns our current track manager (const version)
const TrackManager* IrSensor::getTrackManager() const
{
   return trackManager;
}
//------------------------------------------------------------------------------
// getStoredMessage() -- Get the next stored message
//------------------------------------------------------------------------------
IrQueryMsg* IrSensor::getStoredMessage()
{
   IrQueryMsg* msg = nullptr;

   lcLock(storedMessagesLock);
   msg = storedMessagesQueue.get();
   lcUnlock(storedMessagesLock);

   return msg;
}

//------------------------------------------------------------------------------
// peekStoredMessage() -- Return the ith stored object but do NOT remove it.
//------------------------------------------------------------------------------
IrQueryMsg* IrSensor::peekStoredMessage(unsigned int i)
{
   IrQueryMsg* msg = nullptr;

   lcLock(storedMessagesLock);
   msg = storedMessagesQueue.peek0(i);
   lcUnlock(storedMessagesLock);

   return msg;
}
//------------------------------------------------------------------------------
// addStoredMessage() -- Accept a new stored message
//------------------------------------------------------------------------------
void IrSensor::addStoredMessage(IrQueryMsg* msg)
{
   // Queue up emissions reports
   if (msg != nullptr) {
      lcLock(storedMessagesLock);
      storedMessagesQueue.put(msg);
      lcUnlock(storedMessagesLock);
   }
}
//------------------------------------------------------------------------------
// clearTracksAndQueues() -- clear out tracks and queues
//------------------------------------------------------------------------------
void IrSensor::clearTracksAndQueues()
{
   // ---
   // Clear out the queues
   // ---
   lcLock(storedMessagesLock);
   for (IrQueryMsg* msg = storedMessagesQueue.get(); msg != nullptr; msg = storedMessagesQueue.get())  {
      msg->unref();
   }
   lcUnlock(storedMessagesLock);
}

}
}

