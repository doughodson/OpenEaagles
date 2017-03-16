
#include "openeaagles/interop/hla/Ambassador.hpp"
#include "openeaagles/interop/hla/NetIO.hpp"
#include "openeaagles/interop/hla/Nib.hpp"

#include "openeaagles/models/player/Player.hpp"

#include <iostream>

namespace oe {
namespace hla {

Ambassador::Ambassador(NetIO* netIO): hlaIo(netIO)
{
}

Ambassador::~Ambassador() throw(RTI::FederateInternalError)
{
}

void Ambassador::startRegistrationForObjectClass (
   RTI::ObjectClassHandle   theClass)
throw (
   RTI::ObjectClassNotPublished,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador::startRegistrationForObjectClass(): " ;
   NetIO* netIO = getNetIO();
   int classIndex = netIO->findObjectClassIndex(theClass);
   if (classIndex != 0 && netIO->isObjectClassPublished(classIndex)) {
      // It's an object class that we publish, so we can start to
      // register our objects ...
      //std::cout << classIndex;
      netIO->setObjectClassRegistrationFlag(classIndex,true);
   }
   //std::cout << std::endl;
}

void Ambassador::stopRegistrationForObjectClass (
   RTI::ObjectClassHandle   theClass)
throw (
   RTI::ObjectClassNotPublished,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador::stopRegistrationForObjectClass(): ";
   NetIO* netIO = getNetIO();
   int classIndex = netIO->findObjectClassIndex(theClass);
   if (classIndex != 0 ) {
      //std::cout << classIndex;
      netIO->setObjectClassRegistrationFlag(classIndex,false);
   }
   //std::cout << std::endl;
}

void Ambassador::turnUpdatesOnForObjectInstance (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& theAttributes)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotOwned,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador:turnUpdatesOnForObjectInstance(): theObject = " << theObject;

   // Find the output object
   NetIO* netIO = getNetIO();
   Nib* nib = static_cast<Nib*>(netIO->findNibByObjectHandle(theObject, NetIO::OUTPUT_NIB));

   if (nib != nullptr) nib->turnUpdatesOn(theAttributes);
   //else std::cout << " NOT FOUND";
   //std::cout << std::endl;
}

void Ambassador::turnUpdatesOffForObjectInstance (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& theAttributes)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotOwned,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador:turnUpdatesOffForObjectInstance(): ";

   // Find the output object
   NetIO* netIO = getNetIO();
   Nib* nib = static_cast<Nib*>(netIO->findNibByObjectHandle(theObject, NetIO::OUTPUT_NIB));

   if (nib != nullptr) nib->turnUpdatesOff(theAttributes);
   //std::cout << std::endl;
}

void Ambassador::provideAttributeValueUpdate (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& theAttrs)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::AttributeNotOwned,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador:provideAttributeValueUpdate(): ";
   // Find the output object
   NetIO* netIO = getNetIO();
   Nib* nib = static_cast<Nib*>( netIO->findNibByObjectHandle(theObject, NetIO::OUTPUT_NIB) );

   if (nib != nullptr) nib->provideAttributeValueUpdate(theAttrs);
   //std::cout << std::endl;
}

void Ambassador::discoverObjectInstance (
   RTI::ObjectHandle          theObject,
   RTI::ObjectClassHandle     theObjectClass,
   const char *               theObjectName)
throw (
   RTI::CouldNotDiscover,
   RTI::ObjectClassNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Ambassador::discoverObjectInstance(): " << theObjectName << ", handle = " << theObject << std::endl;
   NetIO* netIO = getNetIO();
   int classIndex = netIO->findObjectClassIndex(theObjectClass);
   if (classIndex != 0 && netIO->isObjectClassSubscribed(classIndex)) {
      // It's an object class that we've subscribed to ...

      // just act as an interface to NetIO, which will handle this.
      getNetIO()->discoverObjectInstance(theObject, theObjectClass, theObjectName);
   }
}

void Ambassador::removeObjectInstance (
        RTI::ObjectHandle          theObject,
  const RTI::FedTime&              theTime,
  const char*                      theTag,
        RTI::EventRetractionHandle theHandle)
throw (
   RTI::ObjectNotKnown,
   RTI::InvalidFederationTime,
   RTI::FederateInternalError)
{
   removeObjectInstance(theObject, theTag);
}

void Ambassador::removeObjectInstance (
        RTI::ObjectHandle          theObject,
  const char*                      theTag)
throw (
   RTI::ObjectNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Ambassador::removeObjectInstance(): remove object = " << theObject << ", theTag=" << theTag << std::endl;

   // find the input object
   NetIO* netIO = getNetIO();
   Nib* nib = netIO->findNibByObjectHandle(theObject, NetIO::INPUT_NIB);
   if (nib != nullptr) {
      // set NIB delete request (Simulation::NetIO::cleanupInputList() should handle this)
      nib->setMode(models::Player::DELETE_REQUEST);
   }
}

