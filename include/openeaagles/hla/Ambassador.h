//------------------------------------------------------------------------------
// Class:	Hla::Ambassador
//------------------------------------------------------------------------------
#ifndef __Hla_Ambassador_H_B8FDDD29_2276_4fee_BE81_EEE8BF866F31__
#define __Hla_Ambassador_H_B8FDDD29_2276_4fee_BE81_EEE8BF866F31__

#include <RTI.hh>
#include <fedtime.hh>

namespace Eaagles {
namespace Network {
namespace Hla {

class NetIO;

//------------------------------------------------------------------------------
// Class:	Hla::Ambassador
// Base class:	RTI::FederateAmbassador -> Hla::Ambassador
//
// Description:	Basic ambassador to the Federation.  Our federate unqiue
//              ambassadors will be derived from this class.  (Notes on
//              parameter passing are in <federateAmbServices.hh>, which
//              is included in <RTI.hh>)
//
//
// Public member functions:
//
//      Hla::NetIO* getNetIO()
//          Returns a pointer to the HLA Network I/O class that is controlling
//          this ambassador.
//
//------------------------------------------------------------------------------
class Ambassador : public RTI::FederateAmbassador {

public:
   Ambassador(NetIO* nio);
   virtual ~Ambassador() throw(RTI::FederateInternalError);
    
   NetIO* getNetIO()               { return hlaIo; }

private:
   NetIO*         hlaIo;         // Our controlling NeIO

// ---
// -- Standard FederateAmbassador interface --
// ---
public:
   // 4.7
   virtual void synchronizationPointRegistrationSucceeded (
     const char *label) // supplied C4
   throw (
      RTI::FederateInternalError);

   virtual void synchronizationPointRegistrationFailed (
     const char *label) // supplied C4
   throw (
      RTI::FederateInternalError);

   // 4.8
   virtual void announceSynchronizationPoint (
     const char *label, // supplied C4
     const char *tag)   // supplied C4
   throw (
      RTI::FederateInternalError);

   // 4.10
   virtual void federationSynchronized (
     const char *label) // supplied C4)
   throw (
      RTI::FederateInternalError);

   // 4.12
   virtual void initiateFederateSave (
     const char *label) // supplied C4
   throw (
     RTI::UnableToPerformSave,
     RTI::FederateInternalError);

   // 4.15
   virtual void federationSaved ()
   throw (
      RTI::FederateInternalError);

   virtual void federationNotSaved ()
   throw (
      RTI::FederateInternalError);

   // 4.17
   virtual void requestFederationRestoreSucceeded (
     const char *label) // supplied C4
   throw (
      RTI::FederateInternalError);

   virtual void requestFederationRestoreFailed (
     const char *label,  // supplied C4
     const char *reason) // supplied C4
   throw (
      RTI::FederateInternalError);

   // 4.18
   virtual void federationRestoreBegun ()
   throw (
      RTI::FederateInternalError);

   // 4.19
   virtual void initiateFederateRestore (
     const char *label,   // supplied C4
           RTI::FederateHandle handle)  // supplied C1
   throw (
     RTI::SpecifiedSaveLabelDoesNotExist,
     RTI::CouldNotRestore,
     RTI::FederateInternalError);

   // 4.21
   virtual void federationRestored ()
   throw (
      RTI::FederateInternalError);

   virtual void federationNotRestored ()
   throw (
      RTI::FederateInternalError);

   /////////////////////////////////////
   // Declaration Management Services //
   /////////////////////////////////////

   // 5.10
   virtual void startRegistrationForObjectClass (
           RTI::ObjectClassHandle   theClass)      // supplied C1
   throw (
     RTI::ObjectClassNotPublished,
     RTI::FederateInternalError);

   // 5.11
   virtual void stopRegistrationForObjectClass (
           RTI::ObjectClassHandle   theClass)      // supplied C1
   throw (
     RTI::ObjectClassNotPublished,
     RTI::FederateInternalError);

   // 5.12
   virtual void turnInteractionsOn (
     RTI::InteractionClassHandle theHandle) // supplied C1
   throw (
     RTI::InteractionClassNotPublished,
     RTI::FederateInternalError);

