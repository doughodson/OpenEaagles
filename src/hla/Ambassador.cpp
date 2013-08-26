#include "eaagles/hla/Ambassador.h"
#include "eaagles/hla/HlaIO.h"
#include "eaagles/hla/Nib.h"

using namespace std;

namespace Eaagles {
namespace Network {
namespace Hla {

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Ambassador::Ambassador(HlaIO* n)
{
   hlaIo = n;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
Ambassador::~Ambassador() throw(RTI::FederateInternalError)
{
}

//------------------------------------------------------------------------------
// startRegistrationForObjectClass
//------------------------------------------------------------------------------
void Ambassador::startRegistrationForObjectClass (
   RTI::ObjectClassHandle   theClass)
throw (
   RTI::ObjectClassNotPublished,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador::startRegistrationForObjectClass(): " ;
   HlaIO* netIO = getHlaIO();
   int classIndex = netIO->findObjectClassIndex(theClass);
   if (classIndex != 0 && netIO->isObjectClassPublished(classIndex)) {
      // It's an object class that we publish, so we can start to
      // register our objects ...
      //std::cout << classIndex; 
      netIO->setObjectClassRegistrationFlag(classIndex,true);
   }
   //std::cout << std::endl; 
}

//------------------------------------------------------------------------------
// stopRegistrationForObjectClass
//------------------------------------------------------------------------------
void Ambassador::stopRegistrationForObjectClass (
   RTI::ObjectClassHandle   theClass)
throw (
   RTI::ObjectClassNotPublished,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador::stopRegistrationForObjectClass(): ";
   HlaIO* netIO = getHlaIO();
   int classIndex = netIO->findObjectClassIndex(theClass);
   if (classIndex != 0 ) {
      //std::cout << classIndex;
      netIO->setObjectClassRegistrationFlag(classIndex,false);
   }
   //std::cout << std::endl; 
}

//------------------------------------------------------------------------------
// turnUpdatesOnForObjectInstance
//------------------------------------------------------------------------------
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
   Simulation:HlaIO* netIO = getHlaIO();
   Nib* nib = (Nib*)(netIO->findNibByObjectHandle(theObject, HlaIO::OUTPUT_NIB));

   if (nib != 0) nib->turnUpdatesOn(theAttributes);
   //else std::cout << " NOT FOUND";
   //std::cout << std::endl;
}

//------------------------------------------------------------------------------
// turnUpdatesOffForObjectInstance
//------------------------------------------------------------------------------
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
   HlaIO* netIO = getHlaIO();
   Nib* nib = (Nib*)(netIO->findNibByObjectHandle(theObject, HlaIO::OUTPUT_NIB));

   if (nib != 0) nib->turnUpdatesOff(theAttributes);
   //std::cout << std::endl;
}

//------------------------------------------------------------------------------
// provideAttributeValueUpdate
//------------------------------------------------------------------------------
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
   HlaIO* netIO = getHlaIO();
   Nib* nib = (Nib*)( netIO->findNibByObjectHandle(theObject, HlaIO::OUTPUT_NIB) );

   if (nib != 0) nib->provideAttributeValueUpdate(theAttrs);
   //std::cout << std::endl;
}

//------------------------------------------------------------------------------
// discoverObjectInstance
//------------------------------------------------------------------------------
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
   HlaIO* netIO = getHlaIO();
   int classIndex = netIO->findObjectClassIndex(theObjectClass);
   if (classIndex != 0 && netIO->isObjectClassSubscribed(classIndex)) {
      // It's an object class that we've subscribed to ...

      // just act as an interface to HlaIO, which will handle this.
      getHlaIO()->discoverObjectInstance(theObject, theObjectClass, theObjectName);
   }
}

//------------------------------------------------------------------------------
// removeObjectInstance
//------------------------------------------------------------------------------
void Ambassador::removeObjectInstance (
        RTI::ObjectHandle          theObject,
  const RTI::FedTime&              theTime,
  const char                       *theTag,
        RTI::EventRetractionHandle theHandle)
throw (
   RTI::ObjectNotKnown,
   RTI::InvalidFederationTime,
   RTI::FederateInternalError)
{
   removeObjectInstance(theObject,theTag);
}