void Ambassador::reflectAttributeValues (
        RTI::ObjectHandle                 theObject,
  const RTI::AttributeHandleValuePairSet& theAttrs,
  const RTI::FedTime&                     theTime,
  const char*                             theTag,
        RTI::EventRetractionHandle        theHandle)
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateOwnsAttributes,
  RTI::InvalidFederationTime,
  RTI::FederateInternalError)
{
   reflectAttributeValues(theObject, theAttrs, theTag);
}

void Ambassador::reflectAttributeValues (
        RTI::ObjectHandle                 theObject,
  const RTI::AttributeHandleValuePairSet& theAttrs,
  const char*                             theTag)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::FederateOwnsAttributes,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador::reflectAttributeValues(): object = " << theObject << ", theTag=" << theTag << std::endl;
   // find the input object
   NetIO* netIO = getNetIO();
   Nib* nib = static_cast<Nib*>( netIO->findNibByObjectHandle(theObject, NetIO::INPUT_NIB) );
   if (nib != nullptr) nib->reflectAttributeValues(theAttrs);
}

void Ambassador::turnInteractionsOn (
     RTI::InteractionClassHandle theInteraction)
   throw (
     RTI::InteractionClassNotPublished,
     RTI::FederateInternalError)
{
   std::cout << "Ambassador::turnInteractionsOn(): " ;
   NetIO* netIO = getNetIO();
   int idx = netIO->findInteractionClassIndex(theInteraction);
   if (idx != 0 && netIO->isInteractionClassPublished(idx)) {
      // It's an interaction that we publish, so we can start
      // send new interactions of this class ...
      std::cout << idx;
      netIO->setInteractionEnabledFlag(idx,true);
   }
   std::cout << std::endl;
}

void Ambassador::turnInteractionsOff (
     RTI::InteractionClassHandle theInteraction)
   throw (
     RTI::InteractionClassNotPublished,
     RTI::FederateInternalError)
{
   std::cout << "Ambassador::turnInteractionsOff(): " ;
   NetIO* netIO = getNetIO();
   int idx = netIO->findInteractionClassIndex(theInteraction);
   if (idx != 0) {
      std::cout << idx;
      netIO->setInteractionEnabledFlag(idx,false);
   }
   std::cout << std::endl;
}

void Ambassador::receiveInteraction (
        RTI::InteractionClassHandle       theInteraction,
  const RTI::ParameterHandleValuePairSet& theParameters,
  const RTI::FedTime&                     theTime,
  const char*                             theTag,
        RTI::EventRetractionHandle        theHandle)
throw (
   RTI::InteractionClassNotKnown,
   RTI::InteractionParameterNotKnown,
   RTI::InvalidFederationTime,
   RTI::FederateInternalError)
{
   receiveInteraction(theInteraction, theParameters, theTag);
}

void Ambassador::receiveInteraction (
        RTI::InteractionClassHandle       theInteraction,
  const RTI::ParameterHandleValuePairSet& theParameters,
  const char*)
throw (
   RTI::InteractionClassNotKnown,
   RTI::InteractionParameterNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Ambassador::receiveInteraction(): " << theInteraction << std::endl;
   NetIO* netIO = getNetIO();
   int idx = netIO->findInteractionClassIndex(theInteraction);
   if (idx != 0 && netIO->isInteractionClassSubscribed(idx)) {
      // It's an interaction that we subscribe to, so ...

      // just act as an interface to NetIO, which will handle this.
      getNetIO()->receiveInteraction(theInteraction, theParameters);
   }
}

//------------------------------------------------------------------------------
// Federate Ambassador functions -- Default/null functions
//------------------------------------------------------------------------------

void
Ambassador::synchronizationPointRegistrationSucceeded (
   const char *label)
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled synchronizationPointRegistrationSucceeded()" << std::endl;
}

void
Ambassador::synchronizationPointRegistrationFailed (
   const char *label)
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled synchronizationPointRegistrationFailed()" << std::endl;
}

void
Ambassador::announceSynchronizationPoint (
   const char *label,
   const char *tag)
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled announceSynchronizationPoint()" << std::endl;
}

void
Ambassador::federationSynchronized (
   const char *label)
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled federationSynchronized()" << std::endl;
}

void
Ambassador::initiateFederateSave (
   const char *label)
throw (
   RTI::UnableToPerformSave,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled initiateFederateSave()" << std::endl;
}

void
Ambassador::federationSaved ()
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled federationSaved()" << std::endl;
}

void
Ambassador::federationNotSaved ()
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled federationNotSaved()" << std::endl;
}

void
Ambassador::requestFederationRestoreSucceeded (
   const char *label)
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled requestFederationRestoreSucceeded()" << std::endl;
}

void
Ambassador::requestFederationRestoreFailed (
   const char* label,
   const char* reason)
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled requestFederationRestoreFailed()" << std::endl;
}

void
Ambassador::federationRestoreBegun ()
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled federationRestoreBegun()" << std::endl;
}

void
Ambassador::initiateFederateRestore (
   const char* label,
   RTI::FederateHandle handle)