   // 5.13
   virtual void turnInteractionsOff (
     RTI::InteractionClassHandle theHandle) // supplied C1
   throw (
     RTI::InteractionClassNotPublished,
     RTI::FederateInternalError);

   ////////////////////////////////
   // Object Management Services //
   ////////////////////////////////

   // 6.3
   virtual void discoverObjectInstance (
           RTI::ObjectHandle          theObject,      // supplied C1
           RTI::ObjectClassHandle     theObjectClass, // supplied C1
     const char *                theObjectName)  // supplied C4
   throw (
     RTI::CouldNotDiscover,
     RTI::ObjectClassNotKnown,
     RTI::FederateInternalError);

   // 6.5
   virtual void reflectAttributeValues (
           RTI::ObjectHandle                 theObject,     // supplied C1
     const RTI::AttributeHandleValuePairSet& theAttributes, // supplied C4
     const RTI::FedTime&                     theTime,       // supplied C1
     const char                        *theTag,        // supplied C4
           RTI::EventRetractionHandle        theHandle)     // supplied C1
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::FederateOwnsAttributes,
     RTI::InvalidFederationTime,
     RTI::FederateInternalError);

   virtual void reflectAttributeValues (
           RTI::ObjectHandle                 theObject,     // supplied C1
     const RTI::AttributeHandleValuePairSet& theAttributes, // supplied C4
     const char                        *theTag)        // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::FederateOwnsAttributes,
     RTI::FederateInternalError);

   // 6.7
   virtual void receiveInteraction (
           RTI::InteractionClassHandle       theInteraction, // supplied C1
     const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
     const RTI::FedTime&                     theTime,        // supplied C4
     const char                        *theTag,         // supplied C4
           RTI::EventRetractionHandle        theHandle)      // supplied C1
   throw (
     RTI::InteractionClassNotKnown,
     RTI::InteractionParameterNotKnown,
     RTI::InvalidFederationTime,
     RTI::FederateInternalError);

   virtual void receiveInteraction (
           RTI::InteractionClassHandle       theInteraction, // supplied C1
     const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
     const char                        *theTag)         // supplied C4
   throw (
     RTI::InteractionClassNotKnown,
     RTI::InteractionParameterNotKnown,
     RTI::FederateInternalError);

   // 6.9
   virtual void removeObjectInstance (
           RTI::ObjectHandle          theObject, // supplied C1
     const RTI::FedTime&              theTime,   // supplied C4
     const char                 *theTag,    // supplied C4
           RTI::EventRetractionHandle theHandle) // supplied C1
   throw (
     RTI::ObjectNotKnown,
     RTI::InvalidFederationTime,
     RTI::FederateInternalError);

   virtual void removeObjectInstance (
           RTI::ObjectHandle          theObject, // supplied C1
     const char                 *theTag)    // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::FederateInternalError);

   // 6.13
   virtual void attributesInScope (
           RTI::ObjectHandle        theObject,     // supplied C1
     const RTI::AttributeHandleSet& theAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::FederateInternalError);

   // 6.14
   virtual void attributesOutOfScope (
           RTI::ObjectHandle        theObject,     // supplied C1
     const RTI::AttributeHandleSet& theAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::FederateInternalError);

   // 6.16
   virtual void provideAttributeValueUpdate (
           RTI::ObjectHandle        theObject,     // supplied C1
     const RTI::AttributeHandleSet& theAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::AttributeNotOwned,
     RTI::FederateInternalError);

   // 6.17
   virtual void turnUpdatesOnForObjectInstance (
           RTI::ObjectHandle        theObject,     // supplied C1
     const RTI::AttributeHandleSet& theAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotOwned,
     RTI::FederateInternalError);

   // 6.18
   virtual void turnUpdatesOffForObjectInstance (
           RTI::ObjectHandle        theObject,      // supplied C1
     const RTI::AttributeHandleSet& theAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotOwned,
     RTI::FederateInternalError);

   ///////////////////////////////////
   // Ownership Management Services //
   ///////////////////////////////////