void Ambassador::removeObjectInstance (
        RTI::ObjectHandle          theObject,
  const char                      *theTag)
throw (
   RTI::ObjectNotKnown,
   RTI::FederateInternalError)
{
   std::cout << "Ambassador::removeObjectInstance(): remove object = " << theObject << ", theTag=" << theTag << std::endl;

   // find the input object
   HlaIO* netIO = getHlaIO();
   Nib* nib = netIO->findNibByObjectHandle(theObject, HlaIO::INPUT_NIB);
   if (nib != 0) {
      // set NIB delete request (Simulation::NetworkIO::cleanupInputList() should handle this)
      nib->setMode(Simulation::Player::DELETE_REQUEST);
   }
}

//------------------------------------------------------------------------------
// reflectAttributeValues
//------------------------------------------------------------------------------
void Ambassador::reflectAttributeValues (
        RTI::ObjectHandle                 theObject,
  const RTI::AttributeHandleValuePairSet& theAttrs,
  const RTI::FedTime&                     theTime,
  const char                              *theTag,
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
  const char                              *theTag)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::FederateOwnsAttributes,
   RTI::FederateInternalError)
{
   //std::cout << "Ambassador::reflectAttributeValues(): object = " << theObject << ", theTag=" << theTag << std::endl;
   // find the input object
   HlaIO* netIO = getHlaIO();
   Nib* nib = (Nib*)( netIO->findNibByObjectHandle(theObject, HlaIO::INPUT_NIB) );
   if (nib != 0) nib->reflectAttributeValues(theAttrs);
}

//------------------------------------------------------------------------------
// turnInteractionsOn
//------------------------------------------------------------------------------
void Ambassador::turnInteractionsOn (
     RTI::InteractionClassHandle theInteraction) 
   throw (
     RTI::InteractionClassNotPublished,
     RTI::FederateInternalError)
{
   std::cout << "Ambassador::turnInteractionsOn(): " ;
   HlaIO* netIO = getHlaIO();
   int idx = netIO->findInteractionClassIndex(theInteraction);
   if (idx != 0 && netIO->isInteractionClassPublished(idx)) {
      // It's an interaction that we publish, so we can start
      // send new interactions of this class ...
      std::cout << idx; 
      netIO->setInteractionEnabledFlag(idx,true);
   }
   std::cout << std::endl; 
}

//------------------------------------------------------------------------------
// turnInteractionsOff
//------------------------------------------------------------------------------
void Ambassador::turnInteractionsOff (
     RTI::InteractionClassHandle theInteraction) 
   throw (
     RTI::InteractionClassNotPublished,
     RTI::FederateInternalError)
{
   std::cout << "Ambassador::turnInteractionsOff(): " ;
   HlaIO* netIO = getHlaIO();
   int idx = netIO->findInteractionClassIndex(theInteraction);
   if (idx != 0) {
      std::cout << idx; 
      netIO->setInteractionEnabledFlag(idx,false);
   }
   std::cout << std::endl; 
}


//------------------------------------------------------------------------------
// receiveInteraction
//------------------------------------------------------------------------------
void Ambassador::receiveInteraction (
        RTI::InteractionClassHandle       theInteraction,
  const RTI::ParameterHandleValuePairSet& theParameters,
  const RTI::FedTime&                     theTime,
  const char                              *theTag,
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
   HlaIO* netIO = getHlaIO();
   int idx = netIO->findInteractionClassIndex(theInteraction);
   if (idx != 0 && netIO->isInteractionClassSubscribed(idx)) {
      // It's an interaction that we subscribe to, so ...

      // just act as an interface to HlaIO, which will handle this.
      getHlaIO()->receiveInteraction(theInteraction, theParameters);
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
   cout << "Unhandled synchronizationPointRegistrationSucceeded()" << endl;
}

void
Ambassador::synchronizationPointRegistrationFailed (
   const char *label) 
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled synchronizationPointRegistrationFailed()" << endl;
}

void
Ambassador::announceSynchronizationPoint (
   const char *label,
   const char *tag)
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled announceSynchronizationPoint()" << endl;
}

