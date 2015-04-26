//------------------------------------------------------------------------------
// Class: IrSensor
//
// An IrSensor must discover its seeker. This operations is performed by the IrSystem bases class,
// either in the background thread or during reset() processing.
// In some cases (during missile launch), waiting for the background thread to perform this
// operation may disrupt a seeker/sensor's ability to track a target.
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_IrSensor_H__
#define __Eaagles_Simulation_IrSensor_H__

#include "openeaagles/simulation/IrSystem.h"
#include "openeaagles/basic/safe_queue.h"

namespace Eaagles {

namespace Basic { class Integer; class Number; class String; }

namespace Simulation {

class IrSeeker;
class IrQueryMsg;
class Player;
class TrackManager;

//------------------------------------------------------------------------------
// Class: IrSensor
// Description: Base class for I/R Sensor Models.  Provides a common
//              interface, and some default behavior, for I/R sensors.
//
// Slots:
//
//    lowerWavelength     (Real)           The lower wavelength limit in microns
//
//    upperWavelength     (Real)           The upper wavelength limit in microns
//
//    nei                 (Real)           The Noise Equivalent Irradiance in watts/str-cm^2
//
//    threshold           (Real)           The Signal to Noise Threshold
//
//    IFOV                (Real)           The Instantaneous Field of View in steradians
//
//    sensorType          (String)         The type of sensor(CONTRAST, HOTSPOT)
//
//    FOR                 (Real)           The Field of Regard in steradians
//
// Events:
//    bool irQueryReturnEvent(IrQueryMsg* const irQuery);
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
//
//
// Public Member functions:
//
//    bool isScanOn()
//       Returns true if the Sensor scanning
//
//    LCreal getLowerWavelength()
//    bool setLowerWavelength(const LCreal w)
//       Gets/Sets the lower wavelength (microns; must be greater than 0)
//
//    LCreal getUpperWavelength()
//    bool setUpperWavelength(const LCreal w)
//       Gets/Sets the upper wavelength (microns; must be greater than 0)
//
//    LCreal getNEI()
//    bool setNEI(const LCreal n)
//       Gets/Sets the Noise Equivalent Irradiance  (watts/str-cm^2); must be greater than 0)
//
//    LCreal getThreshold()
//    bool setThreshold(const LCreal t)
//       Gets/Sets the Signal to Noise Threshold
//
//    LCreal getIFOV()
//    bool setIFOV(const LCreal i)
//       Gets/Sets the Instantaneous Field of View  (steradians)
//       What the sensor can see with no gimbal movement
//
//    SensorType getSensorType()
//    bool setSensorType(const SensorType st)
//       Gets/Sets the Type of sensor whether hot spot (only objects hotter than the environment
//           are detected) or contrast (objects hotter and cooler than the environment are observed)
//
//    LCreal getFieldOfRegard()
//    bool setFieldOfRegard(const LCreal fov)
//       Gets/Sets the Field of Regard  (steradians)
//       What the sensor can see with gimbal's full range of movement
//
//------------------------------------------------------------------------------
class IrSensor : public IrSystem
{
    DECLARE_SUBCLASS(IrSensor,IrSystem)

public:
   enum SensorType {
         CONTRAST,   // Detection by difference (hot or cold) from background
         HOTSPOT     // Detection if hotter than background
   };

public:

   IrSensor();

   virtual LCreal getLowerWavelength() const {return lowerWavelength; };   // Returns the lower wavelength (microns)
   virtual bool setLowerWavelength(const LCreal w);                        // Sets the lower wavelength (microns; must be greater than 0)

   virtual LCreal getUpperWavelength() const {return upperWavelength; };   // Returns the upper wavelength (microns)
   virtual bool setUpperWavelength(const LCreal w);                        // Sets the upper wavelength (microns; must be greater than 0)

   virtual LCreal getNEI() const {return nei; };   // Returns the Noise Equivalent Irradiance  (watts/str-cm^2)
   virtual bool setNEI(const LCreal n);            // Sets the Noise Equivalent Irradiance  (watts/str-cm^2); must be greater than 0)

   virtual LCreal getThreshold() const {return threshold; };   // Returns the Signal to Noise Threshold
   virtual bool setThreshold(const LCreal t);                  // Sets the Signal to Noise Threshold

   virtual LCreal getIFOVTheta() const {return ifovTheta; };   // Returns ifov planar angle (radians)
   virtual LCreal getIFOV() const {return ifov; };             // Returns the Instantaneous Field of View  (steradians)
   virtual bool setIFOV(const LCreal i);                       // Sets the Instantaneous Field of View  (steradians)

   virtual SensorType getSensorType() const {return sensorType; } ;  // Returns the type of sensor
   virtual bool setSensorType(const SensorType st);                  // Sets the type of sensor