   // 7.4
   virtual void requestAttributeOwnershipAssumption (
           RTI::ObjectHandle        theObject,         // supplied C1
     const RTI::AttributeHandleSet& offeredAttributes, // supplied C4
     const char               *theTag)            // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::AttributeAlreadyOwned,
     RTI::AttributeNotPublished,
     RTI::FederateInternalError);

   // 7.5
   virtual void attributeOwnershipDivestitureNotification (
           RTI::ObjectHandle        theObject,          // supplied C1
     const RTI::AttributeHandleSet& releasedAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::AttributeNotOwned,
     RTI::AttributeDivestitureWasNotRequested,
     RTI::FederateInternalError);

   // 7.6
   virtual void attributeOwnershipAcquisitionNotification (
           RTI::ObjectHandle        theObject,         // supplied C1
     const RTI::AttributeHandleSet& securedAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::AttributeAcquisitionWasNotRequested,
     RTI::AttributeAlreadyOwned,
     RTI::AttributeNotPublished,
     RTI::FederateInternalError);

   // 7.9
   virtual void attributeOwnershipUnavailable (
           RTI::ObjectHandle        theObject,         // supplied C1
     const RTI::AttributeHandleSet& theAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::AttributeNotDefined,
     RTI::AttributeAlreadyOwned,
     RTI::AttributeAcquisitionWasNotRequested,
     RTI::FederateInternalError);

   // 7.10
   virtual void requestAttributeOwnershipRelease (
           RTI::ObjectHandle        theObject,           // supplied C1
     const RTI::AttributeHandleSet& candidateAttributes, // supplied C4
     const char               *theTag)              // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::AttributeNotOwned,
     RTI::FederateInternalError);

   // 7.14
   virtual void confirmAttributeOwnershipAcquisitionCancellation (
           RTI::ObjectHandle        theObject,         // supplied C1
     const RTI::AttributeHandleSet& theAttributes) // supplied C4
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::AttributeNotDefined,
     RTI::AttributeAlreadyOwned,
     RTI::AttributeAcquisitionWasNotCanceled,
     RTI::FederateInternalError);

   // 7.16
   virtual void informAttributeOwnership (
     RTI::ObjectHandle    theObject,    // supplied C1
     RTI::AttributeHandle theAttribute, // supplied C1
     RTI::FederateHandle  theOwner)     // supplied C1
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::FederateInternalError);

   virtual void attributeIsNotOwned (
     RTI::ObjectHandle    theObject,    // supplied C1
     RTI::AttributeHandle theAttribute) // supplied C1
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::FederateInternalError);

   virtual void attributeOwnedByRTI (
     RTI::ObjectHandle    theObject,    // supplied C1
     RTI::AttributeHandle theAttribute) // supplied C1
   throw (
     RTI::ObjectNotKnown,
     RTI::AttributeNotKnown,
     RTI::FederateInternalError);

   //////////////////////////////
   // Time Management Services //
   //////////////////////////////

   // 8.3
   virtual void timeRegulationEnabled (
    const  RTI::FedTime& theFederateTime) // supplied C4
   throw (
     RTI::InvalidFederationTime,
     RTI::EnableTimeRegulationWasNotPending,
     RTI::FederateInternalError);

   // 8.6
   virtual void timeConstrainedEnabled (
     const RTI::FedTime& theFederateTime) // supplied C4
   throw (
     RTI::InvalidFederationTime,
     RTI::EnableTimeConstrainedWasNotPending,
     RTI::FederateInternalError);

   // 8.13
   virtual void timeAdvanceGrant (
     const RTI::FedTime& theTime) // supplied C4
   throw (
     RTI::InvalidFederationTime,
     RTI::TimeAdvanceWasNotInProgress,
     RTI::FederationTimeAlreadyPassed,
     RTI::FederateInternalError);

   // 8.22
   virtual void requestRetraction (
     RTI::EventRetractionHandle theHandle) // supplied C1
   throw (
     RTI::EventNotKnown,
     RTI::FederateInternalError);

};

} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif  /* __Hla_Ambassador_H_B8FDDD29_2276_4fee_BE81_EEE8BF866F31__ */
