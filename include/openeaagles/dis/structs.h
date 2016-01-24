//--------------------------------------------------------------
// IEEE Standard for Distributed Interactive Simulation (DIS)
// Application Protocols - Record Structures for DIS PDU's
//
// Standard: IEEE P1278.1/D15
//
// Struct List
//    6.2.7    AngularVelocityVectorDIS
//    6.2.9    AntennaPattern
//    6.2.12   BeamData
//    6.2.16   ClockTime
//    6.2.18   DataQueryDatumSpecification
//    6.2.19   DatumSpecification
//    6.2.20.2 BurstDescriptor (Munition)
//    6.2.22   EeFundamentalParameterData
//    6.2.23   EmitterSystem
//    6.2.28   EntityIdentifierDIS
//    6.2.29   EntityMarking
//    6.2.30   EntityType
//    6.2.33   EulerAngles
//    6.2.34   EventIdentifier
//    6.2.38   FixedDatum
//    6.2.40   FundamentalOpData
//    6.2.50 --JammingTechnique (IEEE P1278.1/D15)
//    6.2.59   ModulationType
//    6.2.66   PDUHeader
//    6.2.70   RadioEntityType
//    6.2.79   SimulationAddressDIS
//    6.2.81   SimulationManagementPDUHeader
//    6.2.85   SupplyQuantity
//    6.2.86   SystemID
//    6.2.89   TrackJamTargets
//    6.2.89   VariableDatum
//    6.2.93.2 VpArticulatedPart
//    6.2.95   VectorDIS
//    6.2.97   WorldCoordinates
//
//    7.6.2    EmissionSystem (part of Electromagnetic Emission PDU)
//    7.6.2    EmitterBeamData (part of Electromagnetic Emission PDU)
//--------------------------------------------------------------
#ifndef __oe_network_dis_Struct_H__
#define __oe_network_dis_Struct_H__

#include <iostream>
#include <cstdint>

namespace oe {
namespace network {
namespace dis {

//--------------------------------------------------------------
// Byte swap functions used by this file
//--------------------------------------------------------------

union convDoubleData
{
   int64_t   llword;
   uint64_t  ullword;
   double    dword;
   int8_t    byte[8];
};

union convData
{
   int32_t  lword;
   uint32_t uint;
   float    fword;
   int16_t  sword[2];
   uint16_t usword[2];
   int8_t   byte[4];
};


// Byte swap check
// -- returns true if this computer's byte order is not big ending (network) order.
inline bool checkSwap() {
  uint16_t s = 1;
  uint8_t* c = reinterpret_cast<uint8_t*>(&s);
  return (*c != 0);
}

inline int32_t convertInt32(const int32_t v) {
   convData value;
   value.lword = v;
   convData newValue;

   newValue.byte[3] = value.byte[0];
   newValue.byte[2] = value.byte[1];
   newValue.byte[1] = value.byte[2];
   newValue.byte[0] = value.byte[3];

   return newValue.lword;
}

inline uint32_t convertUInt32(const uint32_t v) {
   convData value;
   value.uint = v;
   convData newValue;

   newValue.byte[3] = value.byte[0];
   newValue.byte[2] = value.byte[1];
   newValue.byte[1] = value.byte[2];
   newValue.byte[0] = value.byte[3];

   return newValue.uint;
}

inline float convertFloat(const float v) {
   convData value;
   value.fword = v;
   convData newValue;

   newValue.byte[3] = value.byte[0];
   newValue.byte[2] = value.byte[1];
   newValue.byte[1] = value.byte[2];
   newValue.byte[0] = value.byte[3];

   return newValue.fword;
}

inline int64_t convertInt64(const int64_t v) {
   convDoubleData value;
   value.llword = v;
   convDoubleData newValue;

   newValue.byte[7] = value.byte[0];
   newValue.byte[6] = value.byte[1];
   newValue.byte[5] = value.byte[2];
   newValue.byte[4] = value.byte[3];
   newValue.byte[3] = value.byte[4];
   newValue.byte[2] = value.byte[5];
   newValue.byte[1] = value.byte[6];
   newValue.byte[0] = value.byte[7];

   return newValue.llword;
}

inline uint64_t convertUInt64(const uint64_t v) {
   convDoubleData value;
   value.ullword = v;
   convDoubleData newValue;

   newValue.byte[7] = value.byte[0];
   newValue.byte[6] = value.byte[1];
   newValue.byte[5] = value.byte[2];
   newValue.byte[4] = value.byte[3];
   newValue.byte[3] = value.byte[4];
   newValue.byte[2] = value.byte[5];
   newValue.byte[1] = value.byte[6];
   newValue.byte[0] = value.byte[7];

   return newValue.ullword;
}

inline double convertDouble(const double v) {
   convDoubleData value;
   value.dword = v;
   convDoubleData newValue;

   newValue.byte[7] = value.byte[0];
   newValue.byte[6] = value.byte[1];
   newValue.byte[5] = value.byte[2];
   newValue.byte[4] = value.byte[3];
   newValue.byte[3] = value.byte[4];
   newValue.byte[2] = value.byte[5];
   newValue.byte[1] = value.byte[6];
   newValue.byte[0] = value.byte[7];

   return newValue.dword;
}

inline int16_t convertInt16(const int16_t v) {
   convData value;
   value.sword[0] = v;
   convData newValue;

   newValue.byte[1] = value.byte[0];
   newValue.byte[0] = value.byte[1];

   return newValue.sword[0];
}

inline uint16_t convertUInt16(const uint16_t v) {
   convData value;
   value.usword[0] = v;
   convData newValue;

   newValue.byte[1] = value.byte[0];
   newValue.byte[0] = value.byte[1];

   return newValue.usword[0];
}


//-----------------------------------------------
// 6.2.30 -- Entity Type Record (64 bits)
//-----------------------------------------------
struct EntityType {

   uint8_t  kind;
   uint8_t  domain;
   uint16_t country;
   uint8_t  category;
   uint8_t  subcategory;
   uint8_t  specific;
   uint8_t  extra;

   void swapBytes() {
      country = convertUInt16(country);
   };

   inline bool operator<(const EntityType& a) const {
      if (kind < a.kind)
         return true;

      if (kind == a.kind &&
         domain < a.domain) return true;

      if (kind == a.kind &&
         domain == a.domain &&
         country < a.country) return true;

      if (kind == a.kind &&
         domain == a.domain &&
         country == a.country &&
         category < a.category) return true;
      //( category < a.category && category != 0 )) return true;

      if (kind == a.kind &&
         domain == a.domain &&
         country == a.country &&
         category == a.category &&
         subcategory < a.subcategory) return true;
      //(category == a.category || category == 0 || a.category == 0) &&
      //( subcategory < a.subcategory && subcategory != 0 )) return true;

      if (kind == a.kind &&
         domain == a.domain &&
         country == a.country &&
         category == a.category &&
         subcategory == a.subcategory &&
         specific < a.specific) return true;
      //(category == a.category || category == 0 || a.category == 0) &&
      //(subcategory == a.subcategory || subcategory == 0 || a.subcategory == 0) &&
      //(specific < a.specific && specific != 0)) return true;

      if (kind == a.kind &&
         domain == a.domain &&
         country == a.country &&
         category == a.category &&
         subcategory == a.subcategory &&
         specific == a.specific &&
         extra < a.extra) return true;
      //(category == a.category || category == 0 || a.category == 0) &&
      //(subcategory == a.subcategory || subcategory == 0 || a.subcategory == 0) &&
      //(specific == a.specific || specific == 0 || a.specific == 0) &&
      //(extra < a.extra && extra != 0)) return true;

      return false;
   };