   //virtual LCreal getFieldOfRegardTheta() const {return fieldOfRegardTheta; };   // Returns Field of Regard planar angle (radians)
   //virtual LCreal getFieldOfRegard() const { return fieldOfRegard; };            // Returns the Field of Regard  (steradians)
   //virtual bool setFieldOfRegard(const LCreal fov);                              // Sets the Field of Regard  (steradians)

   //virtual LCreal getAzimuthBin() const {return azimuthBin; };       // Returns min azimuth distance
   //virtual bool setAzimuthBin(const LCreal azimuthBin);              // Sets min Azimuth Distance

   //virtual LCreal getElevationBin() const {return elevationBin; };     // Returns min azimuth distance
   //virtual bool setElevationBin(const LCreal elevationBin);            // Sets min Azimuth Distance

   virtual LCreal getMaximumRange() const {return maximumRange; };   // Returns maximum Range
   virtual bool setMaximumRange(const LCreal maximumRange);          // Sets maximum Range

   virtual const Basic::String* getTrackManagerName() const; // Returns the requested track manager's name
   virtual bool setTrackManagerName(Basic::String* const a); // Sets the name of the track manager to use

   virtual TrackManager* getTrackManager();               // Returns our current track manager
   virtual const TrackManager* getTrackManager() const;   // Returns our current track manager (const version)
   virtual bool setTrackManager(TrackManager* const a);   // Sets the track manager

   // Slot functions
   virtual bool setSlotLowerWavelength(const Basic::Number* const msg);   // Sets lower wavelength
   virtual bool setSlotUpperWavelength(const Basic::Number* const msg);   // Sets upper wavelength
   virtual bool setSlotNEI(const Basic::Number* const msg);               // Sets Noise Equivalent Irradiance
   virtual bool setSlotThreshold(const Basic::Number* const msg);         // Sets Signal to Noise Threshold
   virtual bool setSlotIFOV(const Basic::Number* const msg);              // Sets Instantaneous Field of View
   virtual bool setSlotSensorType(const Basic::String* const msg);        // Sets the Sensor Type
   //virtual bool setSlotFieldOfRegard(const Basic::Number* const msg);   // Sets the field of regard
   //virtual bool setSlotAzimuthBin(const Basic::Number* const msg);      // Sets the Azimuth Bin
   //virtual bool setSlotElevationBin(const Basic::Number* const msg);    // Sets the Elevation Bin
   virtual bool setSlotMaximumRange(const Basic::Number* const msg);      // Sets the Maximum Range
   virtual bool setSlotTrackManagerName(Basic::String* const v);          // Sets our track manager by name

   // Store sensor reports until we are ready to pass on to track manager.
   void addStoredMessage(IrQueryMsg* msg);

   virtual bool calculateIrQueryReturn(IrQueryMsg* const irQuery);

   void updateData(const LCreal dt = 0.0) override;
   void reset() override;

protected:
   void transmit(const LCreal dt) override;
   void process(const LCreal dt) override;

   bool shutdownNotification() override;

   virtual IrQueryMsg* getStoredMessage();
   virtual IrQueryMsg* peekStoredMessage(unsigned int i);

   Basic::safe_queue<IrQueryMsg*> storedMessagesQueue;
   mutable long storedMessagesLock;          // Semaphore to protect 'storedMessagesQueue'

private:
   static const int MAX_EMISSIONS = 10000;   // Max size of emission queues and arrays

   void clearTracksAndQueues();

   // Characteristics
   LCreal lowerWavelength;          // Lower wavelength limit (microns)
   LCreal upperWavelength;          // Upper wavelength limit (microns)
   LCreal nei;                      // Noise Equivalent Irradiance  (watts/str-cm^2)
   LCreal threshold;                // Signal to Noise Threshold
   LCreal ifov;                     // Instantaneous Field of View  (steradians) (what is in view without gimbal movement)
   LCreal ifovTheta;                // IFOV planar angle, where ifov = 2 * pi * (1-cos(ifovTheta/2)

   // results in a simple cone.
   SensorType sensorType;             // Sensor Type(CONTRAST, HOTSPOT)
   //LCreal fieldOfRegard;            // Field of Regard (steradians) (what can be in view to gimbals limits)
   //LCreal fieldOfRegardTheta;       // Field of Regard planar angle, where fieldOfRegard = 2 * pi * (1-cos(fieldOfRegardTheta/2)

   // above results in a simple cone.
   Basic::String* tmName;          // Name of our track manager
   TrackManager*  trackManager;    // Our Track manager -- managed by the onboard computer

   //LCreal azimuthBin;            // minimum azimuth we can distinguish -- two signals whose
                                   // azimuth differs by less than this will be merged

   //LCreal elevationBin;          // minimum azimuth we can distinguish -- two signals whose
                                   // azimuth differs by less than this will be merged

   LCreal maximumRange;            // max sensor range.
};

}
}

#endif

