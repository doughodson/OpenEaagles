//------------------------------------------------------------------------------
// Classes: HlaIO
//------------------------------------------------------------------------------
#ifndef __Hla_HlaIO_H_FCB0A791_BD3D_4521_AA66_0413F1206F73__
#define __Hla_HlaIO_H_FCB0A791_BD3D_4521_AA66_0413F1206F73__

#include "eaagles/simulation/NetworkIO.h"
#include "eaagles/basic/String.h"
#include <RTI.hh>
#include <fedtime.hh>

namespace Eaagles {
namespace Network {
namespace Hla {

class Ambassador;
class Nib;

//==============================================================================
// Class:       HlaIO
// Base class:	Basic::Object -> LcComponent -> Simulation::NetworkIO -> HlaIO
// Description: Abstract High-Level Architecture (HLA) protocol manager.
//              Unique Federations are managed by derived classes.
//
// Slots:
//      fedFile:            ! FED file name
//      regulatingTime:     ! Requlating time flag
//      constrainedTime:    ! constrained time flag
//
// Public member functions:
//
//      int findClassIndex(RTI::ObjectClassHandle handle)
//          Returns the index of the class handle or zero if not found.
//
//==============================================================================
class HlaIO : public Simulation::NetworkIO  
{
    DECLARE_SUBCLASS(HlaIO,Simulation::NetworkIO)

public:
   // Max active HLA object classes
   // Note: the class handle indexes range from 1 to MAX_CLASSES
   static const int MAX_CLASSES = 30;

   // Max active HLA object attributes
   // Note: the class handle indexes range from 1 to MAX_ATTRIBUTES
   static const int MAX_ATTRIBUTES = 100;
                              
   // Max active HLA interactions
   // Note: the class handle indexes range from 1 to MAX_INTERACTIONS
   static const int MAX_INTERACTIONS = 30;

   // Max active HLA interaction parameters
   // Note: the class handle indexes range from 1 to MAX_CLASSES
   static const int MAX_PARAMETERS = 100;

public:
    HlaIO();
    
    // HLA Object Class handles
    //    Note: ndex range [ 1 ... MAX_CLASSES ]
    int findObjectClassIndex(const RTI::ObjectClassHandle handle) const;
    virtual int getNumberOfObjectClasses() const=0;
    virtual bool setObjectClassHandle(const int idx, const RTI::ObjectClassHandle handle);    
    RTI::ObjectClassHandle getObjectClassHandle(const int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassHandles[idx-1] : 0; }

    // Object class published?
    bool isObjectClassPublished(const int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassPublished[idx-1] : 0; }
    virtual bool setObjectClassPublished(const int idx, const bool flag);    

    // Object class subscribed?
    bool isObjectClassSubscribed(const int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? objectClassSubscribed[idx-1] : 0; }
    virtual bool setObjectClassSubscribed(const int idx, const bool flag);    
   
    // HLA Object attribute handles
    //    Note: ndex range [ 1 ... MAX_ATTRIBUTES ]
    int findAttributeIndex(const RTI::AttributeHandle handle) const;
    virtual int getNumberOfObjectAttributes() const=0;
    virtual bool setObjectAttributeHandle(const int idx, const RTI::AttributeHandle handle);    
    RTI::AttributeHandle getObjectAttributeHandle(const int idx) const
            { return (idx >= 1 && idx <= MAX_ATTRIBUTES) ? objectAttributeHandles[idx-1] : 0; }
    
    // HLA Interaction Class handles
    //    Note: ndex range [ 1 ... MAX_INTERACTIONS ]
    int findInteractionClassIndex(const RTI::InteractionClassHandle handle) const;
    virtual int getNumberOfOInteractionClasses() const=0;
    virtual bool setInteractionClassHandle(const int idx, const RTI::InteractionClassHandle handle);    
    RTI::InteractionClassHandle getInteractionClassHandle(const int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassHandles[idx-1] : 0; }

    // Interaction class published?
    bool isInteractionClassPublished(const int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassPublished[idx-1] : 0; }
    virtual bool setInteractionClassPublished(const int idx, const bool flag);    

    // Interaction class subscribed?
    bool isInteractionClassSubscribed(const int idx) const
        { return (idx >= 1 && idx <= MAX_INTERACTIONS) ? interactionClassSubscribed[idx-1] : 0; }
    virtual bool setInteractionClassSubscribed(const int idx, const bool flag);    
        
    // HLA Interaction parameter handles
    //    Note: index range [ 1 ... MAX_PARAMETERS ]
    int findParameterIndex(const RTI::ParameterHandle handle) const;
    virtual int getNumberOfInteractionParameters() const=0;
    virtual bool setInteractionParameterHandle(const int idx, const RTI::ParameterHandle handle);    
    RTI::ParameterHandle getInteractionParameterHandle(const int idx) const
            { return (idx >= 1 && idx <= MAX_PARAMETERS) ? interactionParameterHandles[idx-1] : 0; }

    // Object class registration enabled flags
    virtual bool setObjectClassRegistrationFlag(const int idx, const bool flg);
    bool isObjectClassRegistrationEnabled(const int idx) const
        { return (idx >= 1 && idx <= MAX_CLASSES) ? regEnbl[idx-1] : false; }

    // Interaction enabled flags
    virtual bool setInteractionEnabledFlag(const int idx, const bool flg);
    bool isInteractionEnabled(const int idx) const
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
    virtual bool HlaIO::sendInteraction(const RTI::InteractionClassHandle handle, RTI::ParameterHandleValuePairSet* pParams, const char* theTag = 0);
    
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

    // Simulation::NetworkIO Interface
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

    // Simulation::NetworkIO Interface (Callbacks)
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
   Ambassador*     fedAmb;             // Our Fed Ambassador
   SPtr<Basic::String>  fedFileName;            // FED filename

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
   int        nInObjects;                // Number of input objects in both tables

   // output tables
   Nib*  outNameTbl[MAX_OBJECTS];   // Table of output objects in name order 
   Nib*  outHandleTbl[MAX_OBJECTS]; // Table of output objects in handle order 
   int        nOutObjects;               // Number of output objects in both tables

   // Support functions
   void addNibToNameTable(Nib* const nib, Nib** tbl, const int n);
   void addNibToHandleTable(Nib* const nib, Nib** tbl, const int n);
   void removeNibFromTable(Nib* const nib, Nib** tbl, const int n);

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

#endif // __Hla_HlaIO_H_FCB0A791_BD3D_4521_AA66_0413F1206F73__