   inline bool operator==(const EntityType& a) const {
      if (kind == a.kind &&
         domain == a.domain &&
         country == a.country &&
         (category == a.category || category == 0 || a.category == 0) &&
         (subcategory == a.subcategory || subcategory == 0 || a.subcategory == 0) &&
         (specific == a.specific || specific == 0 || a.specific == 0) &&
         (extra == a.extra || extra == 0 || a.extra == 0)) return true;

      return false;
   };

   inline bool operator!=(const EntityType& a) const {
      return !(*this == a);
   };

   inline bool operator>=(const EntityType& a) const {
      return !(*this < a);
   };

   friend std::ostream& operator << ( std::ostream& s, const EntityType& v ) {
      s  << "  KD:DN:CN:CT:SC:SP:EX"
         << " -  ";

      s.width(3);
      s.fill('0');
      s << static_cast<int>(v.kind) << ":";
      s.width(3);
      s.fill('0');
      s << static_cast<int>(v.domain) << ":";
      s.width(4);
      s.fill('0');
      s << static_cast<int>(v.country) << ":";
      s.width(3);
      s.fill('0');
      s << static_cast<int>(v.category) << ":";
      s.width(3);
      s.fill('0');
      s << static_cast<int>(v.subcategory) << ":";
      s.width(3);
      s.fill('0');
      s << static_cast<int>(v.specific) << ":";
      s.width(3);
      s.fill('0');
      s << static_cast<int>(v.extra);

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const EntityType* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.79 -- Simulation Address Record
//-----------------------------------------------
struct SimulationAddressDIS{

   uint16_t siteIdentification;
   uint16_t applicationIdentification;

   // Constructors
   SimulationAddressDIS(const uint16_t a, const uint16_t b) : siteIdentification(a), applicationIdentification(b) { }
   SimulationAddressDIS() : siteIdentification(0), applicationIdentification(0) { }

   // Swap bytes 'to' or 'from' the network
   void swapBytes() {
      siteIdentification        = convertUInt16(siteIdentification);
      applicationIdentification = convertUInt16(applicationIdentification);
   }

   // Assignment operator
   void operator=(const SimulationAddressDIS& a) {
      siteIdentification = a.siteIdentification;
      applicationIdentification = a.applicationIdentification;
   }

   bool operator<(const SimulationAddressDIS& a) const {
      if (siteIdentification < a.siteIdentification) return true;

      if (siteIdentification == a.siteIdentification &&
         applicationIdentification < a.applicationIdentification) return true;

      return false;
   }

   bool operator==(const SimulationAddressDIS& a) const {
      if (siteIdentification == a.siteIdentification &&
         applicationIdentification == a.applicationIdentification) {
            return true;
      }
      return false;
   }

   bool operator!=(const SimulationAddressDIS& a) const {
      return !(*this == a);
   }

   bool operator>=(const SimulationAddressDIS& a) const {
      return !(*this < a);
   }

   friend std::ostream& operator << ( std::ostream& s, const SimulationAddressDIS& v ) {
      s  << "  siteIdentification:        " << static_cast<int>(v.siteIdentification) << std::endl
         << "  applicationIdentification: " << static_cast<int>(v.applicationIdentification) << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const SimulationAddressDIS* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};


//-----------------------------------------------
// 6.2.95 -- VectorDIS Record (96 bits)
//-----------------------------------------------
struct VectorDIS {

   float component[3];        // X, Y Z components of the vectory

   // Constructor(s)
   VectorDIS(const float x, const float y, const float z) {
      component[0] = x;
      component[1] = y;
      component[2] = z;
   }
   VectorDIS() {
      component[0] = 0;
      component[1] = 0;
      component[2] = 0;
   }

   // Compare to another VectorDIS structure
   bool operator==(const VectorDIS& s2) const {
      return (
         component[0] == s2.component[0] &&
         component[1] == s2.component[1] &&
         component[2] == s2.component[2]
      );
   }

   bool operator!=(const VectorDIS& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network
   void swapBytes() {
      component[0] = convertFloat(component[0]);
      component[1] = convertFloat(component[1]);
      component[2] = convertFloat(component[2]);
   }

   friend std::ostream& operator << ( std::ostream& s, const VectorDIS& v ) {
      s  << "  component[0]:       " << v.component[0] << std::endl
         << "  component[1]:       " << v.component[1] << std::endl
         << "  component[2]:       " << v.component[2] << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const VectorDIS* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }
};


//-----------------------------------------------
// 6.2.97 -- World Coordinates Record (192 bits)
//-----------------------------------------------
struct WorldCoordinates {

   double X_coord;
   double Y_coord;
   double Z_coord;

   WorldCoordinates(const double a, const double b, const double c) : X_coord(a), Y_coord(b), Z_coord(c) {}
   WorldCoordinates() : X_coord(0), Y_coord(0), Z_coord(0) {}

   // Assignmnent operator
   void operator=(const WorldCoordinates& _v) {
      X_coord = _v.X_coord;
      Y_coord = _v.Y_coord;
      Z_coord = _v.Z_coord;
   }

   // Compare to another WorldCoordinates structure
   bool operator==(const WorldCoordinates& s2) const {
      return (
         X_coord == s2.X_coord &&
         Y_coord == s2.Y_coord &&
         Z_coord == s2.Z_coord
         );
   }

   bool operator!=(const WorldCoordinates& a) const {
      return !(*this == a);
   }

   // Swap bytes to or from the network
   void swapBytes() {
      X_coord = convertDouble(static_cast<double>(X_coord));
      Y_coord = convertDouble(static_cast<double>(Y_coord));
      Z_coord = convertDouble(static_cast<double>(Z_coord));
   }

   friend std::ostream& operator << ( std::ostream& s, const WorldCoordinates& v ) {
      s  << "  X_coord:       " << static_cast<double>(v.X_coord) << std::endl
         << "  Y_coord:       " << static_cast<double>(v.Y_coord) << std::endl
         << "  Z_coord:       " << static_cast<double>(v.Z_coord) << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const WorldCoordinates* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};


//-----------------------------------------------
// 6.2.7 -- Angular Velocity Vector Record (96 bits)
//-----------------------------------------------
struct AngularVelocityVectorDIS {

   float x_axis;
   float y_axis;
   float z_axis;

   void swapBytes() {
      x_axis = convertFloat(x_axis);
      y_axis = convertFloat(y_axis);
      z_axis = convertFloat(z_axis);
   };

   friend std::ostream& operator << ( std::ostream& s, const AngularVelocityVectorDIS& v ) {
      s  << "  x_axis:      " << static_cast<int>(v.x_axis) << std::endl
         << "  y_axis:      " << static_cast<int>(v.y_axis) << std::endl
         << "  z_axis:      " << static_cast<int>(v.z_axis) << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const AngularVelocityVectorDIS* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.9 -- Antenna Pattern Record (320 bits)
//-----------------------------------------------
struct AntennaPattern {

   float psi;
   float theta;
   float phi;
   float azimuthBeamWidth;
   float elevationBeamWidth;
   int8_t referenceSystem;
   int8_t padding1[3];
   float ez;
   float ex;
   float phase;
   float padding2; // (IEEE P1278.1/D15 ????)

   void swapBytes() {
      psi                = convertFloat(psi);
      theta              = convertFloat(theta);
      phi                = convertFloat(phi);
      azimuthBeamWidth   = convertFloat(azimuthBeamWidth);
      elevationBeamWidth = convertFloat(elevationBeamWidth);
      ez                 = convertFloat(ez);
      ex                 = convertFloat(ex);
      phase              = convertFloat(phase);
   };

   friend std::ostream& operator << ( std::ostream& s, const AntennaPattern& v ) {
      s  << "  psi:                   " << v.psi << std::endl
         << "  theta:                 " << v.theta << std::endl
         << "  phi:                   " << v.phi << std::endl
         << "  azimuthBeamWidth:      " << v.azimuthBeamWidth << std::endl
         << "  elevationBeamWidth:    " << v.elevationBeamWidth << std::endl
         << "  referenceSystem:       " << static_cast<int>(v.referenceSystem) << std::endl
         << "  ez:                    " << v.ez << std::endl
         << "  ex:                    " << v.ex << std::endl
         << "  phase:                 " << v.phase << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const AntennaPattern* v ) {

      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};


//-----------------------------------------------
// 6.2.13 -- Beam Data Record
//-----------------------------------------------
struct BeamData {

   float beamAzimuthCenter;         // Azimuth center of sweep volume (radians)
   float beamAzimuthSweep;          // Azimuth sweep width, half angle (radians)
   float beamElevationCenter;       // Elevation center of sweep volume (radians)
   float beamElevationSweep;        // Elevation sweep height, half angle (radians)
   float beamSweepSync;             // Percent of sweep completed

   // Constructor(s)
   BeamData() :  beamAzimuthCenter(0), beamAzimuthSweep(0), beamElevationCenter(0),
      beamElevationSweep(0), beamSweepSync(0) { }

   // Assignment operator
   void operator=(const BeamData& _v) {
      beamAzimuthCenter = _v.beamAzimuthCenter;
      beamAzimuthSweep = _v.beamAzimuthSweep;
      beamElevationCenter = _v.beamElevationCenter;
      beamElevationSweep = _v.beamElevationSweep;
      beamSweepSync = _v.beamSweepSync;
   }

   // Compare to another BeamData structure
   bool operator==(const BeamData& s2) const {
      return (
            beamAzimuthCenter == s2.beamAzimuthCenter &&
            beamAzimuthSweep == s2.beamAzimuthSweep &&
            beamElevationCenter == s2.beamElevationCenter &&
            beamElevationSweep == s2.beamElevationSweep &&
            beamSweepSync == s2.beamSweepSync
         );
   }
   bool operator!=(const BeamData& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      beamAzimuthCenter        = convertFloat(beamAzimuthCenter);
      beamAzimuthSweep         = convertFloat(beamAzimuthSweep);
      beamElevationCenter      = convertFloat(beamElevationCenter);
      beamElevationSweep       = convertFloat(beamElevationSweep);
      beamSweepSync            = convertFloat(beamSweepSync);
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const BeamData& v ) {
      s  << "  beamAzimuthCenter:        " << v.beamAzimuthCenter << std::endl
         << "  beamAzimuthSweep:         " << v.beamAzimuthSweep << std::endl
         << "  beamElevationCenter:      " << v.beamElevationCenter << std::endl
         << "  beamElevationSweep:       " << v.beamElevationSweep << std::endl
         << "  beamSweepSync:            " << v.beamSweepSync << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const BeamData* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};


//-----------------------------------------------
// 6.2.16 -- Clock Time Record
//-----------------------------------------------
struct ClockTime{

   float hour;
   float timePastTheHour;

   void swapBytes() {
      hour            = convertFloat(hour);
      timePastTheHour = convertFloat(timePastTheHour);
   };

   friend std::ostream& operator << ( std::ostream& s, const ClockTime& v ) {
      s  << "hour:            " << v.hour << std::endl
         << "timePastTheHour: " << v.timePastTheHour << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const ClockTime* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.18 -- Data Query Datum Specification Record
//-----------------------------------------------
struct DataQueryDatumSpecification{
   // Not Finished
};


//-----------------------------------------------
// 6.2.19 -- Datum Specification Record
//-----------------------------------------------
struct DatumSpecification{
   // Not Finished
};


//-----------------------------------------------
// 6.2.20.2 -- Burst () Descriptor Record
//-----------------------------------------------
struct BurstDescriptor {

   EntityType munition;
   uint16_t warhead;
   uint16_t fuse;
   uint16_t quantity;
   uint16_t rate;

   void swapBytes() {
      munition.swapBytes();
      warhead  = convertUInt16(warhead);
      fuse     = convertUInt16(fuse);
      quantity = convertUInt16(quantity);
      rate     = convertUInt16(rate);
   };

   friend std::ostream& operator << ( std::ostream& s, const BurstDescriptor& v ) {
      s  << "  Munition ID:"
         << v.munition << std::endl
         << "  warhead:      " << static_cast<int>(v.warhead)  << std::endl
         << "  fuse:         " << static_cast<int>(v.fuse)     << std::endl
         << "  quantity:     " << static_cast<int>(v.quantity) << std::endl
         << "  rate:         " << static_cast<int>(v.rate)     << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const BurstDescriptor* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.22 -- EE Fundamental Parameter Data Record
//-----------------------------------------------
struct EeFundamentalParameterData {

   float frequency;                 // Center frequency (Hz)
   float frequencyRange;            // Beam width (i.e., at 3db) or zero (Hz)
   float effectiveRadiatedPower;    // Effective radiated power ( watts dB)
   float pulseRepetitiveFrequency;  // PRF (hz)
   float pulseWidth;                // Pulse Width (uSec)

   // Constructor(s)
   EeFundamentalParameterData() :
         frequency(0), frequencyRange(0), effectiveRadiatedPower(0), pulseRepetitiveFrequency(0), pulseWidth(0) { }

   // Assignment operator
   void operator=(const EeFundamentalParameterData& _v) {
      frequency = _v.frequency;
      frequencyRange = _v.frequencyRange;
      effectiveRadiatedPower = _v.effectiveRadiatedPower;
      pulseRepetitiveFrequency = _v.pulseRepetitiveFrequency;
      pulseWidth = _v.pulseWidth;
   }

   // Compare to another EeFundamentalParameterData structure
   bool operator==(const EeFundamentalParameterData& s2) const {
      return (
            frequency == s2.frequency &&
            frequencyRange == s2.frequencyRange &&
            effectiveRadiatedPower == s2.effectiveRadiatedPower &&
            pulseRepetitiveFrequency == s2.pulseRepetitiveFrequency &&
            pulseWidth == s2.pulseWidth
         );
   }
   bool operator!=(const EeFundamentalParameterData& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      frequency                = convertFloat(frequency);
      frequencyRange           = convertFloat(frequencyRange);
      effectiveRadiatedPower   = convertFloat(effectiveRadiatedPower);
      pulseRepetitiveFrequency = convertFloat(pulseRepetitiveFrequency);
      pulseWidth               = convertFloat(pulseWidth);
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const EeFundamentalParameterData& v ) {
      s  << "  frequency:                " << v.frequency << std::endl
         << "  frequencyRange:           " << v.frequencyRange << std::endl
         << "  effectiveRadiatedPower:   " << v.effectiveRadiatedPower << std::endl
         << "  pulseRepetitiveFrequency: " << v.pulseRepetitiveFrequency << std::endl
         << "  pulseWidth:               " << v.pulseWidth << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const EeFundamentalParameterData* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};


//-----------------------------------------------
// 6.2.23 -- Emitter System Record
//-----------------------------------------------
struct EmitterSystem {

   uint16_t emitterName;                  // Emitter Name enum (see IST-CF-03-01, May 5, 2003, Sec 8.1.1.1)
   uint8_t  function;                     // Function enum (see IST-CF-03-01, May 5, 2003, Sec 8.1.2)
   uint8_t  emitterIdentificationNumber;  // Unique emitter system number

   // Constructor(s)
   EmitterSystem() : emitterName(0), function(0), emitterIdentificationNumber(0) {}

   // Assignment operator
   void operator=(const EmitterSystem& _v) {
      emitterName = _v.emitterName;
      function = _v.function;
      emitterIdentificationNumber = _v.emitterIdentificationNumber;
   }

   // Compare to another EmitterSystem structure
   bool operator==(const EmitterSystem& s2) const {
      return (
         emitterName == s2.emitterName &&
         function == s2.function &&
         emitterIdentificationNumber == s2.emitterIdentificationNumber
         );
   }

   bool operator!=(const EmitterSystem& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      emitterName = convertUInt16(emitterName);
   }

   // Friendly print functions
   friend std::ostream& operator << ( std::ostream& s, const EmitterSystem& v ) {
      s  << "emitterName:                 " << static_cast<int>(v.emitterName)                 << std::endl
         << "function:                    " << static_cast<int>(v.function)                    << std::endl
         << "emitterIdentificationNumber: " << static_cast<int>(v.emitterIdentificationNumber) << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const EmitterSystem* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};


//-----------------------------------------------
// 6.2.28 -- Entity Identifier Record (48 bits)
//-----------------------------------------------
struct EntityIdentifierDIS {

   SimulationAddressDIS simulationID;      // ID of the simulation
   uint16_t             ID;                // Entity ID within the simulation

   // Constructors
   EntityIdentifierDIS() : simulationID(), ID(0)  {}
   EntityIdentifierDIS(const uint16_t a, const uint16_t b, const uint16_t c) : simulationID(a,b), ID(c) {}

   // Assignment operator
   void operator=(const EntityIdentifierDIS& a) {
      simulationID = a.simulationID;
      ID = a.ID;
   }

   void swapBytes() {
      simulationID.swapBytes();
      ID = convertUInt16(ID);
   };

   bool operator<(const EntityIdentifierDIS& a) const {
      if (simulationID.siteIdentification < a.simulationID.siteIdentification) return true;

      if (simulationID.siteIdentification == a.simulationID.siteIdentification &&
         simulationID.applicationIdentification < a.simulationID.applicationIdentification) return true;

      if (simulationID == a.simulationID && ID < a.ID) return true;

      return false;
   };

   bool operator==(const EntityIdentifierDIS& a) const {
      if (simulationID == a.simulationID && ID == a.ID) return true;
      return false;
   }

   bool operator!=(const EntityIdentifierDIS& a) const {
      return !(*this == a);
   }

   bool operator>=(const EntityIdentifierDIS& a) const {
      return !(*this < a);
   }

   friend std::ostream& operator << ( std::ostream& s, const EntityIdentifierDIS& v ) {
      s  << "Sim ID:" << std::endl
         << v.simulationID
         << "  entityID:                  " << static_cast<int>(v.ID) << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const EntityIdentifierDIS* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }
};


//-----------------------------------------------
// 6.2.29 -- Entity Marking Record (96 bits)
//-----------------------------------------------
struct EntityMarking {

   static const uint32_t BUFF_SIZE = 11;
   uint8_t characterSet;         // Marking character set
   uint8_t marking[BUFF_SIZE];   // Entity marking buffer

   friend std::ostream& operator << ( std::ostream& s, const EntityMarking& v ) {
      s  << "  Character Set:   " << static_cast<int>(v.characterSet) << std::endl
         << "  Markings:        " << v.marking << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const EntityMarking* v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.33 -- Euler Angles Record (96)
//-----------------------------------------------
struct EulerAngles{

   float psi;
   float theta;
   float phi;

   EulerAngles() {
      psi   = 0.0;
      theta = 0.0;
      phi   = 0.0;
   };

   EulerAngles(float a,float b,float c) {
      psi   = a;
      theta = b;
      phi   = c;
   };

   ~EulerAngles() {};

   void swapBytes() {
      psi   = convertFloat(psi);
      theta = convertFloat(theta);
      phi   = convertFloat(phi);
   };

   friend std::ostream& operator << ( std::ostream& s, const EulerAngles& v ) {
      s  << "  psi:          " << v.psi << std::endl
         << "  theta:        " << v.theta << std::endl
         << "  phi:          " << v.phi << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const EulerAngles* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };

   inline void operator =(const EulerAngles& _s) {
      psi   = _s.psi;
      theta = _s.theta;
      phi   = _s.phi;
   };
};


//-----------------------------------------------
// 6.2.34 -- Event Identifier Record (48 bits)
//-----------------------------------------------
struct EventIdentifier {

   SimulationAddressDIS simulationID;
   uint16_t             eventNumber;

   void swapBytes() {
      simulationID.swapBytes();
      eventNumber = convertUInt16(eventNumber);
   };

   friend std::ostream& operator << ( std::ostream& s, const EventIdentifier& v ) {
      s  << "  Event Identifier:" << std::endl
         << v.simulationID
         << "  eventNumber:     " << static_cast<int>(v.eventNumber) << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const EventIdentifier* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.38 -- Fixed Datum Record
//-----------------------------------------------
struct FixedDatum {

   uint32_t fixedDatumID;
   uint32_t fixedDatumValue;

   void swapBytes() {
      fixedDatumID    = convertUInt32(fixedDatumID);
      fixedDatumValue = convertUInt32(fixedDatumValue);
   };
};


//-----------------------------------------------
// 6.2.40 -- Fundamental Op Data (128 bits)
//-----------------------------------------------
struct FundamentalOpData {

   uint8_t  systemStatus;
   uint8_t  alternateParam4;
   uint8_t  informationLayers;
   uint8_t  modifier;
   uint16_t param1;
   uint16_t param2;
   uint16_t param3;
   uint16_t param4;
   uint16_t param5;
   uint16_t param6;

   FundamentalOpData() {
      systemStatus = 0;
      alternateParam4 = 0;
      informationLayers = 0;
      modifier = 0;
      param1 = 0;
      param2 = 0;
      param3 = 0;
      param4 = 0;
      param5 = 0;
      param6 = 0;
   };

   void swapBytes() {
      param1 = convertUInt16(param1);
      param2 = convertUInt16(param2);
      param3 = convertUInt16(param3);
      param4 = convertUInt16(param4);
      param5 = convertUInt16(param5);
      param6 = convertUInt16(param6);
   };

   friend std::ostream& operator << ( std::ostream &s, const FundamentalOpData &v ) {
      s  << "  System Status:      " << static_cast<int>(v.systemStatus)      << std::endl
         << "  Alternate Param4:   " << static_cast<int>(v.alternateParam4)   << std::endl
         << "  Information Layers: " << static_cast<int>(v.informationLayers) << std::endl
         << "  Modifier:           " << static_cast<int>(v.modifier)          << std::endl;

      s.width(2);
      s.fill('0');
      s << "    Parameter 1:    " << std::hex << static_cast<int>(v.param1) << std::endl;

      s.width(2);
      s.fill('0');
      s << "    Parameter 2:    " << std::hex << static_cast<int>(v.param2) << std::endl;

      s.width(2);
      s.fill('0');
      s << "    Parameter 3:    " << std::hex << static_cast<int>(v.param3) << std::endl;

      s.width(2);
      s.fill('0');
      s << "    Parameter 4:    " << std::hex << static_cast<int>(v.param4) << std::endl;

      s.width(2);
      s.fill('0');
      s << "    Parameter 5:    " << std::hex << static_cast<int>(v.param5) << std::endl;

      s.width(2);
      s.fill('0');
      s << "    Parameter 6:    " << std::hex << static_cast<int>(v.param6) << std::endl;

      s << std::dec;
      return s;
   };

   friend std::ostream& operator << ( std::ostream &s, const FundamentalOpData *v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.50 -- JammingTechnique Record (IEEE P1278.1/D15)
//
// Use getJammingModeSequence() and setJammingModeSequence()
// to access the older 32 bit jamming mode sequence value
//-----------------------------------------------
struct JammingTechnique {

   uint8_t kind;                // Jamming technique kind
   uint8_t category;            // Jamming technique category
   uint8_t subcat;              // Jamming technique subcategory
   uint8_t specific;            // Jamming technique specific

   // Constructor(s)
   JammingTechnique() : kind(0), category(0), subcat(0), specific(0) {}

   // Returns the jamming mode sequence (pre P1278.1/D15)
   uint32_t getJammingModeSequence() const {
      convData value;
      value.byte[0] = kind;
      value.byte[1] = category;
      value.byte[2] = subcat;
      value.byte[3] = specific;

      if (checkSwap())
         return convertUInt32(value.uint);
      else
         return value.uint;
   }

   // Sets the jamming mode sequence (pre P1278.1/D15)
   void setJammingModeSequence(const uint32_t value0)  {
      convData value;
      if (checkSwap())
         value.uint = convertUInt32(value0);
      else
         value.uint = value0;

      kind     = value.byte[0];
      category = value.byte[1];
      subcat   = value.byte[2];
      specific = value.byte[3];
   }


   // Assignment operator
   void operator=(const JammingTechnique& _v) {
      kind = _v.kind;
      category = _v.category;
      subcat = _v.subcat;
      specific = _v.specific;
   }

   // Compare to another EmitterSystem structure
   bool operator==(const JammingTechnique& s2) const {
      return (
         kind == s2.kind &&
         category == s2.category &&
         subcat == s2.subcat &&
         specific == s2.specific
         );
   }
   bool operator!=(const JammingTechnique& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      // Jamming technique 8 bit int's do not need to be swapped,
      // and the 32 bit jamming mode sequence value will be swapped
      // by getJammingModeSequence() and setJammingModeSequence().
   }

   // Friendly print functions
   friend std::ostream& operator << ( std::ostream& s, const JammingTechnique& v ) {
      s  << "kind:                 "        << static_cast<long>(v.kind)     << std::endl
         << "category:                    " << static_cast<long>(v.category) << std::endl
         << "subcat: "                      << static_cast<long>(v.subcat)   << std::endl
         << "specific: "                    << static_cast<long>(v.specific) << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const JammingTechnique* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};


//-----------------------------------------------
//  6.2.59 -- Modulation Type Record (64 bits)
//-----------------------------------------------
struct ModulationType {

   uint16_t spreadSpectrum;         // Spread Spectrum
   uint16_t majorModulationType;    // Major Modulation type
   uint16_t detail;                 // Detail
   uint16_t system;                 // system

   // Constructor(s)
   ModulationType() : spreadSpectrum(0), majorModulationType(0), detail(0), system(0) {}

   // Assignment operator
   void operator=(const ModulationType& _v) {
      spreadSpectrum = _v.spreadSpectrum;
      majorModulationType = _v.majorModulationType;
      detail = _v.detail;
      system = _v.system;
   }

   // Compare to another ModulationType structure
   bool operator==(const ModulationType& s2) const {
      return (
         spreadSpectrum == s2.spreadSpectrum &&
         majorModulationType == s2.majorModulationType &&
         detail == s2.detail &&
         system == s2.system
         );
   }

   bool operator!=(const ModulationType& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      spreadSpectrum      = convertUInt16(spreadSpectrum);
      majorModulationType = convertUInt16(majorModulationType);
      detail              = convertUInt16(detail);
      system              = convertUInt16(system);
   };

   friend std::ostream& operator << ( std::ostream& s, const ModulationType& v ) {
      s  << "  spreadSpectrum:      " << v.spreadSpectrum << std::endl
         << "  majorModulationType: " << v.majorModulationType << std::endl
         << "  detail:              " << v.detail << std::endl
         << "  system:              " << v.system << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const ModulationType* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};


//-----------------------------------------------
// 6.2.66 -- PDU Header Record (96 bits)
//-----------------------------------------------
struct PDUHeader {

   uint8_t  protocolVersion;
   uint8_t  exerciseIdentifier;
   uint8_t  PDUType;
   uint8_t  protocolFamily;
   uint32_t timeStamp;
   uint16_t length;
   uint8_t  status;     // IEEE P1278.1/D15
   uint8_t  padding;

   void swapBytes() {
      timeStamp = convertUInt32(timeStamp);
      length    = convertUInt16(length);
   };

   friend std::ostream& operator << ( std::ostream& s, const PDUHeader& v ) {
      s  << "  protocolVersion:     " << int(v.protocolVersion) << std::endl
         << "  exerciseIdentifier:  " << int(v.exerciseIdentifier) << std::endl
         << "  PDUType:             " << int(v.PDUType) << std::endl
         << "  protocolFamily:      " << int(v.protocolFamily) << std::endl
         << "  timeStamp:           " << v.timeStamp << std::endl
         << "  length:              " << v.length << std::endl
         << "  status:              " << v.status << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const PDUHeader* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.70 -- Radio Entity Type Record (64 bits)
//-----------------------------------------------
struct RadioEntityType {

   uint8_t  kind;                // Kind
   uint8_t  domain;              // Domain
   uint16_t country;             // Country
   uint8_t  category;            // Category
   uint8_t  nomenclatureVersion; // Nomenclature Version
   uint16_t nomenclature;        // Nomenclature

   // Constructor(s)
   RadioEntityType() : kind(0), domain(0), country(0), category(0), nomenclatureVersion(0), nomenclature(0) {}

   // Assignmnent operator
   void operator=(const RadioEntityType& _v) {
      kind = _v.kind;
      domain = _v.domain;
      country = _v.country;
      category = _v.category;
      nomenclatureVersion = _v.nomenclatureVersion;
      nomenclature = _v.nomenclature;
   }

   // Compare to another RadioEntityType structure
   bool operator==(const RadioEntityType& s2) const {
      return (
         kind == s2.kind &&
         domain == s2.domain &&
         country == s2.country &&
         category == s2.category &&
         nomenclatureVersion == s2.nomenclatureVersion &&
         nomenclature == s2.nomenclature
         );
   }

   bool operator!=(const RadioEntityType& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      country      = convertUInt16(country);
      nomenclature = convertUInt16(nomenclature);
   };

   friend std::ostream& operator << ( std::ostream& s, const RadioEntityType& v ) {
      s  << "  kind:                " << static_cast<int>(v.kind)                << std::endl
         << "  domain:              " << static_cast<int>(v.domain)              << std::endl
         << "  country:             " << static_cast<int>(v.country)             << std::endl
         << "  category:            " << static_cast<int>(v.category)            << std::endl
         << "  nomenclatureVersion: " << static_cast<int>(v.nomenclatureVersion) << std::endl
         << "  nomenclature:        " << static_cast<int>(v.nomenclature)        << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const RadioEntityType* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};


//-----------------------------------------------
// 6.2.81 -- Simulation Management PDU Header Record
//-----------------------------------------------
struct SimulationManagementPDUHeader{

   PDUHeader        header;
   EntityIdentifierDIS originatingEntityID;
   EntityIdentifierDIS receivingEntityID;

   void swapBytes() {
      header.swapBytes();
      originatingEntityID.swapBytes();
      receivingEntityID.swapBytes();
   };

   friend std::ostream& operator << ( std::ostream& s, const SimulationManagementPDUHeader& v ) {
      s  << "Header:" << std::endl
         << v.header << std::endl
         << "Originating Entity ID:" << std::endl
         << v.originatingEntityID << std::endl
         << "Receiving Entity ID:" << std::endl
         << v.receivingEntityID << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const SimulationManagementPDUHeader* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.85 -- Supply Quantity Record
//-----------------------------------------------
struct SupplyQuantity{

   EntityType supplyType;
   float      quantity;

   void swapBytes() {
      supplyType.swapBytes();
      quantity = convertFloat(quantity);
   };

   friend std::ostream& operator << ( std::ostream& s, const SupplyQuantity& v ) {
      s  << "Supply Type:" << std::endl
         << v.supplyType << std::endl
         << "Quantity:" << std::endl
         << v.quantity << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const SupplyQuantity* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.86 -- System ID
//-----------------------------------------------
struct SystemID {

   uint16_t systemType;
   uint16_t systemName;
   uint8_t  systemMode;
   uint8_t  options;

   void swapBytes() {
      systemType = convertUInt16(systemType);
      systemName = convertUInt16(systemName);
   };

   friend std::ostream& operator << ( std::ostream &s, const SystemID &v ) {
      s  << "  System Type:    " << v.systemType << std::endl
         << "  System Name:    " << v.systemName << std::endl;

      s.width(2);
      s.fill('0');
      s << "  System Mode:    " << std::hex << static_cast<int>(v.systemMode) << std::endl;

      s.width(2);
      s.fill('0');
      s << "  Change/Options: " << static_cast<int>(v.options) << std::dec << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream &s, const SystemID *v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// 6.2.89 -- Fundamental Parameter Data Record
//-----------------------------------------------
struct TrackJamTargets {

   EntityIdentifierDIS targetID; // Track/Jam target ID
   uint8_t emitterID;      // Target's emitter ID being jammed
   uint8_t beamID;         // Target's beam ID being jammed

   // Constructor(s)
   TrackJamTargets() : targetID(), emitterID(0), beamID(0) {}

   // Assignment operator
   void operator=(const TrackJamTargets& _v) {
      targetID = _v.targetID;
      emitterID = _v.emitterID;
      beamID = _v.beamID;
   }

   // Compare to another TrackJamTargets structure
   bool operator==(const TrackJamTargets& s2) const
   {
      return (
         targetID == s2.targetID &&
         emitterID == s2.emitterID &&
         beamID == s2.beamID
      );
   }

   bool operator!=(const TrackJamTargets& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      targetID.swapBytes();
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const TrackJamTargets& v ) {
      s  << "Target ID:       " << std::endl << v.targetID
         << "  emitterID:     " << static_cast<int>(v.emitterID) << std::endl
         << "  beamID:        " << static_cast<int>(v.beamID)    << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const TrackJamTargets* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};


//-----------------------------------------------
// 6.2.89 -- Variable Datum Record
//-----------------------------------------------
struct VariableDatum {

   uint32_t   variableDatumID;
   uint32_t   variableDatumLength;

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      variableDatumID     = convertUInt32(variableDatumID);
      variableDatumLength = convertUInt32(variableDatumLength);
   };

   unsigned int getSize() {
      return variableDatumLength/8;
   };

   // Temp connection to data, better way...
   uint8_t* getData(){ return reinterpret_cast<uint8_t*>(&variableDatumLength) + sizeof(long); };

   friend std::ostream& operator << ( std::ostream& s, const VariableDatum&  v) {
      s  << "  variableDatumID:       " << static_cast<int>(v.variableDatumID) << std::endl
         << "  variableDatumLength:   " << static_cast<int>(v.variableDatumLength) << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const VariableDatum* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};


//-----------------------------------------------
// 6.2.93.2 -- Variable Parameter (VP) Articulated Part (128 bits)
//-----------------------------------------------
struct VpArticulatedPart {

   // Parameter Type Designator; SISO-REF-010-2005, 25 March 2005
   enum {
      ARTICULATED_PART,       // Articulated Part
      ATTACHED_PART           // Attached Part
   };

   // Articulated Parts; SISO-REF-010-2005, 25 March 2005
   enum {
      RUDDER         = 1024,        // rudder
      LEFT_FLAP      = 1056,        // left flap
      RIGHT_FLAP     = 1088,        // right flap
      LEFT_AILERON   = 1120,        // left aileron
      RIGHT_AILERON  = 1152,        // right aileron
      LANDING_GEAR   = 3072,        // Gear position
      TAIL_HOOK      = 3104,        // tail hook
      SPEED_BRAKE    = 3136,        // speed brake
      LEFT_WEAPON_BAY_DOOR = 3168,  // Left Weapon Bay Door
      RIGHT_WEAPON_BAY_DOOR = 3200, // Right Weapon Bay Door
      WING_SWEEP = 3264,            // Wing sweep
      PRIMARY_LAUNCHER_1 = 4736,    // Primary launcher 1
   };

   // Articulated-parts Parameters; SISO-REF-010-2005, 25 March 2005
   enum {
      POSITION = 1,
      POSITION_RATE = 2,
      EXTENSION = 3,
      EXTENSION_RATE = 4,
      X = 5,
      X_RATE = 6,
      Y = 7,
      Y_RATE = 8,
      Z = 9,
      Z_RATE = 10,
      AZIMUTH = 11,
      AZIMUTH_RATE = 12,
      ELEVATION = 13,
      ELEVATION_RATE = 14,
      ROTATION = 15,
      ROTATION_RATE = 16
   };

   // data structure
   uint8_t  parameterTypeDesignator;
   uint8_t  changeIndicator;
   uint16_t id;
   uint32_t parameterType;
   union ParameterValue {
      float value[2];
      EntityType entityType;
   } parameterValue;

   void swapBytes() {
      id = convertUInt16(id);
      parameterType = convertUInt32(parameterType);
      if (parameterTypeDesignator == ATTACHED_PART) {
         parameterValue.entityType.swapBytes();
      }
      else {
         parameterValue.value[0] = convertFloat(parameterValue.value[0]);
         parameterValue.value[1] = convertFloat(parameterValue.value[1]);
      }
   };

   friend std::ostream& operator << ( std::ostream& s, const VpArticulatedPart& v ) {
      s  << "  parameterTypeDesignator: " << static_cast<int>(v.parameterTypeDesignator) << std::endl
         << "  changeIndicator:         " << static_cast<int>(v.changeIndicator)         << std::endl
         << "  id:                      " << static_cast<int>(v.id)                      << std::endl
         << "  parameterType:           " << static_cast<int>(v.parameterType)           << std::endl;

      if (v.parameterTypeDesignator == ATTACHED_PART)
         s << v.parameterValue.entityType << std::endl;
      else
         s << v.parameterValue.value[0] << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const VpArticulatedPart* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------
// 7.6.2 -- Emitter Beam Data (part of Electromagnetic Emission PDU)
//-----------------------------------
struct EmitterBeamData {

   // Enum for highDensityTracks, (see IST-CF-03-01, May 5, 2003, Sec 8.1.5)
   enum { NOT_SELECTED, SELECTED };

   uint8_t            beamDataLength;        // Length of this beam data, in 32bit words, including the track/jam targets
   uint8_t            beamIDNumber;          // Unique ID for this beam
   uint16_t           beamParameterIndex;    // Use for the lookup of stored database parameters
   EeFundamentalParameterData parameterData; // Parameter data
   BeamData           beamData;              // Beam data
   uint8_t            beamFunction;          // Beam function enum (see IST-CF-03-01, May 5, 2003, Sec 8.1.4)
   uint8_t            numberOfTargetsInTrack; // Number of track/jam targets that will follow
   uint8_t            highDensityTracks;     // High Density Tracks flag
   uint8_t            beamStatus;            // Beam active/inactive flag (IEEE P1278.1/D15; see 6.2.14)
   JammingTechnique   jammingTechnique;      //  (IEEE P1278.1/D15)

   // Constructor(s)
   EmitterBeamData() :
      beamDataLength(0), beamIDNumber(0), beamParameterIndex(0), parameterData(),
      beamData(),
      beamFunction(0), numberOfTargetsInTrack(0), highDensityTracks(0), beamStatus(0),
      jammingTechnique() { }

   // Assignment operator
   void operator=(const EmitterBeamData& _v) {
      beamDataLength = _v.beamDataLength;
      beamIDNumber = _v.beamIDNumber;
      beamParameterIndex = _v.beamParameterIndex;
      parameterData = _v.parameterData;
      beamData = _v.beamData;
      beamFunction = _v.beamFunction;
      numberOfTargetsInTrack = _v.numberOfTargetsInTrack;
      highDensityTracks = _v.highDensityTracks;
      beamStatus = _v.beamStatus;
      jammingTechnique = _v.jammingTechnique;
   }

   // Returns a pointer to the idx'th TrackJamTarget structure;
   // Note: This ONLY works after the numberOfTargetsInTrack have been initialized!
   // Note: 'idx' is zero based, so a idx == 1 will return the second TrackJamTargets structure
   TrackJamTargets* getTrackTarget(const int idx) {
      TrackJamTargets* tjt = nullptr;
      if (idx < numberOfTargetsInTrack) {
         uint8_t* p = reinterpret_cast<uint8_t*>(this) + sizeof(*this) + (sizeof(TrackJamTargets) * idx);
         tjt = reinterpret_cast<TrackJamTargets*>(p);
      }
      return tjt;
   }
   const TrackJamTargets* getTrackTarget(const int idx) const {
      const TrackJamTargets* tjt = nullptr;
      if (idx < numberOfTargetsInTrack) {
         uint8_t* p = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(this)) + sizeof(*this) + (sizeof(TrackJamTargets) * idx);
         tjt = (const TrackJamTargets*) p;
      }
      return tjt;
   }

   // Compare to another EmitterBeamData structure
   bool operator==(const EmitterBeamData& s2) const {
      return (
         beamDataLength == s2.beamDataLength &&
         beamIDNumber == s2.beamIDNumber &&
         beamParameterIndex == s2.beamParameterIndex &&
         parameterData == s2.parameterData &&
         beamData == s2.beamData &&
         beamFunction == s2.beamFunction &&
         numberOfTargetsInTrack == s2.numberOfTargetsInTrack &&
         highDensityTracks == s2.highDensityTracks &&
         beamStatus == s2.beamStatus &&
         jammingTechnique == s2.jammingTechnique
         );
   }
   bool operator!=(const EmitterBeamData& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      // Swap our stuff first
      beamParameterIndex  = convertUInt16(beamParameterIndex);
      parameterData.swapBytes();
      beamData.swapBytes();
      jammingTechnique.swapBytes();

      // Then swap the "targets in track" data
      for (int i = 0; i < numberOfTargetsInTrack; i++) {
         TrackJamTargets* tjt = getTrackTarget(i);
         if (tjt != nullptr) tjt->swapBytes();
      }
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const EmitterBeamData& v ) {
      s  << "  beamDataLength:         " << static_cast<long>(v.beamDataLength)         << std::endl
         << "  beamIDNumber:           " << static_cast<long>(v.beamIDNumber)           << std::endl
         << "  beamParameterIndex:     " << static_cast<long>(v.beamParameterIndex)     << std::endl
         << "Parameter Data:           " << std::endl << v.parameterData                << std::endl
         << "  beamData:               " << std::endl << v.beamData                     << std::endl
         << "  beamFunction:           " << static_cast<long>(v.beamFunction)           << std::endl
         << "  numberOfTargetsInTrack: " << static_cast<long>(v.numberOfTargetsInTrack) << std::endl
         << "  highDensityTracks:      " << static_cast<long>(v.highDensityTracks)      << std::endl
         << "  jammingTechnique:       " << v.jammingTechnique                          << std::endl;

      for(int i=0; i < v.numberOfTargetsInTrack; i++)
      {
         const TrackJamTargets* tgt = v.getTrackTarget(i);
         if (tgt != nullptr) {
            s  << "-----------------------------------------" << std::endl
               << "Target: " << i
               << tgt
               << "-----------------------------------------" << std::endl;
         }
      }

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const EmitterBeamData* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }
};


//-----------------------------------
// 7.6.2 -- Emission System Data (part of Electromagnetic Emission PDU)
//-----------------------------------
struct EmissionSystem {

   uint8_t        systemDataLength;    // Length of this system data, in 32bit words, including the Emitter Beam Data
   uint8_t        numberOfBeams;       // Number of beams (EmitterBeamData's that follow)
   uint16_t       padding1;
   EmitterSystem  emitterSystem;       // Description of the Emitter System
   VectorDIS      location;            // Location of emitter system; entities ref system (meters)

   // Constructor(s)
   EmissionSystem() : systemDataLength(0), numberOfBeams(0), padding1(0), emitterSystem(), location() {}

   // Assignment operator (does NOT copy the beams which follow the EmissionSystem structure)
   void operator=(const EmissionSystem& _v) {
      systemDataLength = _v.systemDataLength;
      numberOfBeams = _v.numberOfBeams;
      emitterSystem = _v.emitterSystem;
      location = _v.location;
   }

   // Compare to another EmissionSystem structure
   bool operator==(const EmissionSystem& s2) const {
      return (
         // systemDataLength == s2.systemDataLength  // Don't compare the total length
         numberOfBeams == s2.numberOfBeams &&
         emitterSystem == s2.emitterSystem &&
         location == s2.location
         );
   }

   bool operator!=(const EmissionSystem& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      // Swap our stuff first
      emitterSystem.swapBytes();
      location.swapBytes();

      // Then swap emission data
      for(int i = 0; i < numberOfBeams; i++) {
         EmitterBeamData* bd = getEmitterBeamData(i);
         if (bd != 0) bd->swapBytes();
      }
   }

   // Returns a pointer to the idx'th EmitterBeamData structure;
   // Note: This ONLY works after the emitter beam data structures have been created and initialized!
   //   (The beamDataLength's in the EmitterBeamData structures, as well as our
   //    'numberOfBeams' member variable, need to be correct for this to work)
   // Note: 'idx' is zero based, so a idx == 1 will return the second EmitterBeamData structure
   EmitterBeamData* getEmitterBeamData(const int idx) {
      EmitterBeamData* bd = nullptr;
      if (idx < numberOfBeams) {
         uint8_t *p = (reinterpret_cast<uint8_t*>(this)) + sizeof(*this);

         // First Emitter beam data is just after this structure
         bd = reinterpret_cast<EmitterBeamData*>(p);

         for (int i = 0; i < idx; i++) {
            // step down to the correct beam data using 'beamDataLength'
            p += (bd->beamDataLength * 4);
            bd = reinterpret_cast<EmitterBeamData*>(p);
         }

      }
      return bd;
   }

   const EmitterBeamData* getEmitterBeamData(const int idx) const {
      const EmitterBeamData* bd = nullptr;
      if (idx < numberOfBeams) {
         uint8_t *p = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(this)) + sizeof(*this);

         // First Emitter beam data is just after this structure
         bd = reinterpret_cast<const EmitterBeamData*>(p);

         for (int i = 0; i < idx; i++) {
            // step down to the correct beam data using 'beamDataLength'
            p += (bd->beamDataLength * 4);
            bd = reinterpret_cast<const EmitterBeamData*>(p);
         }

      }
      return bd;
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const EmissionSystem& v ) {
      s  << "  systemDataLength:       " << static_cast<int>(v.systemDataLength) << std::endl
         << "  numberOfBeams:          " << static_cast<int>(v.numberOfBeams)    << std::endl
         << "Emitter System:           " << std::endl << v.emitterSystem         << std::endl
         << "Reference Location:       " << std::endl << v.location              << std::endl;

      for (unsigned int i = 0; i < v.numberOfBeams; i++) {
         const EmitterBeamData* bd = v.getEmitterBeamData(i);
         s  << "-----------------------------------------" << std::endl
            << "Emitter Beam: " << i
            << bd
            << "-----------------------------------------" << std::endl;
      }

      return s;
   }

   // Another friendly print functions
   friend std::ostream& operator << ( std::ostream& s, const EmissionSystem* const v ) {
      if ( v == nullptr )
         return s;
      else {
         s << *v;
      }
      return s;
   }
};


#if 0 /* Old stuff -- keep around for a while */

//-----------------------------------------------
// Spherical Harmonic Antenna Pattern Record
//-----------------------------------------------
struct SphericalHarmonicAntennaPattern {
public:
   // Not Finished
};

//-----------------------------------------------
// Sim Euler Angles
//-----------------------------------------------
struct SimEulerAngles{
public:

   float roll;
   float pitch;
   float hdg;

   SimEulerAngles() {};

   SimEulerAngles(float a,float b,float c){
      roll  = a;
      pitch = b;
      hdg   = c;
   };

   ~SimEulerAngles() {};

   void swapBytes() {
      roll  = convertFloat(roll);
      pitch = convertFloat(pitch);
      hdg   = convertFloat(hdg);
   };

   friend std::ostream& operator << ( std::ostream& s, const SimEulerAngles& v ) {
      s  << "  roll:         " << v.roll << std::endl
         << "  pitch:        " << v.pitch << std::endl
         << "  hdg:          " << v.hdg << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const SimEulerAngles* v ) {
      if ( !v )
         return s;
      else {
         s << *v;
      }
      return s;
   };

   inline void operator =(const SimEulerAngles& _s) {
      roll  = _s.roll;
      pitch = _s.pitch;
      hdg   = _s.hdg;
   };
};


//-----------------------------------------------
// Sim Coordinates Record
//-----------------------------------------------
struct SimCoordinates {

   double lat;       // Latitude
   double lon;       // Longitude
   double alt;       // Altitude

   // Constructors
   SimCoordinates(double a,double b,double c) : lat(a), lon(b), alt(c) {}
   SimCoordinates() : lat(0), lon(0), alt(0) {}

   // Assignment operator
   void operator=(const SimCoordinates& _v) {
      lat = _v.lat;
      lon = _v.lon;
      alt = _v.alt;
   }

   // Compare to another SimCoordinates structure
   bool operator==(const SimCoordinates& s2) const {
      return (
         lat == s2.lat &&
         lon == s2.lon &&
         alt == s2.alt
         );
   }

   bool operator!=(const SimCoordinates& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      lat = convertDouble(lat);
      lon = convertDouble(lon);
      alt = convertDouble(alt);
   }

   friend std::ostream& operator << ( std::ostream& s, const SimCoordinates& v ) {
      s  << "  lat:       " << static_cast<double>(v.lat) << std::endl
         << "  lon:       " << static_cast<double>(v.lon) << std::endl
         << "  alt:       " << static_cast<double>(v.alt) << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const SimCoordinates* const v )
   {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};

#endif

} // End dis namespace
} // End network namespace
} // End oe namespace

#endif
