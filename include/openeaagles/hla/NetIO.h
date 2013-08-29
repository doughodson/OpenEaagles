//------------------------------------------------------------------------------
// Classes: NetIO
//------------------------------------------------------------------------------
#ifndef __Eaagles_Network_Hla_NetIO_H__
#define __Eaagles_Network_Hla_NetIO_H__

#include "openeaagles/simulation/NetIO.h"
#include "openeaagles/basic/String.h"
#include <RTI.hh>
#include <fedtime.hh>

namespace Eaagles {
namespace Network {
namespace Hla {

class Ambassador;
class Nib;

//==============================================================================
// Class:       Hla::NetIO
// Base class:	... -> Simulation::NetIO -> Network::Hla::NetIO
// Description: Abstract High-Level Architecture (HLA) protocol manager.
//              Unique Federations are managed by derived classes.
//
// Form Name: HlaNetIO
// Slots:
//      fedFile:            ! FED file name
//      regulatingTime:     ! Requlating time flag
//      constrainedTime:    ! constrained time flag
//
//==============================================================================
class NetIO : public Simulation::NetIO
{
   DECLARE_SUBCLASS(NetIO,Simulation::NetIO)

public:
   // Max active HLA object classes
   // Note: the class handle indexes range from 1 to MAX_CLASSES
   static const unsigned int MAX_CLASSES = 30;

   // Max active HLA object attributes
   // Note: the class handle indexes range from 1 to MAX_ATTRIBUTES
   static const unsigned int MAX_ATTRIBUTES = 100;
                              
   // Max active HLA interactions
   // Note: the class handle indexes range from 1 to MAX_INTERACTIONS
   static const unsigned int MAX_INTERACTIONS = 30;

   // Max active HLA interaction parameters
   // Note: the class handle indexes range from 1 to MAX_CLASSES
   static const unsigned int MAX_PARAMETERS = 100;

public:
    NetIO();
    
    // HLA Object Class handles
    //    Note: ndex range [ 1 ... MAX_CLASSES ]
    unsigned int findObjectClassIndex(const RTI::ObjectClassHandle handle) const;
    virtual unsigned int getNumberOfObjectClasses() const=0;
    virtual bool setObjectClassHandle(const unsigned int idx, const RTI::ObjectClassHandle handle);    
    RTI::ObjectClassHandle getObjectClassHandle(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassHandles[idx-1] : 0; }

    // Object class published?
    bool isObjectClassPublished(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassPublished[idx-1] : 0; }
    virtual bool setObjectClassPublished(const unsigned int idx, const bool flag);    

    // Object class subscribed?
    bool isObjectClassSubscribed(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassSubscribed[idx-1] : 0; }
    virtual bool setObjectClassSubscribed(const unsigned int idx, const bool flag);    
   
    // HLA Object attribute handles
    //    Note: ndex range [ 1 ... MAX_ATTRIBUTES ]
    unsigned int findAttributeIndex(const RTI::AttributeHandle handle) const;
    virtual unsigned int getNumberOfObjectAttributes() const=0;
    virtual bool setObjectAttributeHandle(const unsigned int idx, const RTI::AttributeHandle handle);    
    RTI::AttributeHandle getObjectAttributeHandle(const unsigned int idx) const
            { return (idx >= 1 && idx <= MAX_ATTRIBUTES) ? objectAttributeHandles[idx-1] : 0; }
    
    // HLA Interaction Class handles
    //    Note: ndex range [ 1 ... MAX_INTERACTIONS ]
    unsigned int findInteractionClassIndex(const RTI::InteractionClassHandle handle) const;
    virtual unsigned int getNumberOfOInteractionClasses() const=0;
    virtual bool setInteractionClassHandle(const unsigned int idx, const RTI::InteractionClassHandle handle);    
    RTI::InteractionClassHandle getInteractionClassHandle(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassHandles[idx-1] : 0; }

    // Interaction class published?
    bool isInteractionClassPublished(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassPublished[idx-1] : 0; }
    virtual bool setInteractionClassPublished(const unsigned int idx, const bool flag);    

