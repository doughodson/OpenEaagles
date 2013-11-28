//==============================================================================
// Class: MergingIrSensor
//==============================================================================

#include "openeaagles/simulation/MergingIrSensor.h"

#include "openeaagles/simulation/IrAtmosphere.h"
#include "openeaagles/simulation/IrSeeker.h"
#include "openeaagles/simulation/AngleOnlyTrackManager.h"
#include "openeaagles/simulation/IrQueryMsg.h"
#include "openeaagles/simulation/OnboardComputer.h"
#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Integer.h"
#include "openeaagles/basic/units/Distances.h"
#include "openeaagles/basic/units/Angles.h"

namespace Eaagles {
namespace Simulation {

IMPLEMENT_SUBCLASS(MergingIrSensor, "MergingIrSensor")
EMPTY_SERIALIZER(MergingIrSensor)
EMPTY_DELETEDATA(MergingIrSensor)

// Slot table
BEGIN_SLOTTABLE(MergingIrSensor)
   "azimuthBin",        // 1: azimuthBin
   "elevationBin",      // 2: elevationBin 
END_SLOTTABLE(MergingIrSensor)

//  Map slot table 
BEGIN_SLOT_MAP(MergingIrSensor)
   ON_SLOT(1,setSlotAzimuthBin,Basic::Number)
   ON_SLOT(2,setSlotElevationBin,Basic::Number)
END_SLOT_MAP()


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
MergingIrSensor::MergingIrSensor()
{
   STANDARD_CONSTRUCTOR()
   azimuthBin = 0.0f;
   elevationBin = 0.0f;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void MergingIrSensor::copyData(const MergingIrSensor& org, const bool)
{
   BaseClass::copyData(org);
   azimuthBin = org.azimuthBin;
   elevationBin = org.elevationBin;
}


//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void MergingIrSensor::reset()
{
   BaseClass::reset();

   // check the type of our track manager - we need to use the AirAngleOnlyTrkMgrPT
   if (getTrackManager() != 0) {
      const AirAngleOnlyTrkMgrPT* a = dynamic_cast<const AirAngleOnlyTrkMgrPT*>(getTrackManager());
      if (a==0) {
         if (isMessageEnabled(MSG_WARNING)) {
            std::cerr << "MergingIrSensor::reset() : track manager is not an AirAngleOnlyTrkMgrPT" << std::endl;
         }
      }
   }
}

void MergingIrSensor::receive(const LCreal dt)
{
   BaseClass::receive(dt);
   mergeIrReturns();
}

void MergingIrSensor::mergeIrReturns()
{ 
   int numRecords = storedMessagesQueue.entries();
   if (numRecords > 0) {

      //int* deleteArray = new int [numRecords]; 
      //if (deleteArray == 0) {
      //   if (isMessageEnabled(MSG_ERROR)) {
      //      std::cerr << "Error: Allocation memory failure in IrSensor::mergeIrReturns" << std::endl; 
      //   }
      //}
      //else { 
         //for (int i=0; i < numRecords; i++) {
         //   deleteArray[i] = 0; 
         //}

         lcLock(storedMessagesLock);
         // Traverse the stored message queue using peek (). Examine every 
         // message. Compare every stored message against every OTHER stored 
         // message. If the two are too close together, merge the two signals 
         // and mark the second message in the delete array.  
         // Proceed through the loop, ignoring all messages marked "deleted" 
         // in the delete array. 
         numRecords = storedMessagesQueue.entries();

         if (isMessageEnabled(MSG_DEBUG)) {
            std::cout << "IrSensor: numRecords returned " << numRecords << std::endl;
         }

         for (int i=0; i < numRecords; i++) {
               //if (deleteArray[i] == 0) {  // Do not bother processing those marked 
               // for deletion -- these have already been
               // merged and must be ignored. 

            IrQueryMsg* currentMsg = storedMessagesQueue.peek0(i); 

            // Do not bother processing those marked 
            // for deletion -- these have already been
            // merged and must be ignored. 
            if (currentMsg->getQueryMergeStatus()!= IrQueryMsg::MERGED_OUT) {

               for (int j = i+1; j < numRecords; j++) {

                  IrQueryMsg* nextMsg = storedMessagesQueue.peek0(j);
                  LCreal azimuthDelta = currentMsg->getRelativeAzimuth() - nextMsg->getRelativeAzimuth();
                  LCreal elevationDelta = currentMsg->getRelativeElevation() 
                     - nextMsg->getRelativeElevation();

                  if (azimuthDelta < 0) 
                     azimuthDelta = -azimuthDelta; 

                  if (elevationDelta < 0) 
                     elevationDelta = -elevationDelta; 

                  if ((azimuthDelta < azimuthBin) && 
                     (elevationDelta < elevationBin)) { // two signals are too close together
                    // for the sensor to distinguish between them; 
                    // we will merge the two signals based 
                    // on their weighted signal-to-noise. 
                    LCreal currentRatio = 0.0f;
                    LCreal nextRatio = 0.0f;

                    // find current ratio. 
                    if (isMessageEnabled(MSG_DEBUG)) {
                     std::cout << "IrSensor: merging target " <<  nextMsg->getTarget()->getName()->getString() 
                       << " into target " <<currentMsg->getTarget()->getName()->getString()  << std::endl;
                    }

                    if (currentMsg->getSignalToNoiseRatio() > 
                       currentMsg->getBackgroundNoiseRatio()) {

                          currentRatio = currentMsg->getSignalToNoiseRatio() +
                             currentMsg->getBackgroundNoiseRatio();

                    } else {
                       if (currentMsg->getSignalToNoiseRatio() < 0) {
                          currentRatio = -currentMsg->getSignalToNoiseRatio() -
                             currentMsg->getBackgroundNoiseRatio();
                       } else { 
                          currentRatio = -currentMsg->getSignalToNoiseRatio() -
                             currentMsg->getBackgroundNoiseRatio();
                       } // signaltonoise < 0 

                    } // if current signal > background 

                    //now do the same thing for the next message. 
                    if (nextMsg->getSignalToNoiseRatio() > 
                       nextMsg->getBackgroundNoiseRatio()) {
                          nextRatio = nextMsg->getSignalToNoiseRatio() +
                             nextMsg->getBackgroundNoiseRatio();
                    } else {
                       if (nextMsg->getSignalToNoiseRatio() < 0) {
                          nextRatio = -nextMsg->getSignalToNoiseRatio() -
                             nextMsg->getBackgroundNoiseRatio();
                       } else { 
                          nextRatio = -nextMsg->getSignalToNoiseRatio() -
                             nextMsg->getBackgroundNoiseRatio();
                       } // signaltonoise < 0 

                    } // if next signal > background 

                    // use ratios to find weights. 
                    LCreal sumRatio = currentRatio + nextRatio; 

                    LCreal currentWeight = currentRatio / sumRatio;
                    LCreal nextWeight = (LCreal) 1.0 - currentWeight; 

                    //combine line-of-sight vector using weights
                    currentMsg->setLosVec((currentMsg->getLosVec() * currentWeight) +
                       (nextMsg->getLosVec() * nextWeight));

                    // combine position 
                    currentMsg->setPosVec((currentMsg->getPosVec() * currentWeight) +
                       (nextMsg->getPosVec() * nextWeight));

                    // combine velocity
                    currentMsg->setVelocityVec((currentMsg->getVelocityVec() * currentWeight) +
                       (nextMsg->getVelocityVec() * nextWeight));

                    // combine acceleration 
                    currentMsg->setAccelVec((currentMsg->getAccelVec() * currentWeight) +
                       (nextMsg->getAccelVec() * nextWeight));

                    // combine signal to noise ratios. 
                    sumRatio = sumRatio - currentMsg->getBackgroundNoiseRatio();
                    if (sumRatio < 0) 
                       sumRatio = -sumRatio;

                    currentMsg->setSignalToNoiseRatio(sumRatio);

                    //combine Azimuth and Elevation. 
                    currentMsg->setAzimuthAoi((currentMsg->getAzimuthAoi() * currentWeight) +
                       nextMsg->getAzimuthAoi() * nextWeight);

                    currentMsg->setElevationAoi((currentMsg->getElevationAoi()* currentWeight) +
                       (nextMsg->getElevationAoi() * nextWeight));

                    currentMsg->setAngleAspect((currentMsg->getAngleAspect() * currentWeight) +
                       (nextMsg->getAngleAspect() * nextWeight));

                    currentMsg->setRelativeAzimuth((currentMsg->getRelativeAzimuth() * currentWeight) +
                       (nextMsg->getRelativeAzimuth() * nextWeight));

                    currentMsg->setRelativeElevation((currentMsg->getRelativeElevation() * currentWeight) +
                       (nextMsg->getRelativeElevation() * nextWeight));

                    // signal that this report has merged targets
                    currentMsg->setQueryMergeStatus(IrQueryMsg::MERGED);
                    nextMsg->setQueryMergeStatus(IrQueryMsg::MERGED_OUT);

                    //deleteArray[j] = 1;  // now that we have merged this signal with the 
                    // Ith signal, it must be deleted. It will not 
                    // be passed on to the track manager. 

                    //if (isMessageEnabled(MSG_INFO)) {
                    //std::cout << "IrSensor: End Merge" << std::endl;
                    //}

                  } // if we merge 
               } // end for j = i + 1; 
            } // End if delete Array
            else { // debug - this target ws merged into another
               int x=0;
               x=x+1;
            }
         } // end for i = 0; 
         lcUnlock(storedMessagesLock);
         //delete[] deleteArray;
      //} // newArray is not null. 
   } // numRecords > 0
}

//------------------------------------------------------------------------------
// getSlotByIndex()
//------------------------------------------------------------------------------
Basic::Object* MergingIrSensor::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

// setAzimuthBin() - Sets the lower Azimuth Bin 
bool MergingIrSensor::setAzimuthBin(const LCreal w)
{ 
   azimuthBin = w;
   return true;
}

// setElevationBin() - Sets the lower Elevation Bin 
bool MergingIrSensor::setElevationBin(const LCreal w)
{
   elevationBin = w;
   return true;
}
 
bool MergingIrSensor::setSlotAzimuthBin(const Basic::Number* const msg)
{
   LCreal value = 0.0f;

   const Basic::Angle* a = dynamic_cast<const Basic::Angle*>(msg);
   if (a != 0) {
       Basic::Radians r;
       value = (LCreal)r.convert(*a);
   }
   else if (msg != 0) {
      value = msg->getReal();
   }
   setAzimuthBin(value);
   return true;
}

bool MergingIrSensor::setSlotElevationBin(const Basic::Number* const msg)
{
   LCreal value = 0.0f;
 
   const Basic::Angle* a = dynamic_cast<const Basic::Angle*>(msg);
   if (a != 0) {
       Basic::Radians r;
       value = (LCreal)r.convert(*a);
   }
   else if (msg != 0) {
      value = msg->getReal();
   }
   setElevationBin(value);
   return true;
}

}
}

