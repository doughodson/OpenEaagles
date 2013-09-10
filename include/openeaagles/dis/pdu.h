//--------------------------------------------------------------
// PDU Structures
// Standard: IEEE P1278.1/D15
//
// PDU List
//    7.2.2    EntityStatePDU
//    7.2.3    CollisionPDU
//    7.3.2    FirePDU
//    7.3.3    DetonationPDU
//    7.5.4    StartPDU
//    7.5.5    StopPDU
//    7.5.6    AcknowledgePDU
//    7.5.7    ActionRequestPDU
//    7.5.9    DataQueryPDU
//    7.5.11   DataPDU
//    7.5.13   CommentPDU
//    7.6.2    ElectromagneticEmissionPDU
//    7.6.3    DesignatorPDU
//    7.6.5.2  IffAtcNavaidsPDU
//    7.7.2    TransmitterPDU
//    7.7.3    SignalPDU
//    7.11.7   ActionRequestPDU_R
//    7.11.8   ActionResponsePDU_R
//
//--------------------------------------------------------------
#ifndef __Eaagles_Network_Dis_Pdu_H__
#define __Eaagles_Network_Dis_Pdu_H__

#include "openeaagles/dis/structs.h"
#include "openeaagles/dis/enums.h"

#include <string>

namespace Eaagles {
namespace Network {
namespace Dis {

//-----------------------------------------------
// EntityStatePDU (1152 + 128*n bits)
//-----------------------------------------------
struct EntityStatePDU {

   PDUHeader            header;
   EntityIdentifierDIS  entityID;
   uint8_t              forceID;
   uint8_t              numberOfArticulationParameters;
   EntityType           entityType;
   EntityType           alternativeType;
   VectorDIS            entityLinearVelocity;
   WorldCoordinates     entityLocation;
   EulerAngles          entityOrientation;
   uint32_t             appearance;
   uint8_t              deadReckoningAlgorithm;
   uint8_t              otherParameters[15];
   VectorDIS            DRentityLinearAcceleration;
   AngularVelocityVectorDIS DRentityAngularVelocity;
   EntityMarking        entityMarking;
   uint32_t             capabilites;

   // Returns a pointer to the idx'th VpArticulatedPart structure;
   // Note: This ONLY works after the PDU have been created and initialized!
   //   (The "numberOfArticulationParameters' member variable needs to be correct
   //    for this to work)
   // Note: 'idx' is zero based, so an idx == 1 will return the second VpArticulatedPart structure
   VpArticulatedPart* getArticulationParameter(const unsigned int idx)
   {
      VpArticulatedPart* ap = 0;
      if (idx < numberOfArticulationParameters) {
         uint8_t *p = ((uint8_t *)this) + sizeof(*this);

         // First Emission system data is just after this structure
         ap = (VpArticulatedPart*) p;

         for (unsigned int i = 0; i < idx; i++) {
            // step down to the correct emission system data using 'systemDataLength'
            p += sizeof(VpArticulatedPart);
            ap = (VpArticulatedPart*) p;
         }

      }
      return ap;
   }

   const VpArticulatedPart* getArticulationParameter(const unsigned int idx) const
   {
      const VpArticulatedPart* ap = 0;
      if (idx < numberOfArticulationParameters) {

         // First articulation parameter is just after this structure
         const uint8_t *p = ((uint8_t *)this) + sizeof(*this);
         ap = (const VpArticulatedPart*) p;

         for (unsigned int i = 0; i < idx; i++) {
            // step down to the correct VpArticulatedPart structure
            p += sizeof(VpArticulatedPart);
            ap = (const VpArticulatedPart*) p;
         }

      }
      return ap;
   }