    // Interaction class subscribed?
    bool isInteractionClassSubscribed(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassSubscribed[idx-1] : 0; }
    virtual bool setInteractionClassSubscribed(const unsigned int idx, const bool flag);    
        
    // HLA Interaction parameter handles
    //    Note: index range [ 1 ... MAX_PARAMETERS ]
    unsigned int findParameterIndex(const RTI::ParameterHandle handle) const;
    virtual unsigned int getNumberOfInteractionParameters() const=0;
    virtual bool setInteractionParameterHandle(const unsigned int idx, const RTI::ParameterHandle handle);    
    RTI::ParameterHandle getInteractionParameterHandle(const unsigned int idx) const
            { return (idx >= 1 && idx <= MAX_PARAMETERS) ? interactionParameterHandles[idx-1] : 0; }

    // Object class registration enabled flags
    virtual bool setObjectClassRegistrationFlag(const unsigned int idx, const bool flg);
    bool isObjectClassRegistrationEnabled(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? regEnbl[idx-1] : false; }

    // Interaction enabled flags
    virtual bool setInteractionEnabledFlag(const unsigned int idx, const bool flg);
    bool isInteractionEnabled(const unsigned int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactEnbl[idx-1] : false; }

    // Discover new objects
    virtual void discoverObjectInstance(
        const RTI::ObjectHandle theObject, const RTI::ObjectClassHandle theObjectClass, const char* theObjectName
    )=0;

    // Receive Interactions
    virtual void receiveInteraction(
        const RTI::InteractionClassHandle theInteraction,
        const RTI::ParameterHandleValuePairSet& theParameters
    )=0;


    // FED file name
    virtual const char* getFedFileName() const;
    
    // NIB lookup/search support
    Nib* findNibByObjectHandle(RTI::ObjectHandle handle, const IoType ioType);
    Nib* findNibByObjectName(const char* name, const IoType ioType);
    virtual void addNibToObjectTables(Simulation::Nib* const nib, const IoType ioType);
    virtual void removeNibFromObjectTables(Simulation::Nib* const nib, const IoType ioType);
    
    // ---
    // Other RTI functions
    // 
    
    RTI::RTIambassador* getRTIambassador() { return &rtiAmb; }  // Returns the RTI ambassador
    
    // Send object class attribute updates to the RTI ambassador
    virtual bool updateAttributeValues(const RTI::ObjectHandle handle, RTI::AttributeHandleValuePairSet* attrs, const char* theTag = 0);
    
    // Send an interaction to the RTI ambassador
    virtual bool sendInteraction(const RTI::InteractionClassHandle handle, RTI::ParameterHandleValuePairSet* pParams, const char* theTag = 0);
    
    RTIfedTime getLookAhead() const                         { return lookAhead; }
    void setLookAhead(const RTIfedTime time)                { lookAhead = time; }
    
    RTIfedTime getTimeIncrement() const                     { return timeIncrement; }
    void setTimeIncrement(const RTIfedTime time)            { timeIncrement = time; }
    
    RTIfedTime getFederationTime() const                    { return federationTime; }
    void setFederationTime(const RTIfedTime time)           { federationTime = time; }
   
    const RTI::Boolean getTimeAdvanceOutstanding() const    { return otaFlag; }
    void setTimeAdvanceOutstanding(const RTI::Boolean flag) { otaFlag = flag; }
    
    const RTI::Boolean getRegulating() const                { return rFlag; }
    void setRegulating(const RTI::Boolean flag)             { rFlag = flag; }
    
    const RTI::Boolean getConstrained() const               { return cFlag; }
    void setConstrained(const RTI::Boolean flag)            { cFlag = flag; }

