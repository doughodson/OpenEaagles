//------------------------------------------------------------------------------
// Class: IrQueryMsg
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_IrQueryMsg_H__
#define __Eaagles_Simulation_IrQueryMsg_H__

#include "openeaagles/simulation/SensorMsg.h"

namespace Eaagles {

namespace Basic { class List; }

namespace Simulation {

class IrSensor;

//------------------------------------------------------------------------------
// Class: IrQueryMsg
//
// Description: General IR query message -- Used to request an IR Signature
//
// Factory name: IrQueryMsg
//
//------------------------------------------------------------------------------
class IrQueryMsg : public SensorMsg
{
    DECLARE_SUBCLASS(IrQueryMsg, SensorMsg)

public:
   IrQueryMsg();

      // Angle of target off gimbal boresight
   const LCreal getAngleOffBoresight() const { return angleOffBoresight; }

   // Set angle off gimbal boresight
   void setAngleOffBoresight(const LCreal aob) { angleOffBoresight = aob; }

   // angleAspect to target
   const LCreal getAngleAspect() const { return angleAspect; }

   // Set relative azimuth
   void setRelativeAzimuth(const LCreal & n) { raz = n; }

   // Relative azimuth of target
   const LCreal getRelativeAzimuth() const { return raz; }

   // Set relative azimuth
   void setRelativeElevation(const LCreal & n) { rel = n; }

   // Relative azimuth of target
   const LCreal getRelativeElevation() const { return rel; }

   // Set angle aspect.
   void setAngleAspect(const LCreal & n) { angleAspect = n; }

   // Position of target player
   const osg::Vec3& getPosVec() const { return pos; }

   // Sets the target position
   void setPosVec(const osg::Vec3& v) { pos = v; }

   // Velocity of target player
   const osg::Vec3& getVelocityVec() const { return vel; }

   // Sets the target velocity
   void setVelocityVec(const osg::Vec3& v) { vel = v; }

   // acceleration of target player
   const osg::Vec3& getAccelVec() const { return accel; }

   // Sets the target acceleration
   void setAccelVec(const osg::Vec3& v) { accel = v; }

   // Lower wavelength  (microns)
   LCreal getLowerWavelength() const {return lowerWavelength;}

   //Set the lower wavelength (microns)
   void setLowerWavelength(const LCreal lw) {lowerWavelength = lw; }

   // Upper wavelength  (microns)
   LCreal getUpperWavelength() const {return upperWavelength; }

   //Set the upper wavelength (microns)
   void setUpperWavelength(const LCreal uw) {upperWavelength = uw; }

   //Instantaneous Field of View
   LCreal getInstantaneousFieldOfView() const {return instantaneousFieldOfView; }

   //Set the instantaneous Field of View
   void setInstantaneousFieldOfView(const LCreal ifov) {instantaneousFieldOfView = ifov; }

   // Noise Equivalent Irradiance
   LCreal getNEI() const {return nei;}

   // Set the Noise Equivalent Irradiance
   void setNEI(const LCreal n) {nei = n; }

   //Signature at Range
   LCreal getSignatureAtRange() const {return signatureAtRange; }

   //Set the Signature at Range
   void setSignatureAtRange (const LCreal sig) {signatureAtRange = sig; }

   //Signature by waveband
   const LCreal* getSignatureByWaveband() const {return signatureByWaveband; }

   //Set the signature by waveband
   void setSignatureByWaveband (LCreal* const sig);

   //Transmissivity
   LCreal getEmissivity() const {return emissivity; }

   //Set the transmissivity
   void setEmissivity (const LCreal emiss) {emissivity = emiss; }

   //Projected Area in field of view
   LCreal getProjectedArea() const {return projectedArea; }

   //Set the projected area in the field of view
   void setProjectedArea(const LCreal area) {projectedArea = area; }

   LCreal getSignalToNoiseRatio() const {return signalToNoiseRatio;}
   void setSignalToNoiseRatio (LCreal const newSN) {signalToNoiseRatio = newSN; }

   LCreal getBackgroundNoiseRatio() const {return backgroundNoiseRatio;}
   void setBackgroundNoiseRatio (LCreal const newSN) {backgroundNoiseRatio = newSN; }

   IrSensor* getSendingSensor() {return sendingSensor;}
   const IrSensor* getSendingSensor() const {return sendingSensor;}
   void setSendingSensor(IrSensor* const newSensor);

   // IrQueryMsg class interface
   virtual void clearIrSignature();

   void setRange(const LCreal r) override;
   void clear() override;

   // FAB - valuable to keep info about merging
   enum MergedQueryStatus {
      NOT_MERGED,     // 0 = this query not involved in a merging
      MERGED,         // 1 = one or more other queries merged into this query
      MERGED_OUT      // 2 = this query merged into another query
   };

   MergedQueryStatus getQueryMergeStatus() const { return mergedQueryStatus; }
   void setQueryMergeStatus(MergedQueryStatus status) { mergedQueryStatus = status; }

private:

   LCreal         lowerWavelength;           //Lower wavelength          (microns)
   LCreal         upperWavelength;           //Upper wavelength          (microns)
   LCreal         instantaneousFieldOfView;  //IFOV                      (steradians)
   LCreal         nei;                       //NEI
   LCreal         signatureAtRange;          //Transmitted signal
   LCreal* signatureByWaveband;              //Transmitted signal by waveband  2d array where x = number of
                                             // waveband bins, y = 0 (lower wavelength) , 1 (upper wavelength), 2 (sig)
   LCreal         emissivity;                //Emissivity (unitless)
   LCreal         projectedArea;             //Projected area observed   (m^2)
   LCreal         signalToNoiseRatio;        // ratio of Signal to Noise
   LCreal         backgroundNoiseRatio;      // ratio of background noise
   IrSensor*      sendingSensor;             //sensor that reported this message.
   osg::Vec3      pos;                       // position of the target.
   osg::Vec3      vel;                       // velocity of target.
   osg::Vec3      accel;                     // acceleration of target.
   LCreal         angleAspect;               // Angle aspect to target.
   LCreal         angleOffBoresight;         // Angle of target off boresight of sensor
   LCreal         rel;                       // The relative elevation from target to sensor
   LCreal         raz;                       // The relative azimuth from target to sensor
   MergedQueryStatus mergedQueryStatus;      // Flags status of query merging   // FAB
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