   void swapBytes() {
      // Swap our stuff first
      header.swapBytes();
      entityID.swapBytes();
      entityType.swapBytes();
      alternativeType.swapBytes();
      entityLinearVelocity.swapBytes();
      entityLocation.swapBytes();
      entityOrientation.swapBytes();
      DRentityLinearAcceleration.swapBytes();
      DRentityAngularVelocity.swapBytes();
      appearance = convertUInt32(appearance);
      capabilites = convertUInt32(capabilites);

      // then swap the articulation parameters
      for(int i = 0; i < numberOfArticulationParameters; i++) {
         VpArticulatedPart* ap = getArticulationParameter(i);
         if (ap != 0) ap->swapBytes();
      }
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Entity State PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Time Stamp (" << (int)header.timeStamp << ")" << std::endl;
      std::cout << entityID << std::endl;
      std::cout << "ForceID (" << (int)forceID << ")" << std::endl;
      std::cout << "Num Articulation Params (" << (int)numberOfArticulationParameters << ")" << std::endl;
      std::cout << "Entity Type:" << std::endl;
      std::cout << entityType << std::endl;
      std::cout << "Entity Alternate Type:" << std::endl;
      std::cout << alternativeType << std::endl;
      std::cout << "Entity Linear Velocity:" << std::endl;
      std::cout << entityLinearVelocity;
      std::cout << "Entity Location:" << std::endl;
      WorldCoordinates loc;
      memcpy(&loc,&entityLocation,sizeof(WorldCoordinates));
      std::cout << loc;
      std::cout << "Entity Orientation:" << std::endl;
      std::cout << entityOrientation;
      std::cout << "  Appearance (" << std::hex << appearance << ")" << std::endl;
      std::cout << std::dec;
      std::cout << "  DR Algor   (" << (int)deadReckoningAlgorithm << ")" << std::endl;
      std::cout << "  DR Params  ( ";
      std::cout << std::hex;
      for(unsigned int i = 0; i < 15; i++) {
         std::cout.width(2);
         std::cout.fill('0');
         std::cout << (int)otherParameters[i] << " ";
      }
      std::cout.width(0);
      std::cout << std::dec << ")" << std::endl;;
      std::cout << "DR Entity Linear Acceleration:" << std::endl;
      std::cout << DRentityLinearAcceleration;
      std::cout << "DR Angular Velocity:" << std::endl;
      std::cout << DRentityAngularVelocity;
      std::cout << "Entity Markings:" << std::endl;
      std::cout << entityMarking;
      std::cout << "Capabilities (" << capabilites << ")" << std::endl;
      for (unsigned int i = 0; i < numberOfArticulationParameters; i++) {
         const VpArticulatedPart* ap = getArticulationParameter(i);
         std::cout << "Articulation Parameter(" << i << ") = " << *ap << std::endl;
      }
      std::cout.flush();
   };
};


//-----------------------------------------------
// CollisionPDU (480 bits)
//-----------------------------------------------
struct CollisionPDU {

   PDUHeader            header;
   EntityIdentifierDIS  issuingEntityID;
   EntityIdentifierDIS  collidingEntityID;
   EventIdentifier      eventID;
   uint8_t              collisionType;
   uint8_t              padding;
   VectorDIS            velocity;
   float                mass;
   VectorDIS            location;

   void swapBytes() {
      header.swapBytes();
      issuingEntityID.swapBytes();
      collidingEntityID.swapBytes();
      eventID.swapBytes();
      velocity.swapBytes();
      mass = convertFloat(mass);
      location.swapBytes();
   };

   void dumpData() const {
   };
};


//-----------------------------------------------
// Fire PDU (768 bits)
//-----------------------------------------------
struct FirePDU {

   PDUHeader            header;
   EntityIdentifierDIS  firingEntityID;
   EntityIdentifierDIS  targetEntityID;
   EntityIdentifierDIS  munitionID;
   EventIdentifier      eventID;
   uint32_t             fireMissionIndex;
   WorldCoordinates     location;
   BurstDescriptor      burst;
   VectorDIS            velocity;
   float                range;

   void swapBytes() {
      header.swapBytes();
      firingEntityID.swapBytes();
      targetEntityID.swapBytes();
      munitionID.swapBytes();
      eventID.swapBytes();
      fireMissionIndex = convertUInt32(fireMissionIndex);
      location.swapBytes();
      burst.swapBytes();
      velocity.swapBytes();
      range = convertFloat(range);
   };

   void dumpData() const {
      WorldCoordinates loc;
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Fire PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Firing   Entity:" << std::endl << firingEntityID;
      std::cout << "Target   Entity:" << std::endl << targetEntityID;
      std::cout << "Munition Entity:" << std::endl << munitionID;
      std::cout << "Event ID:" << std::endl << eventID;
      std::cout << "fireMissionIndex (" << fireMissionIndex << ")" << std::endl;
      memcpy(&loc,&location,sizeof(WorldCoordinates));
      std::cout << "Location:" << std::endl << loc;
      std::cout << "Burst Descriptor:" << std::endl << burst;
      std::cout << "Velocity vectorDISy:" << std::endl << velocity;
      std::cout << "Range:" << std::endl << range << std::endl;
      std::cout.flush();
   };
};


//-----------------------------------------------
// Detonation PDU
//-----------------------------------------------
struct DetonationPDU {