throw (
   RTI::SpecifiedSaveLabelDoesNotExist,
   RTI::CouldNotRestore,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled initiateFederateRestore()" << std::endl;
}

void
Ambassador::federationRestored ()
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled federationRestored()" << std::endl;
}

void
Ambassador::federationNotRestored ()
throw (
   RTI::FederateInternalError)
{
   std::cout << "Unhandled federationNotRestored()" << std::endl;
}

////////////////////////////////
// Object Management Services //
////////////////////////////////

void
Ambassador::attributesInScope (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& theAttributes)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled attributesInScope()" << std::endl;
}

void
Ambassador::attributesOutOfScope (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& theAttributes)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled attributesOutOfScope()" << std::endl;
}


///////////////////////////////////
// Ownership Management Services //
///////////////////////////////////

void
Ambassador::requestAttributeOwnershipAssumption (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& offeredAttributes,
  const char*                    theTag)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::AttributeAlreadyOwned,
   RTI::AttributeNotPublished,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled requestAttributeOwnershipAssumption()" << std::endl;
}

void
Ambassador::attributeOwnershipDivestitureNotification (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& releasedAttributes)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::AttributeNotOwned,
   RTI::AttributeDivestitureWasNotRequested,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled attributeOwnershipDivestitureNotification()" << std::endl;
}

void
Ambassador::attributeOwnershipAcquisitionNotification (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& securedAttributes)
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeAcquisitionWasNotRequested,
  RTI::AttributeAlreadyOwned,
  RTI::AttributeNotPublished,
  RTI::FederateInternalError)
{
   std::cout << "Unhandled attributeOwnershipAcquisitionNotification()" << std::endl;
}

void
Ambassador::attributeOwnershipUnavailable (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& theAttributes)
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeNotDefined,
  RTI::AttributeAlreadyOwned,
  RTI::AttributeAcquisitionWasNotRequested,
  RTI::FederateInternalError)
{
   std::cout << "Unhandled attributeOwnershipUnavailable()" << std::endl;
}

void
Ambassador::requestAttributeOwnershipRelease (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& candidateAttributes,
  const char*                    theTag)
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeNotOwned,
  RTI::FederateInternalError)
{
   std::cout << "Unhandled requestAttributeOwnershipRelease()" << std::endl;
}

void
Ambassador::confirmAttributeOwnershipAcquisitionCancellation (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& theAttributes)
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeNotDefined,
  RTI::AttributeAlreadyOwned,
  RTI::AttributeAcquisitionWasNotCanceled,
  RTI::FederateInternalError)
{
   std::cout << "Unhandled confirmAttributeOwnershipAcquisitionCancellation()" << std::endl;
}

void
Ambassador::informAttributeOwnership (
   RTI::ObjectHandle    theObject,
   RTI::AttributeHandle theAttribute,
   RTI::FederateHandle  theOwner)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled informAttributeOwnership()" << std::endl;
}

void
Ambassador::attributeIsNotOwned (
   RTI::ObjectHandle    theObject,
   RTI::AttributeHandle theAttribute)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled attributeIsNotOwned()" << std::endl;
}

void
Ambassador::attributeOwnedByRTI (
   RTI::ObjectHandle    theObject,
   RTI::AttributeHandle theAttribute)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled attributeOwnedByRTI()" << std::endl;
}

//////////////////////////////
// Time Management Services //
//////////////////////////////

void
Ambassador::timeRegulationEnabled (
   const RTI::FedTime& theFederateTime)
throw (
   RTI::InvalidFederationTime,
   RTI::EnableTimeRegulationWasNotPending,
   RTI::FederateInternalError)
{
  std::cout << "### timeRegulationEnabled callback";
  hlaIo->setFederationTime(theFederateTime);
  std::cout << "  Federate Time is " << hlaIo->getFederationTime() << std::endl;
}

void
Ambassador::timeConstrainedEnabled (
   const RTI::FedTime& theFederateTime)
throw (
   RTI::InvalidFederationTime,
   RTI::EnableTimeConstrainedWasNotPending,
   RTI::FederateInternalError)
{
  std::cout << "### timeConstrainedEnabled callback";
  hlaIo->setFederationTime(theFederateTime);
  std::cout << "  Federate Time is " << hlaIo->getFederationTime() << std::endl;
}

void
Ambassador::timeAdvanceGrant (
   const RTI::FedTime& theTime)
throw (
   RTI::InvalidFederationTime,
   RTI::TimeAdvanceWasNotInProgress,
   RTI::FederationTimeAlreadyPassed,
   RTI::FederateInternalError)
{
  hlaIo->setTimeAdvanceOutstanding(RTI::RTI_FALSE);
  hlaIo->setFederationTime(theTime);
//  std::cout << "### timeAdvanceGrant callback Time: " << hlaIo->getFederationTime() << endl;
}

void
Ambassador::requestRetraction (
   RTI::EventRetractionHandle theHandle)
throw (
   RTI::EventNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Unhandled requestRetraction()" << std::endl;
}

}
}