    // Simulation::NetIO Interface
    virtual void destroyInputNib(Simulation::Nib* const nib);
    virtual void destroyOutputNib(Simulation::Nib* const nib);

protected:
    virtual void clearAllObjectClassHandles();
    virtual void clearAllObjectAttributeHandles();
    virtual void clearAllInteractionClassHandles();
    virtual void clearAllInteractionParameterHandles();
    virtual void clearAllObjectClassRegistrationFlags();
    virtual void clearAllInteractionEnabledFlags();

    // Callbacks
    virtual bool publishAndSubscribe()=0;
    virtual bool unPublishAndSubscribe();
    virtual bool unregisterAllObjects();
    virtual Ambassador* createFederateAmbassador();

    // Simulation::NetIO Interface (Callbacks)
    virtual bool initNetwork();              // Initialize the network 
    virtual void netInputHander();           // Network input handler

    // Federation Support
    virtual bool createAndJoinFederation();
    virtual bool resignAndDestroyFederation();
    virtual bool initializeTimeManagement();
    virtual bool advanceTime();
    virtual bool doTick();

    // Slot functions
    bool setSlotFedFile(Basic::String* const msg);
    bool setSlotRegulatingTime(Basic::Number* const msg);
    bool setSlotConstrainedTime(Basic::Number* const msg);

private:
   RTI::ObjectClassHandle  objectClassHandles[MAX_CLASSES];                // Object class handles
   RTI::AttributeHandle objectAttributeHandles[MAX_ATTRIBUTES];            // Object attribute handles
   bool regEnbl[MAX_CLASSES];                                              // Object registration enabled flags
   bool objectClassPublished[MAX_CLASSES];                                 // Object class is published
   bool objectClassSubscribed[MAX_CLASSES];                                // Object class is subscribed

   RTI::InteractionClassHandle interactionClassHandles[MAX_INTERACTIONS];  // Interaction class handles
   RTI::ParameterHandle interactionParameterHandles[MAX_PARAMETERS];       // Interaction parameter handles
   bool interactEnbl[MAX_INTERACTIONS];                                    // Interaction enable flags
   bool interactionClassPublished[MAX_INTERACTIONS];                       // Interaction class is published
   bool interactionClassSubscribed[MAX_INTERACTIONS];                      // Interaction class is subscribed

   RTI::RTIambassador  rtiAmb;             // RTI's Ambassador
   Ambassador*     fedAmb;                 // Our Fed Ambassador
   SPtr<Basic::String>  fedFileName;       // FED filename

   RTIfedTime      lookAhead;              // lookahead time
   RTIfedTime      timeIncrement;          // time to increment clock by
   RTIfedTime      federationTime;         // current federation time
   RTI::Boolean    otaFlag;                // Outstanding time advance flag
   RTI::Boolean    rFlag;                  // Time Regulating flag
   RTI::Boolean    cFlag;                  // Time constrained flag

   // ---
   // Quick lookup tables
   // ---
   // input tables
   Nib*  inNameTbl[MAX_OBJECTS];    // Table of input objects in name order 
   Nib*  inHandleTbl[MAX_OBJECTS];  // Table of input objects in handle order 
   unsigned int nInObjects;         // Number of input objects in both tables

   // output tables
   Nib*  outNameTbl[MAX_OBJECTS];   // Table of output objects in name order 
   Nib*  outHandleTbl[MAX_OBJECTS]; // Table of output objects in handle order 
   unsigned int nOutObjects;        // Number of output objects in both tables

   // Support functions
   void addNibToNameTable(Nib* const nib, Nib** tbl, const unsigned int n);
   void addNibToHandleTable(Nib* const nib, Nib** tbl, const unsigned int n);
   void removeNibFromTable(Nib* const nib, Nib** tbl, const unsigned int n);

   // bsearch callbacks: object name compare function --
   //   True types are (const char* key, const Nib** nib)
   static int compareObjNames(const void* key, const void* nib);

   // bsearch callbacks: object handle compare function -- 
   //   True types are (const RTI::ObjectClassHandle* key, const Nib** nib)
   static int compareObjHandles(const void* key, const void* nib);
};

} // End Hla namespace
} // End Network namespace
} // End Eaagles namespace

#endif