   PDUHeader            header;
   EntityIdentifierDIS  firingEntityID;
   EntityIdentifierDIS  targetEntityID;
   EntityIdentifierDIS  munitionID;
   EventIdentifier      eventID;
   VectorDIS            velocity;
   WorldCoordinates     location;
   BurstDescriptor      burst;
   VectorDIS            locationInEntityCoordinates;
   uint8_t              detonationResult;
   uint8_t              numberOfArticulationParameters;
   uint16_t             padding;

   void swapBytes() {
      header.swapBytes();
      firingEntityID.swapBytes();
      targetEntityID.swapBytes();
      munitionID.swapBytes();
      eventID.swapBytes();
      velocity.swapBytes();
      location.swapBytes();
      burst.swapBytes();
      locationInEntityCoordinates.swapBytes();
   };

   void dumpData() const {
      WorldCoordinates loc;
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Detonation PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Firing   Entity:" << std::endl << firingEntityID;
      std::cout << "Target   Entity:" << std::endl << targetEntityID;
      std::cout << "Munition Entity:" << std::endl << munitionID;
      std::cout << "Event ID:" << std::endl << eventID;
      std::cout << "Velocity vectorDISy:" << std::endl << velocity;
      memcpy(&loc,&location,sizeof(WorldCoordinates));
      std::cout << "Location:" << std::endl << loc;
      std::cout << "Burst Descriptor:" << std::endl << burst;
      std::cout << "Location In Entity Coords:" << std::endl << locationInEntityCoordinates;
      std::cout << "Detonation Result (" << (int)detonationResult << ")" << std::endl;
      std::cout << "Num Of Atriculation Params (" << (int)numberOfArticulationParameters << ")" << std::endl;
      std::cout.flush();
   };
};


//-----------------------------------
// Start/Resume PDU (352 bits)
//-----------------------------------
struct StartPDU{

   PDUHeader            header;
   EntityIdentifierDIS  originatingID;
   EntityIdentifierDIS  receivingID;
   ClockTime            realWorldTime;
   ClockTime            simTime;
   uint32_t             requestID;

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      realWorldTime.swapBytes();
      simTime.swapBytes();
      requestID = convertUInt32(requestID);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Start PDU(" << (long)header.PDUType << ")" << std::endl;
      std::cout << "Sending     Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Real World Time:" << std::endl << realWorldTime;
      std::cout << "Simulation Time:" << std::endl << simTime;
      std::cout << "Request ID: " << std::endl << requestID;
      std::cout.flush();
   };
};

//-----------------------------------
// Stop/Freeze PDU (320 bits)
//-----------------------------------

struct StopPDU{

   PDUHeader            header;
   EntityIdentifierDIS  originatingID;
   EntityIdentifierDIS  receivingID;
   ClockTime            realWorldTime;
   uint8_t              reason;
   uint8_t              frozenBehavior;
   uint16_t             padding;
   uint32_t             requestID;

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      realWorldTime.swapBytes();
      requestID = convertUInt32(requestID);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Start PDU(" << (long)header.PDUType << ")" << std::endl;
      std::cout << "Sending     Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Real World Time:" << std::endl << realWorldTime;
      std::cout << "Reason: (" << std::endl << (long)reason << ")" << std::endl;
      std::cout << "Frozen Behavior (: " << (long)frozenBehavior << ")" << std::endl;
      std::cout << "Request ID: "  << requestID << std::endl;
      std::cout.flush();
   };
};


//-----------------------------------
// Acknowledge PDU
//-----------------------------------
struct AcknowledgePDU{

   PDUHeader              header;
   EntityIdentifierDIS    originatingID;
   EntityIdentifierDIS    receivingID;
   uint16_t               acknowledgeFlag;
   uint16_t               responseFlag;
   uint32_t               requestID;

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Start PDU(" << (long)header.PDUType << ")" << std::endl;
      std::cout << "Sending     Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Acknowledge Flag:" << std::endl << acknowledgeFlag;
      std::cout << "Response Flag:" << std::endl << responseFlag;
      std::cout << "Request ID: " << std::endl << requestID;
      std::cout.flush();
   };

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      acknowledgeFlag = convertUInt16(acknowledgeFlag);
      responseFlag = convertUInt16(responseFlag);
      requestID = convertUInt32(requestID);
   };
};


//-----------------------------------
// Action Request PDU
//-----------------------------------
struct ActionRequestPDU {

   PDUHeader            header;
   EntityIdentifierDIS  originatingID;
   EntityIdentifierDIS  receivingID;
   uint32_t             requestID;
   uint32_t             actionID;
   uint32_t             numFixedRecords;
   uint32_t             numVariableRecords;

   uint8_t* getData() {
      uint8_t *p = (uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + sizeof(requestID)
         + sizeof(actionID)
         + sizeof(numFixedRecords)
         + sizeof(numVariableRecords);
      return &p[pcount];
   };

   const uint8_t* getData() const {
      const uint8_t *p = (const uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + sizeof(requestID)
         + sizeof(actionID)
         + sizeof(numFixedRecords)
         + sizeof(numVariableRecords);
      return &p[pcount];
   };

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      requestID           = convertUInt32(requestID);
      actionID            = convertUInt32(actionID);
      numFixedRecords     = convertUInt32(numFixedRecords);
      numVariableRecords  = convertUInt32(numVariableRecords);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Data Query PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Emitting Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Request ID: " << std::endl << requestID;
      std::cout << "Action ID: " << std::endl << actionID;
      std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
      std::cout << "Number of variable records: " << std::endl << numVariableRecords;
   };
};


//-----------------------------------------------
// Data Query PDU
//-----------------------------------------------
struct DataQueryPDU{

   PDUHeader              header;
   EntityIdentifierDIS    originatingID;
   EntityIdentifierDIS    receivingID;
   uint32_t               requestID;
   uint32_t               timeInterval;
   uint32_t               numFixedRecords;
   uint32_t               numVariableRecords;

   // Returns a pointer to the start of the fixed/variable records
   uint8_t* getData() {
      uint8_t *p = (uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + sizeof(requestID)
         + sizeof(timeInterval)
         + sizeof(numFixedRecords)
         + sizeof(numVariableRecords);
      return &p[pcount];
   };

   // Returns a const pointer to the start of the fixed/variable records
   const uint8_t* getData() const {
      const uint8_t *p = (const uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + sizeof(requestID)
         + sizeof(timeInterval)
         + sizeof(numFixedRecords)
         + sizeof(numVariableRecords);
      return &p[pcount];
   };

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      requestID           = convertUInt32(requestID);
      timeInterval        = convertUInt32(timeInterval);
      numFixedRecords     = convertUInt32(numFixedRecords);
      numVariableRecords  = convertUInt32(numVariableRecords);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Data Query PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Emitting Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Request ID: " << std::endl << requestID;
      std::cout << "Time Interval: " << std::endl << timeInterval;
      std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
      std::cout << "Number of variable records: " << std::endl << numVariableRecords;
   };
};


//-----------------------------------------------
// Data PDU
//-----------------------------------------------
struct DataPDU{

   PDUHeader              header;
   EntityIdentifierDIS    originatingID;
   EntityIdentifierDIS    receivingID;
   uint32_t               requestID;
   uint32_t               padding;
   uint32_t               numFixedRecords;
   uint32_t               numVariableRecords;

   uint8_t* getData() {
      uint8_t *p = (uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + sizeof(requestID)
         + sizeof(padding)
         + sizeof(numFixedRecords)
         + sizeof(numVariableRecords);
      return &p[pcount];
   };
   const uint8_t* getData() const {
      const uint8_t *p = (const uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + sizeof(requestID)
         + sizeof(padding)
         + sizeof(numFixedRecords)
         + sizeof(numVariableRecords);
      return &p[pcount];
   };

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      requestID           = convertUInt32(requestID);
      numFixedRecords     = convertUInt32(numFixedRecords);
      numVariableRecords  = convertUInt32(numVariableRecords);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Data Query PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Emitting Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Request ID: " << std::endl << requestID;
      std::cout << "Padding: " << std::endl << padding;
      std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
      std::cout << "Number of variable records: " << std::endl << numVariableRecords;
   };
};


//-----------------------------------------------
// Comment PDU
//-----------------------------------------------
struct CommentPDU{

   PDUHeader              header;
   EntityIdentifierDIS    originatingID;
   EntityIdentifierDIS    receivingID;
   uint32_t               numFixedRecords;
   uint32_t               numVariableRecords;

   uint8_t* getData() {
      uint8_t *p = (uint8_t*)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + 8;
      return &p[pcount];
   };

   const uint8_t* getData() const {
      const uint8_t *p = (const uint8_t*)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + 8;
      return &p[pcount];
   };


   FixedDatum* getFixedDatum(const unsigned int index) {
      if (index >= numFixedRecords) return 0;

      uint8_t* ptr = getData();
      FixedDatum* datum = (FixedDatum *) ptr;
      for(unsigned int i = 0; i < index; i++) {
         datum++;
      }

      return datum;
   };

   void swapFixedDatum() {
      FixedDatum *datum = 0;
      for(unsigned int  i= 0; i < numFixedRecords; i++) {
         datum = getFixedDatum(i);
         datum->swapBytes();
      }
   };

   VariableDatum* getVariableDatum(const unsigned int index) {
      if (index >= numVariableRecords) return 0;

      uint8_t *ptr = getData();
      if (numFixedRecords > 0) {
         FixedDatum *datum = getFixedDatum(numFixedRecords-1);
         ptr = (uint8_t*)datum;
         ptr += sizeof(FixedDatum);
      }

      VariableDatum *vdatum = (VariableDatum *)ptr;
      for(unsigned int i = 0;i < index;i++) {
         ptr += vdatum->getSize();
         vdatum = (VariableDatum *)ptr;
      }

      return vdatum;
   };

   void swapVariableDatum() {
      VariableDatum *datum = 0;
      for(unsigned int i = 0; i < numVariableRecords; i++) {
         datum = getVariableDatum(i);
         datum->swapBytes();
      }
   };

   void pack() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      swapVariableDatum();
      swapFixedDatum();
      numFixedRecords     = convertUInt32(numFixedRecords);
      numVariableRecords  = convertUInt32(numVariableRecords);
   };

   void unpack() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      numFixedRecords     = convertUInt32(numFixedRecords);
      numVariableRecords  = convertUInt32(numVariableRecords);
      swapVariableDatum();
      swapFixedDatum();
   };

   void swapBytes() {
      unpack();
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Data Query PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Emitting Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
      std::cout << "Number of variable records: " << std::endl << numVariableRecords;
   };
};


//-----------------------------------------------
// 7.6.2 -- Electromagnetic Emission PDU
//-----------------------------------------------
struct ElectromagneticEmissionPDU {

   // (IST-CF-03-01, May 5, 2003)
   enum { STATE_UPDATE = 0, CHANGED_DATA_UPDATE = 1 };

   PDUHeader            header;                 // PDU Header
   EntityIdentifierDIS  emittingEntityID;       // Entity that owns these systems
   EventIdentifier      eventID;                // Event ID
   uint8_t              stateUpdateIndicator;   // State
   uint8_t              numberOfSystems;        // Number of systems (EmissionSystem) that follow
   uint16_t             padding;

   // Returns a pointer to the idx'th EmissionSystem structure;
   // Note: This ONLY works after the PDU and emissions systems have been created and initialized!
   //   (The systemDataLength's in the EmissionSystem structures, as well as our
   //    'numberOfSystems' member variable, need to be correct for this to work)
   // Note: 'idx' is zero based, so a idx == 1 will return the second EmissionSystem structure
   EmissionSystem* getEmissionSystem(const unsigned int idx) {
      EmissionSystem* es = 0;
      if (idx < numberOfSystems) {
         uint8_t *p = ((uint8_t *)this) + sizeof(*this);

         // First Emission system data is just after this structure
         es = (EmissionSystem*) p;

         for (unsigned int i = 0; i < idx; i++) {
            // step down to the correct emission system data using 'systemDataLength'
            p += (es->systemDataLength * 4);
            es = (EmissionSystem*) p;
         }

      }
      return es;
   }
   const EmissionSystem* getEmissionSystem(const unsigned int idx) const {
      const EmissionSystem* es = 0;
      if (idx < numberOfSystems) {
         uint8_t *p = ((uint8_t *)this) + sizeof(*this);

         // First Emission system data is just after this structure
         es = (const EmissionSystem*) p;

         for (unsigned int i = 0; i < idx; i++) {
            // step down to the correct emission system data using 'systemDataLength'
            p += (es->systemDataLength * 4);
            es = (const EmissionSystem*) p;
         }

      }
      return es;
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      // Swap our stuff first
      header.swapBytes();
      emittingEntityID.swapBytes();
      eventID.swapBytes();

      // then swap emission data
      for(unsigned int i = 0; i < numberOfSystems; i++) {
         EmissionSystem* em = getEmissionSystem(i);
         if (em != 0) em->swapBytes();
      }
   }

   // Dump the PDU's data to the standard output device, std::cout.
   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Electromagnetic PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Header:" << std::endl << header;
      std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
      std::cout << "Event ID:" << std::endl << eventID;
      std::cout << "State Update Indicator: " << (int)stateUpdateIndicator << std::endl;
      std::cout << "Num Of Systems:         " << (int)numberOfSystems << std::endl;

      for(unsigned int i = 0; i < numberOfSystems; i++){
         std::cout << "*****************************************" << std::endl;
         std::cout << ">>>System (" << i << ")<<<" << std::endl;
         const EmissionSystem* es = getEmissionSystem(i);
         if (es != 0) std::cout << es;
         std::cout << "*****************************************" << std::endl;
      }
      std::cout.flush();
   }
};

//-----------------------------------------------
// 7.6.3 -- Designator PDU
//-----------------------------------------------
struct DesignatorPDU {

   PDUHeader            header;                 // PDU Header
   EntityIdentifierDIS  designatingEntityID;    // Entity that is positioning the designator
   uint16_t             codeName;               // code name for the designation system
   EntityIdentifierDIS  designatedEntityID;     // Entity that is currently being designated
   uint16_t             designatorCode;
   float                power;
   float                wavelength;
   VectorDIS            spotPosition;
   WorldCoordinates     spotLocation;
   uint8_t              deadReckoningAlgorithm;
   uint8_t              padding[3];
   VectorDIS            DRentityLinearAcceleration;

   // Swap bytes 'to' or 'from' the network.
   void swapBytes(){
      // Swap our stuff first
      header.swapBytes();
      designatingEntityID.swapBytes();
      codeName = convertUInt16(codeName);
      designatedEntityID.swapBytes();
      designatorCode = convertUInt16(designatorCode);
      power = convertFloat(power);
      wavelength = convertFloat(wavelength);
      spotPosition.swapBytes();
      spotLocation.swapBytes();
      DRentityLinearAcceleration.swapBytes();
   };

   void dumpData() const {
       WorldCoordinates spotLoc;
       std::cout << "------------------------------------------------" << std::endl;
       std::cout << "Designator PDU(" << (long)header.PDUType << ")" << std::endl;
       std::cout << "Designating Entity:" << std::endl << designatingEntityID;
       std::cout << "Designated Entity:" << std::endl << designatedEntityID;
       std::cout << "Code Name      : " << codeName << std::endl;
       std::cout << "Designator Code: " << designatorCode << std::endl;
       std::cout << "Power          : " << power << std::endl;
       std::cout << "Wavelength     : " << wavelength << std::endl;
       memcpy(&spotLoc,&spotLocation,sizeof(WorldCoordinates));
       std::cout << "Spot Location:" << std::endl << spotLoc;
       std::cout << "Spot Position:" << std::endl << spotPosition;
       std::cout.flush();
   };
};



//-----------------------------------------------
// IFF/ATC/NAVAIDS PDU
//-----------------------------------------------
struct IffAtcNavaidsPDU
{

   // Layer 1 -- basic data
   PDUHeader            header;
   EntityIdentifierDIS  emittingEntityID;
   EventIdentifier      eventID;
   VectorDIS            location;
   SystemID             systemID;
   uint8_t              sysDesig;         // IEEE P1278.1/D15
   uint8_t              sysSpecData;      // IEEE P1278.1/D15
   FundamentalOpData    operationalData;

   void swapBytes() {
      header.swapBytes();
      emittingEntityID.swapBytes();
      eventID.swapBytes();
      location.swapBytes();
      systemID.swapBytes();
      operationalData.swapBytes();
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "IFF/ATC/NAVAIDS PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Header:" << std::endl << header;
      std::cout << "Emitting Entity:" << std::endl << emittingEntityID;
      std::cout << "Event ID:" << std::endl << eventID;
      std::cout << "Emission Location:" << std::endl << location;
      std::cout << "SystemID:" << std::endl << systemID;
      std::cout << "Operation Data:" << std::endl << operationalData;
      std::cout.flush();
   };
};


//-----------------------------------------------
// Transmitter PDU
//-----------------------------------------------
struct TransmitterPDU {

   PDUHeader            header;                 // PDU Header
   EntityIdentifierDIS  radioRefID;             // Entity that owns these systems
   uint16_t             radioID;                // Radio ID (unique to entity)
   RadioEntityType      radioEntityType;        // Radio entity type
   uint8_t              txState;                // Transmit state
   uint8_t              inputSource;            // Operator position: pilot, co-pilot, etc
   uint16_t             padding;

   // Antenna location on entity
   double               antLoc_X_coord;
   double               antLoc_Y_coord;
   double               antLoc_Z_coord;
   float                antLoc_x_coord;
   float                antLoc_y_coord;
   float                antLoc_z_coord;

   uint16_t             antennaPatternType;     // Pattern type
   uint16_t             antennaPatternLength;   // Pattern length
   uint64_t             frequency;              // Center frequency (hz)
   float                transmitFrequencyBandwidth; //Bandwidth (hz)
   float                power;                  // Power decibel-milliwatts
   ModulationType       modulationType;         // Modulation type
   uint16_t             cryptoSystem;           // Crypto system
   uint16_t             cryptoKeyID;            // Crypto key id
   uint8_t              lengthOfModulationParameters;
   uint8_t              padding1;
   uint8_t              padding2;
   uint8_t              padding3;

   void swapBytes(){
      header.swapBytes();
      radioRefID.swapBytes();
      radioID = convertUInt16(radioID);
      radioEntityType.swapBytes();
      antLoc_X_coord = convertDouble(antLoc_X_coord);
      antLoc_Y_coord = convertDouble(antLoc_Y_coord);
      antLoc_Z_coord = convertDouble(antLoc_Z_coord);
      antLoc_x_coord = convertFloat(antLoc_x_coord);
      antLoc_y_coord = convertFloat(antLoc_y_coord);
      antLoc_z_coord = convertFloat(antLoc_z_coord);
      antennaPatternType = convertUInt16(antennaPatternType);
      antennaPatternLength = convertUInt16(antennaPatternLength);
      frequency = convertUInt64(frequency);
      transmitFrequencyBandwidth = convertFloat(transmitFrequencyBandwidth);
      power = convertFloat(power);
      modulationType.swapBytes();
      cryptoSystem = convertUInt16(cryptoSystem);
      cryptoKeyID = convertUInt16(cryptoKeyID);
      //modulationParam5 = convertUInt32(modulationParam5);
   };

   uint8_t* getModulationData() {
      uint8_t* p = (uint8_t*)this;
      size_t offset = sizeof(TransmitterPDU);
      return &p[offset];
   };

   const uint8_t* getModulationData() const {
      const uint8_t* p = (const uint8_t*)this;
      size_t offset = sizeof(TransmitterPDU);
      return &p[offset];
   };

   void dumpData() const {
      std::cout << "--------------------data---------------------" << std::endl;
      std::cout << "Emitting Entity ID:" << std::endl;
      std::cout << "\tEntity: " << radioRefID.ID << std::endl;
      std::cout << "\tAppID:  " << radioRefID.simulationID.applicationIdentification << std::endl;
      std::cout << "\tSiteID: " << radioRefID.simulationID.siteIdentification << std::endl;
      std::cout << "Radio ID: " << radioID << std::endl;
      std::cout << "RadioEntity:" << radioEntityType << std::endl;
      std::cout << "Transmission State: " << (int)txState << std::endl;
      std::cout << "Input Source: " << (int)inputSource << std::endl;
      std::cout << "antLoc_X_coord: " << (int)antLoc_X_coord << std::endl;
      std::cout << "antLoc_Y_coord: " << (int)antLoc_Y_coord << std::endl;
      std::cout << "antLoc_Z_coord: " << (int)antLoc_Z_coord << std::endl;
      std::cout << "antLoc_x_coord: " << (int)antLoc_x_coord << std::endl;
      std::cout << "antLoc_y_coord: " << (int)antLoc_y_coord << std::endl;
      std::cout << "antLoc_z_coord: " << (int)antLoc_z_coord << std::endl;
      std::cout << "Antenna Pattern Type: " << antennaPatternType << std::endl;
      std::cout << "Antenna Pattern Length: " << antennaPatternLength << std::endl;
      std::cout << "Frequency: " << frequency << std::endl;
      std::cout << "Transmit Frequency Bandwidth: " << transmitFrequencyBandwidth << std::endl;
      std::cout << "Power: " << power << std::endl;
      std::cout << "Modulation Type: " << modulationType << std::endl;
      std::cout << "Crypto System: " << cryptoSystem << std::endl;
      std::cout << "Crypto Key ID: " << cryptoKeyID << std::endl;
      std::cout << "Length of Modulation Parameters: " << lengthOfModulationParameters << std::endl;
      std::cout << "--------------------end data-----------------" << std::endl;
   };

};


//-----------------------------------------------
// Signal PDU
//-----------------------------------------------
struct SignalPDU {

   PDUHeader           header;
   EntityIdentifierDIS radioRefID;
   uint16_t            radioID;
   uint16_t            encodingScheme;
   uint16_t            TDLType;
   uint32_t            sampleRate;
   uint16_t            dataLength;
   uint16_t            samples;

   uint8_t* getData() {
      uint8_t *p = (uint8_t *)this;
      int pcount = sizeof(PDUHeader) + sizeof(radioRefID) + 14;
      return &p[pcount];
   };

   const uint8_t* getData() const {
      const uint8_t *p = (const uint8_t *)this;
      int pcount = sizeof(PDUHeader) + sizeof(radioRefID) + 14;
      return &p[pcount];
   };

   void swapBytes() {
      header.swapBytes();
      radioRefID.swapBytes();
      radioID        = convertUInt16(radioID);
      encodingScheme = convertUInt16(encodingScheme);
      TDLType        = convertUInt16(TDLType);
      sampleRate     = convertUInt32(sampleRate);
      dataLength     = convertUInt16(dataLength);
      samples        = convertUInt16(samples);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Signal PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Emitting Entity:" << std::endl << radioRefID;
      std::cout << "Radio ID: ( " << radioID << " )" << std::endl;
      std::cout << "encodingScheme: ( " << std::hex << encodingScheme << std::dec << " )" << std::endl;
      std::cout << "TDLType: ( " << TDLType << " )" << std::endl;
      std::cout << "sampleRate: ( " << sampleRate << " )" << std::endl;
      std::cout << "dataLength: ( " << dataLength << " )" << std::endl;
      std::cout << "samples: ( " << samples << " )" << std::endl;
      std::cout << "Data:" << std::endl;

      unsigned int count = 0;

      const uint8_t* p = getData();

      std::cout << std::hex;

      for (unsigned int i = 0; i < dataLength; i++) {

         std::cout.width(2);
         std::cout.fill('0');
         std::cout << (int)p[i] << " ";

         if (count >= 16) {
            std::cout << std::endl;
            count = 0;
         }
         else {
            count++;
         }
      }

      std::cout << std::dec << std::endl;
      std::cout.flush();
   };
};


//-----------------------------------
// Action Request-R PDU
//-----------------------------------
struct ActionRequestPDU_R {

   PDUHeader            header;
   EntityIdentifierDIS  originatingID;
   EntityIdentifierDIS  receivingID;
   uint8_t              reliabilityService;
   uint8_t              padding1;
   uint8_t              padding2;
   uint8_t              padding3;
   uint32_t             requestID;
   uint32_t             actionID;
   uint32_t             numFixedRecords;
   uint32_t             numVariableRecords;

   uint8_t* getData() {
      uint8_t *p = (uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + 1 + 3 //reliabilityService and padding
         + 16;   //requestid, actionid, numfixedrecords, numvariablerecords
      return &p[pcount];
   };

   const uint8_t* getData() const {
      const uint8_t *p = (const uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + 1 + 3 //reliabilityService and padding
         + 16;   //requestid, actionid, numfixedrecords, numvariablerecords
      return &p[pcount];
   };

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      requestID           = convertUInt32(requestID);
      actionID            = convertUInt32(actionID);
      numFixedRecords     = convertUInt32(numFixedRecords);
      numVariableRecords  = convertUInt32(numVariableRecords);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Data Query PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Emitting Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Reliability Service: " << std::endl << reliabilityService;
      std::cout << "Request ID: " << std::endl << requestID;
      std::cout << "Action ID: " << std::endl << actionID;
      std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
      std::cout << "Number of variable records: " << std::endl << numVariableRecords;
   };
};


//-----------------------------------
// Action Response PDU
//-----------------------------------
struct ActionResponsePDU_R {

   PDUHeader                 header;
   EntityIdentifierDIS       originatingID;
   EntityIdentifierDIS       receivingID;
   uint32_t                  requestID;
   uint32_t                  responseStatus;
   uint32_t                  numFixedRecords;
   uint32_t                  numVariableRecords;

   uint8_t* getData() {
      uint8_t *p = (uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + 16;  //requestid, responseStatus, numfixedrecords, numvariablerecords
      return &p[pcount];
   };

   const uint8_t* getData() const {
      const uint8_t *p = (const uint8_t *)this;
      int pcount = sizeof(PDUHeader)
         + sizeof(originatingID)
         + sizeof(receivingID)
         + 16;  //requestid, responseStatus, numfixedrecords, numvariablerecords
      return &p[pcount];
   };

   void swapBytes() {
      header.swapBytes();
      originatingID.swapBytes();
      receivingID.swapBytes();
      requestID           = convertUInt32(requestID);
      responseStatus      = convertUInt32(responseStatus);
      numFixedRecords     = convertUInt32(numFixedRecords);
      numVariableRecords  = convertUInt32(numVariableRecords);
   };

   void dumpData() const {
      std::cout << "------------------------------------------------" << std::endl;
      std::cout << "Data Query PDU(" << (int)header.PDUType << ")" << std::endl;
      std::cout << "Emitting Entity:" << std::endl << originatingID;
      std::cout << "Destination Entity:" << std::endl << receivingID;
      std::cout << "Request ID: " << std::endl << requestID;
      std::cout << "Response Status: " << std::endl << responseStatus;
      std::cout << "Number of fixed records: " << std::endl << numFixedRecords;
      std::cout << "Number of variable records: " << std::endl << numVariableRecords;
   };
};

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace

#endif
