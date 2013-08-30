#include "openeaagles/hla/mc02Fom/Mc02FomIO.h"

#include "openeaagles/hla/mc02Fom/Mc02Fom.h"
#include "openeaagles/hla/mc02Fom/Mc02FomNib.h"
#include "openeaagles/hla/mc02Fom/Mc02FomNibEmitterBeam.h"
#include "openeaagles/hla/Ambassador.h"

#include "openeaagles/simulation/Player.h"
#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/NetHandler.h"


//==============================================================================
// Class: HlaIO
//==============================================================================
namespace Eaagles {
namespace Network {
namespace Hla {
namespace Mc02Fom {

//------------------------------------------------------------------------------
// publishAndSubscribe()
//------------------------------------------------------------------------------
bool Mc02FomIO::publishAndSubscribeEmissions()
{
   RTI::RTIambassador* p = getRTIambassador();
   bool ok = true;

   // ----------
   // Get handles to the class, attributes and parameters
   // ----------
   try {

      // ---
      // Emitter beam class and attribute handles:
      // ---
      {
         RTI::ObjectClassHandle emitterBeamClassHandle = 
            p->getObjectClassHandle(Mc02Fom::EmitterBeam::getClassFedName());

         setObjectClassHandle(EMITTER_BEAM_CLASS, emitterBeamClassHandle );

         setObjectAttributeHandle(
            BEAM_AZIMUTH_CENTER_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getBeamAzimuthCenterAttributeFedName(), 
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            BEAM_AZIMUTH_SWEEP_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getBeamAzimuthSweepAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            BEAM_ELEVATION_CENTER_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getBeamElevationCenterAttributeFedName(), 
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            BEAM_ELEVATION_SWEEP_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getBeamElevationSweepAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            BEAM_FUNCTION_CODE_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getBeamFunctionCodeAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            BEAM_IDENTIFIER_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getBeamIdentifierAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            BEAM_PARAMETER_INDEX_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getBeamParameterIndexAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            EFFECTIVE_RADIATED_POWER_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getEffectiveRadiatedPowerAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            EMISSION_FREQUENCY_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getEmissionFrequencyAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            EMITTER_SYSTEM_IDENTIFIER_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getEmitterSystemIdentifierAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            EVENT_IDENTIFIER_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getEventIdentifierAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            FREQUENCY_RANGE_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getFrequencyRangeAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            PULSE_REPETITION_FREQUENCY_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getPulseRepetitionFrequencyAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            PULSE_WIDTH_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getPulseWidthAttributeFedName(),
               emitterBeamClassHandle)
         );

         setObjectAttributeHandle(
            SWEEP_SYNCH_EB_AI,
            p->getAttributeHandle(
               Mc02Fom::EmitterBeam::getSweepSynchAttributeFedName(),
               emitterBeamClassHandle)
         );

      }

      // ---
      // Jammer beam class handle
      // ---
      setObjectClassHandle(
         JAMMER_BEAM_CLASS,
         p->getObjectClassHandle(Mc02Fom::JammerBeam::getClassFedName())
      );

      // ---
      // RADAR beam class handle
      // ---
      setObjectClassHandle(
         RADAR_BEAM_CLASS,
         p->getObjectClassHandle(Mc02Fom::RadarBeam::getClassFedName())
      );

   }
   catch( RTI::Exception& e) {
      std::cerr << &e << std::endl;
      ok = false;
   }


   // ----------
   // Publish & Subscribe to class attributes
   // ----------
   if (ok) {
      try {

         // General purpuse attribue handle set used to publish and subscribe to class attribues
         RTI::AttributeHandleSet* attrs =
            RTI::AttributeHandleSetFactory::create( NUM_OBJECT_ATTRIBUTES );

         // ---
         // Publish these emission classes and attributes
         // ---
         attrs->empty();
         attrs->add(getObjectAttributeHandle(BEAM_AZIMUTH_CENTER_EB_AI));
         attrs->add(getObjectAttributeHandle(BEAM_AZIMUTH_SWEEP_EB_AI));
         attrs->add(getObjectAttributeHandle(BEAM_ELEVATION_CENTER_EB_AI));
         attrs->add(getObjectAttributeHandle(BEAM_ELEVATION_SWEEP_EB_AI));
         attrs->add(getObjectAttributeHandle(BEAM_FUNCTION_CODE_EB_AI));
         attrs->add(getObjectAttributeHandle(BEAM_IDENTIFIER_EB_AI));
         attrs->add(getObjectAttributeHandle(BEAM_PARAMETER_INDEX_EB_AI));
         attrs->add(getObjectAttributeHandle(EFFECTIVE_RADIATED_POWER_EB_AI));
         attrs->add(getObjectAttributeHandle(EMISSION_FREQUENCY_EB_AI));
         attrs->add(getObjectAttributeHandle(EMITTER_SYSTEM_IDENTIFIER_EB_AI));
         attrs->add(getObjectAttributeHandle(EVENT_IDENTIFIER_EB_AI));
         attrs->add(getObjectAttributeHandle(FREQUENCY_RANGE_EB_AI));
         attrs->add(getObjectAttributeHandle(PULSE_REPETITION_FREQUENCY_EB_AI));
         attrs->add(getObjectAttributeHandle(PULSE_WIDTH_EB_AI));
         attrs->add(getObjectAttributeHandle(SWEEP_SYNCH_EB_AI));

         if (isOutputEnabled()) {
            //p->publishObjectClass(getObjectClassHandle(RADAR_BEAM_CLASS), *attrs);
            setObjectClassPublished(RADAR_BEAM_CLASS,true);
         }

         // ---
         // Subscribe to these emission classes and attributes, and request an update.
         // ---
         if (isInputEnabled()) {
            //  Note: For now we subscribe to the same attribute list that we publish!
            //p->subscribeObjectClassAttributes(getObjectClassHandle(RADAR_BEAM_CLASS), *attrs);
            //p->requestClassAttributeValueUpdate(getObjectClassHandle(RADAR_BEAM_CLASS), *attrs);
            setObjectClassSubscribed(RADAR_BEAM_CLASS, true);
         }

         delete attrs;
      }
      catch (RTI::Exception& e) {
         std::cerr << &e << std::endl;
         ok = false;
      }
   }

   return ok;
}


//==============================================================================
// Class: Mc02Fom::Nib
//==============================================================================

//------------------------------------------------------------------------------
// emitterBeamsManager() 
//    -- (Output) Manages the emitter beam for this NIB(Player)
//------------------------------------------------------------------------------
bool Mc02Fom::Nib::emitterBeamsManager(const LCreal curExecTime)
{
   bool ok = true;
   //if (areEmitterBeamUpdatesRequired(curExecTime)) {
   //   std::cout << "###EM-1" << std::endl;
   // 
   //   // Need to update this entity object ...

   //   Mc02FomIO* mc02IO = (Mc02FomIO*)(getNetworkIO());
   //   RTI::RTIambassador* rtiAmb = mc02IO->getRTIambassador();

   //   // Create the attribute-value pair set
   //   RTI::AttributeHandleValuePairSet* attrs =
   //      RTI::AttributeSetFactory::create( Mc02FomIO::NUM_OBJECT_ATTRIBUTES );

   //   // ------
   //   // For all emitter beams ...
   //   // ------
   //   for (int idx = 0; idx < getNumberEmitterBeams(); idx++) {

   //      // ---
   //      // First, make sure this emitter beam has been registered
   //      // ---
   //      if (!isEmitterBeamRegistered(idx)) {
   //         try {
   //            RTI::ObjectClassHandle theClassHandle =
   //               mc02IO->getObjectClassHandle( getEmitterBeamClassIndex(idx) );

   //            makeEmitterBeamObjectName(idx);

   //            setEmitterBeamObjectHandle(
   //               idx, 
   //               rtiAmb->registerObjectInstance( theClassHandle, getEmitterBeamObjectName(idx) ) 
   //            );

   //            std::cout << "Mc02FomIO::updateEmitterBeams(): emitter beam: ";
   //            std::cout << getEmitterBeamObjectName(idx) << " handle = ";
   //            std::cout << getEmitterBeamObjectHandle(idx) << std::endl;
   //         }
   //         catch (RTI::Exception& e) {
   //            std::cerr << &e << std::endl;
   //            ok = false;
   //         }
   //      }

   //      // ---
   //      // Next, update the emitter beam's attribute values ...
   //      // ---
   //      if ( ok && isEmitterBeamRegistered(idx) && isEmitterBeamAttrsUpdateRequired(idx)) {
   //      std::cout << "Mc02FomIO::updateEmitterBeams(): " << getEmitterBeamObjectName(idx) << std::endl;
   //      try {

   //      // Load the set with updated attribute values
   //      attrs->empty();
   //      getEmitterBeamAttrs(idx,attrs);
   //               
   //      // Send the attributes to the RTI
   //      if ( attrs->size() > 0) {
   //      try  {
   //         if (mc02IO->getRegulating()) {
   //               //DPG rtiAmb->queryLookahead(getLookAhead());
   //               rtiAmb->updateAttributeValues(getEmitterBeamObjectHandle(idx), *attrs, (mc02IO->getFederationTime()+mc02IO->getLookAhead()), 0);
   //         }
   //         else {
   //               rtiAmb->updateAttributeValues(getEmitterBeamObjectHandle(idx), *attrs, 0);
   //         }
   //      }
   //      catch (RTI::Exception& e) {
   //         std::cerr << &e << std::endl;
   //         ok = false;
   //      }
   //      }

   //      }

   //      catch (RTI::Exception& e) {
   //      std::cerr << &e << std::endl;
   //      ok = false;
   //      }
   //      }
   //   }
   //   delete attrs;
   //   std::cout << "###EM-2" << std::endl;
   //}

   return ok;
}

//==============================================================================
// Class: EmitterBeamNib
//==============================================================================

//------------------------------------------------------------------------------
// reflectAttributeValues() -- (Output support)
//------------------------------------------------------------------------------
//bool EmitterBeamNib::makeEmitterBeamObjectName(const int idx)
//{
//    bool ok = false;
//    if (idx >= 0 && idx < getNumberEmitterBeams()) {
//        if (eb[idx] != 0 && getObjectName() != 0) {
//            char cname1[128];
//            // Default name is our entity object's name with "_EBnn" added
//            sprintf(cname1, "%s_EB%d", getObjectName(),idx);
//            ok = setEmitterBeamObjectName(idx,cname1);
//        }
//    }
//    return ok;
//}

//------------------------------------------------------------------------------
// reflectAttributeValues() -- (Input support)
//  Called by our FederateAmbassador to update the attribute values for
//  this object instance.  (Also handles the network to host byte swapping)
//------------------------------------------------------------------------------
void EmitterBeamNib::reflectAttributeValues(const RTI::AttributeHandleValuePairSet& theAttrs)
{
    //Mc02FomIO* mc02IO = (Mc02FomIO*)(getNetworkIO());
    //if (mc02IO != 0 && emitterBeam != 0) {
    //
    //    // PhysicalEntity pointer
    //    //Mc02Fom::PhysicalEntity* physicalEntity = dynamic_cast<Mc02Fom::PhysicalEntity*>(baseEntity);

    //    RTI::ULong length;
    //    char netBuffer[1000];
    //    for (RTI::ULong j = 0 ; j < theAttrs.size(); j++ ) {
    //        
    //        // get the attribute's handle and data (network byte order)
    //        RTI::AttributeHandle theAttr = theAttrs.getHandle(j);
    //        theAttrs.getValue(j, netBuffer, length);

    //        // Process the attribute
    //        switch ( mc02IO->findAttributeIndex(theAttr) ) {
    //        
    //            case Mc02FomIO::BEAM_AZIMUTH_CENTER_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::BEAM_AZIMUTH_CENTER_EB_AI,true);
    //                //std::cout << "Recv beam azimuth center: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::BEAM_AZIMUTH_SWEEP_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::BEAM_AZIMUTH_SWEEP_EB_AI,true);
    //                //std::cout << "Recv beam azimuth sweep: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //                
    //            case Mc02FomIO::BEAM_ELEVATION_CENTER_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::BEAM_ELEVATION_CENTER_EB_AI,true);
    //                //std::cout << "Recv beam elevation center: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::BEAM_ELEVATION_SWEEP_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::BEAM_ELEVATION_SWEEP_EB_AI,true);
    //                //std::cout << "Recv beam elevation sweep: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::BEAM_FUNCTION_CODE_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::BEAM_FUNCTION_CODE_EB_AI,true);
    //                //std::cout << "Recv beam function code: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::BEAM_IDENTIFIER_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::BEAM_IDENTIFIER_EB_AI,true);
    //                //std::cout << "Recv XXXX: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::BEAM_PARAMETER_INDEX_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::BEAM_PARAMETER_INDEX_EB_AI,true);
    //                //std::cout << "Recv beam parameter index: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::EFFECTIVE_RADIATED_POWER_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::EFFECTIVE_RADIATED_POWER_EB_AI,true);
    //                //std::cout << "Recv effective radiated power: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::EMISSION_FREQUENCY_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::EMISSION_FREQUENCY_EB_AI,true);
    //                //std::cout << "Recv emission frequency: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::EMITTER_SYSTEM_IDENTIFIER_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::EMITTER_SYSTEM_IDENTIFIER_EB_AI,true);
    //                //std::cout << "Recv emitter system identifier: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::EVENT_IDENTIFIER_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::EVENT_IDENTIFIER_EB_AI,true);
    //                //std::cout << "Recv event identifier: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::FREQUENCY_RANGE_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::FREQUENCY_RANGE_EB_AI,true);
    //                //std::cout << "Recv frequency range: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::PULSE_REPETITION_FREQUENCY_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::PULSE_REPETITION_FREQUENCY_EB_AI,true);
    //                //std::cout << "Recv pulse repetition frequency: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::PULSE_WIDTH_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::PULSE_WIDTH_EB_AI,true);
    //                //std::cout << "Recv pulse width: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //        
    //            case Mc02FomIO::SWEEP_SYNCH_EB_AI : {
    //                // Attribute stuff here ...

    //                setAttributeUpdateRequiredFlag(Mc02FomIO::SWEEP_SYNCH_EB_AI,true);
    //                //std::cout << "Recv sweep synch: (" << baseEntity->entityIdentifier.entityNumber << "," << baseEntity->entityIdentifier.federateIdentifier.applicationID << "," << baseEntity->entityIdentifier.federateIdentifier.siteID << ")" << std::endl;
    //            }
    //            break;
    //
    //        }
    //    }
    //        
    //    // Update the basic NIB state data with this new data
    //    //entityState2Nib();
    //}
}

} // End Mc02Fom namespace
} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