void
Ambassador::federationSynchronized (
   const char *label)
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled federationSynchronized()" << endl;
}

void
Ambassador::initiateFederateSave (
   const char *label)
throw (
   RTI::UnableToPerformSave,
   RTI::FederateInternalError)
{
   cout << "Unhandled initiateFederateSave()" << endl;
}

void
Ambassador::federationSaved ()
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled federationSaved()" << endl;
}

void
Ambassador::federationNotSaved ()
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled federationNotSaved()" << endl;
}

void
Ambassador::requestFederationRestoreSucceeded (
   const char *label) 
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled requestFederationRestoreSucceeded()" << endl;
}

void
Ambassador::requestFederationRestoreFailed (
   const char *label,
   const char *reason)
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled requestFederationRestoreFailed()" << endl;
}

void
Ambassador::federationRestoreBegun ()
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled federationRestoreBegun()" << endl;
}

void
Ambassador::initiateFederateRestore (
   const char *label,
   RTI::FederateHandle handle)
throw (
   RTI::SpecifiedSaveLabelDoesNotExist,
   RTI::CouldNotRestore,
   RTI::FederateInternalError)
{
   cout << "Unhandled initiateFederateRestore()" << endl;
}

void
Ambassador::federationRestored ()
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled federationRestored()" << endl;
}

void
Ambassador::federationNotRestored ()
throw (
   RTI::FederateInternalError)
{
   cout << "Unhandled federationNotRestored()" << endl;
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
   cout << "Unhandled attributesInScope()" << endl;
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
   cout << "Unhandled attributesOutOfScope()" << endl;
}


///////////////////////////////////
// Ownership Management Services //
///////////////////////////////////

void
Ambassador::requestAttributeOwnershipAssumption (
        RTI::ObjectHandle        theObject,
  const RTI::AttributeHandleSet& offeredAttributes,
  const char                    *theTag)
throw (
   RTI::ObjectNotKnown,
   RTI::AttributeNotKnown,
   RTI::AttributeAlreadyOwned,
   RTI::AttributeNotPublished,
   RTI::FederateInternalError)
{
   cout << "Unhandled requestAttributeOwnershipAssumption()" << endl;
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
   cout << "Unhandled attributeOwnershipDivestitureNotification()" << endl;
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
   cout << "Unhandled attributeOwnershipAcquisitionNotification()" << endl;
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
   cout << "Unhandled attributeOwnershipUnavailable()" << endl;
}

void
Ambassador::requestAttributeOwnershipRelease (
        RTI::ObjectHandle        theObject, 
  const RTI::AttributeHandleSet& candidateAttributes, 
  const char                     *theTag)
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeNotOwned,
  RTI::FederateInternalError)
{
   cout << "Unhandled requestAttributeOwnershipRelease()" << endl;
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
   cout << "Unhandled confirmAttributeOwnershipAcquisitionCancellation()" << endl;
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
   cout << "Unhandled informAttributeOwnership()" << endl;
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
   cout << "Unhandled attributeIsNotOwned()" << endl;
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
   cout << "Unhandled attributeOwnedByRTI()" << endl;
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
  cout << "### timeRegulationEnabled callback";
  hlaIo->setFederationTime(theFederateTime);
  cout << "  Federate Time is " << hlaIo->getFederationTime() << endl;
}

void
Ambassador::timeConstrainedEnabled (
   const RTI::FedTime& theFederateTime)
throw (
   RTI::InvalidFederationTime,
   RTI::EnableTimeConstrainedWasNotPending,
   RTI::FederateInternalError)
{
  cout << "### timeConstrainedEnabled callback";
  hlaIo->setFederationTime(theFederateTime);
  cout << "  Federate Time is " << hlaIo->getFederationTime() << endl;
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
//  cout << "### timeAdvanceGrant callback Time: " << hlaIo->getFederationTime() << endl;
}


void
Ambassador::requestRetraction (
   RTI::EventRetractionHandle theHandle)
throw (
   RTI::EventNotKnown,
   RTI::FederateInternalError)
{
   cout << "Unhandled requestRetraction()" << endl;
}

} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace
