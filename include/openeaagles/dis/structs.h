//--------------------------------------------------------------
// IEEE Standard for Distributed Interactive Simulation (DIS)
// Application Protocols - Record Structures for DIS PDU's
//--------------------------------------------------------------
#ifndef _1278_1a_STRUCT_H_
#define _1278_1a_STRUCT_H_

#include <iostream>

namespace Eaagles {
namespace Network {
namespace Dis {

struct simulationAddressDIS;
struct entityIdentifierDIS;

class EntityType;
class AngularVelocityvectorDIS;
class BeamAntennaPattern;
class SphericalHarmonicAntennaPattern;
class ArticulationParameter;
class BurstDescriptor;
class ClockTime;
class DataQueryDatumSpecification;
class DatumSpecification;
class EulerAngles;
class EventIdentifier;
class FixedDatum;
class PDUHeader;
class SimulationManagementPDUHeader;
class SupplyQuantity;
class SystemID;
class FundamentalOpData;
struct VariableDatum;

struct EntityMarking;
struct RadioEntityType;
struct ModulationType;
struct vectorDIS;
struct SimCoordinates;
struct WorldCoordinates;
struct TrackJamTargets;
struct FundamentalParameterData;
struct EmitterBeamData;
struct EmitterSystem;
struct EmissionSystem;

//--------------------------------------------------------------
// Byte swap functions used by this file
//--------------------------------------------------------------

inline int checkSwap()
{
  unsigned short s;
  unsigned char *c;
 
  s = 1;
  c = (unsigned char *) &s;
 
  if (*c == 0)
    return 0;
 
  return 1;
}

union convDoubleData
{
  Integer64   llword;
  LCuint64  ullword;
  double    dword;
  char      byte[8];
};

union convData
{
  long    lword;
  unsigned int uint;
  float   fword;
  short   sword[2];
  unsigned short usword[2];
  char    byte[4];
};

                                                                                                           
inline long convertLong(const long v){
  convData value;
  value.lword = v;
  convData newValue;
 
  newValue.byte[3] = value.byte[0];
  newValue.byte[2] = value.byte[1];
  newValue.byte[1] = value.byte[2];
  newValue.byte[0] = value.byte[3];
 
  return newValue.lword;
}

inline unsigned int convertUInt(const unsigned int v){
  convData value;
  value.uint = v;
  convData newValue;
 
  newValue.byte[3] = value.byte[0];
  newValue.byte[2] = value.byte[1];
  newValue.byte[1] = value.byte[2];
  newValue.byte[0] = value.byte[3];
 
  return newValue.uint;
}
 
inline float convertFloat(const float v){
  convData value;
  value.fword = v;
  convData newValue;
 
  newValue.byte[3] = value.byte[0];
  newValue.byte[2] = value.byte[1];
  newValue.byte[1] = value.byte[2];
  newValue.byte[0] = value.byte[3];
 
  return newValue.fword;
}

inline Integer64 convertLongLong(const Integer64 v){
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

inline LCuint64 convertULongLong(const LCuint64 v){
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

inline double convertDouble(const double v){
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
 
inline short convertShort(const short v)
{
  convData value;
  value.sword[0] = v;
  convData newValue;
 
  newValue.byte[1] = value.byte[0];
  newValue.byte[0] = value.byte[1];
 
  return newValue.sword[0];
}
 
inline unsigned short convertUShort(const unsigned short v)
{
  convData value;
  value.usword[0] = v;
  convData newValue;
 
  newValue.byte[1] = value.byte[0];
  newValue.byte[0] = value.byte[1];
 
  return newValue.usword[0];
}

//-----------------------------------------------
// Simulation Address Record
//-----------------------------------------------
struct simulationAddressDIS{

   unsigned short siteIdentification;
   unsigned short applicationIdentification;
   
   // Constructors
   simulationAddressDIS(const unsigned short a, const unsigned short b) : siteIdentification(a), applicationIdentification(b) { }
   simulationAddressDIS() : siteIdentification(0), applicationIdentification(0) { }

   // Swap bytes 'to' or 'from' the network
   void swapBytes() {
      siteIdentification        = convertUShort(siteIdentification);
      applicationIdentification = convertUShort(applicationIdentification);
   }

   // Assignment operator
   void operator=(const simulationAddressDIS& a) {
      siteIdentification = a.siteIdentification;
      applicationIdentification = a.applicationIdentification;
   }

   bool operator<(const simulationAddressDIS& a) const {
      if (siteIdentification < a.siteIdentification) return true;

      if (siteIdentification == a.siteIdentification &&
          applicationIdentification < a.applicationIdentification) return true;

      return false;
   }

   bool operator==(const simulationAddressDIS& a) const {
      if (siteIdentification == a.siteIdentification &&
            applicationIdentification == a.applicationIdentification) {
         return true;
      }
      return false;
   }

   bool operator!=(const simulationAddressDIS& a) const {
      return !(*this == a);
   }

   bool operator>=(const simulationAddressDIS& a) const {
      return !(*this < a);
   }

   friend std::ostream& operator << ( std::ostream& s, const simulationAddressDIS& v ) {
      s << "  siteIdentification:        " << (long)v.siteIdentification << std::endl
        << "  applicationIdentification: " << (long)v.applicationIdentification << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const simulationAddressDIS* const v ) {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};


//-----------------------------------------------
// Entity Identifier Record
//-----------------------------------------------
struct entityIdentifierDIS{

   simulationAddressDIS simulationID;      // ID of the simulation
   unsigned short    ID;                   // Entity ID within the simulation
   
   // Constructors
   entityIdentifierDIS(const unsigned short a, const unsigned short b, const unsigned short c) : simulationID(a,b), ID(c) {}
   entityIdentifierDIS() : simulationID(), ID(0)  {}

   // Assignment operator
   void operator=(const entityIdentifierDIS& a) {
      simulationID = a.simulationID;
      ID = a.ID;           
   }

   void swapBytes(){
      simulationID.swapBytes();
      ID = convertUShort(ID);
   };

   bool operator<(const entityIdentifierDIS& a) const {
      if (simulationID.siteIdentification < a.simulationID.siteIdentification) return true;

      if (simulationID.siteIdentification == a.simulationID.siteIdentification &&
          simulationID.applicationIdentification < a.simulationID.applicationIdentification) return true;

      if (simulationID == a.simulationID && ID < a.ID) return true;

      return false;
   };

   bool operator==(const entityIdentifierDIS& a) const {
      if (simulationID == a.simulationID && ID == a.ID) return true;
      return false;
   }

   bool operator!=(const entityIdentifierDIS& a) const {
      return !(*this == a);
   }

   bool operator>=(const entityIdentifierDIS& a) const {
      return !(*this < a);
   }

   friend std::ostream& operator << ( std::ostream& s, const entityIdentifierDIS& v )
   {
      s << "Sim ID:" << std::endl
        << v.simulationID
        << "  entityID:                  " << (long)v.ID << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const entityIdentifierDIS* const v )
   {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   }
};


//-----------------------------------------------
// Entity Type Record

class EntityType {
public:
  void swapBytes(){
      country = convertShort((short)country);
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

  friend std::ostream& operator << ( std::ostream& s, const EntityType& v )
  {

    s << "  KD:DN:CN:CT:SC:SP:EX" 
      << " -  ";

    s.width(3);
    s.fill('0');
    s << (long)v.kind << ":";
    s.width(3);
    s.fill('0');
    s << (long)v.domain << ":";
    s.width(4);
    s.fill('0');
    s << (long)v.country << ":";
    s.width(3);
    s.fill('0');
    s << (long)v.category << ":";
    s.width(3);
    s.fill('0');
    s << (long)v.subcategory << ":";
    s.width(3);
    s.fill('0');
    s << (long)v.specific << ":";
    s.width(3);
    s.fill('0');
    s << (long)v.extra;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const EntityType* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  unsigned char  kind;
  unsigned char  domain;
  unsigned short country;
  unsigned char  category;
  unsigned char  subcategory;
  unsigned char  specific;
  unsigned char  extra;
};

//-----------------------------------------------
// Angular Velocity vectorDIS Record

class AngularVelocityvectorDIS{
public:
  void swapBytes(){
    x_axis = convertFloat((float)x_axis);
    y_axis = convertFloat((float)y_axis);
    z_axis = convertFloat((float)z_axis);
  };

  friend std::ostream& operator << ( std::ostream& s, const AngularVelocityvectorDIS& v )
  {
    s << "  x_axis:      " << (long)v.x_axis << std::endl
      << "  y_axis:      " << (long)v.y_axis << std::endl
      << "  z_axis:      " << (long)v.z_axis << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const AngularVelocityvectorDIS* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  float	x_axis;
  float	y_axis;
  float	z_axis;
};

//-----------------------------------------------
// Beam Antenna Pattern Record

class BeamAntennaPattern{
public:
  void swapBytes(){
    psi                = convertFloat((float)psi);
    theta              = convertFloat((float)theta);
    phi                = convertFloat((float)phi);
    azimuthBeamWidth   = convertFloat((float)azimuthBeamWidth);
    elevationBeamWidth = convertFloat((float)elevationBeamWidth);
    ez                 = convertFloat((float)ez);
    ex                 = convertFloat((float)ex);
    phase              = convertFloat((float)phase);
  };

  friend std::ostream& operator << ( std::ostream& s, const BeamAntennaPattern& v )
  {
    s << "  psi:                   " << (float)v.psi << std::endl
      << "  theta:                 " << (float)v.theta << std::endl
      << "  phi:                   " << (float)v.phi << std::endl
      << "  azimuthBeamWidth:      " << (float)v.azimuthBeamWidth << std::endl
      << "  elevationBeamWidth:    " << (float)v.elevationBeamWidth << std::endl
      << "  referenceSystem:       " << (long)v.referenceSystem << std::endl
      << "  ez:                    " << (float)v.ez << std::endl
      << "  ex:                    " << (float)v.ex << std::endl
      << "  phase:                 " << (float)v.phase << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const BeamAntennaPattern* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  // Beam Direction
  float psi;
  float theta;
  float phi;
  
  float azimuthBeamWidth;
  float elevationBeamWidth;
  char  referenceSystem;
  char  padding[3];
  float ez;
  float ex;
  float phase;
};

//-----------------------------------------------
// Spherical Harmonic Antenna Pattern Record

class SphericalHarmonicAntennaPattern {
public:
  // Not Finished
};

//-----------------------------------------------
// Articulation Parameter Record

class ArticulationParameter {

public:
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

public:
   void swapBytes() {
      id = convertUShort(id);
      parameterType = convertUInt(parameterType);
      if (parameterTypeDesignator == ATTACHED_PART) {
         parameterValue.entityType.swapBytes();
      }
      else {
         parameterValue.value[0] = convertFloat(parameterValue.value[0]);
         parameterValue.value[1] = convertFloat(parameterValue.value[1]);
      }
   };

   friend std::ostream& operator << ( std::ostream& s, const ArticulationParameter& v )
   {
      s << "  parameterTypeDesignator: " << (long)v.parameterTypeDesignator << std::endl
        << "  changeIndicator:         " << (long)v.changeIndicator << std::endl
        << "  id:                      " << (long)v.id << std::endl
        << "  parameterType:           " << (long)v.parameterType << std::endl;

      if (v.parameterTypeDesignator == ATTACHED_PART)
         s << v.parameterValue.entityType << std::endl;
      else 
         s << v.parameterValue.value[0] << std::endl;

      return s;
   };

  friend std::ostream& operator << ( std::ostream& s, const ArticulationParameter* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  unsigned char  parameterTypeDesignator;
  unsigned char  changeIndicator;
  unsigned short id;
  unsigned int   parameterType;
  union ParameterValue {
     float value[2];
     EntityType entityType;
  } parameterValue;
};

//-----------------------------------------------
// Burst Descriptor Record

class BurstDescriptor{
public:
  void swapBytes(){
    munision.swapBytes();
    warhead  = convertShort((short)warhead);
    fuse     = convertShort((short)fuse);
    quantity = convertShort((short)quantity);
    rate     = convertShort((short)rate);
  };

  friend std::ostream& operator << ( std::ostream& s, const BurstDescriptor& v )
  {
    s << "  Munision ID:"
      << v.munision << std::endl
      << "  warhead:      " << (long)v.warhead << std::endl
      << "  fuse:         " << (long)v.fuse << std::endl
      << "  quantity:     " << (long)v.quantity << std::endl
      << "  rate:         " << (long)v.rate << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const BurstDescriptor* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  EntityType munision;
  unsigned short warhead;
  unsigned short fuse;
  unsigned short quantity;
  unsigned short rate;
};

//-----------------------------------------------
// Clock Time Record

class ClockTime{
public:
  void swapBytes(){
    hour            = convertFloat((float)hour);
    timePastTheHour = convertFloat((float)timePastTheHour);
  };

  friend std::ostream& operator << ( std::ostream& s, const ClockTime& v )
  {
    s << "hour:            " << (float)v.hour << std::endl
      << "timePastTheHour: " << (float)v.timePastTheHour << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const ClockTime* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  float hour;
  float timePastTheHour;
};

//-----------------------------------------------
// Data Query Datum Specification Record

class DataQueryDatumSpecification{
public:
  // Not Finished
}; 

//-----------------------------------------------
// Datum Specification Record

class DatumSpecification{
public:
  // Not Finished
};



//-----------------------------------------------
// Euler Angles Record

class EulerAngles{
public:

  float psi;
  float theta;
  float phi;

  EulerAngles(){};

  EulerAngles(float a,float b,float c){
    psi   = a;
    theta = b;
    phi   = c;
  };

  ~EulerAngles(){};

  void swapBytes(){
    psi   = convertFloat((float)psi);
    theta = convertFloat((float)theta);
    phi   = convertFloat((float)phi);
  };

  friend std::ostream& operator << ( std::ostream& s, const EulerAngles& v )
  {
    s << "  psi:          " << (float)v.psi << std::endl
      << "  theta:        " << (float)v.theta << std::endl
      << "  phi:          " << (float)v.phi << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const EulerAngles* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

/*
  inline EulerAngles operator *(const DISmatrix& _v) {
      float x,y,z;
      x = _v.X0Y0 * psi + _v.X1Y0 * theta + _v.X2Y0 * phi;
      y = _v.X0Y1 * psi + _v.X1Y1 * theta + _v.X2Y1 * phi;
      z = _v.X0Y2 * psi + _v.X1Y2 * theta + _v.X2Y2 * phi;
      return EulerAngles( x,y,z );
  };
*/
  inline void operator =(const EulerAngles& _s) {
      psi   = _s.psi;
      theta = _s.theta;
      phi   = _s.phi;
  };
};

class SimEulerAngles{
public:

  float roll;
  float pitch;
  float hdg;

  SimEulerAngles(){};

  SimEulerAngles(float a,float b,float c){
    roll  = a;
    pitch = b;
    hdg   = c;
  };

  ~SimEulerAngles(){};

  void swapBytes(){
    roll  = convertFloat((float)roll);
    pitch = convertFloat((float)pitch);
    hdg   = convertFloat((float)hdg);
  };

  friend std::ostream& operator << ( std::ostream& s, const SimEulerAngles& v )
  {
    s << "  roll:         " << (float)v.roll << std::endl
      << "  pitch:        " << (float)v.pitch << std::endl
      << "  hdg:          " << (float)v.hdg << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const SimEulerAngles* v )
  {

    if ( !v )
       return s;
    else
    {
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
// Event Identifier Record

class EventIdentifier{
public:
  void swapBytes(){
    simulationID.swapBytes();
    eventNumber = convertUShort(eventNumber);
  };

  friend std::ostream& operator << ( std::ostream& s, const EventIdentifier& v )
  {
    s << "  Event Identifier:" << std::endl
      << v.simulationID 
      << "  eventNumber:     " << (long)v.eventNumber << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const EventIdentifier* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  simulationAddressDIS simulationID;
  unsigned short    eventNumber;
};

//-----------------------------------------------
// Fixed Datum Record

class FixedDatum{
public:
  void swapBytes(){
    fixedDatumID    = convertLong((long)fixedDatumID);
    fixedDatumValue = convertLong((long)fixedDatumValue);
  };

  unsigned int fixedDatumID;
  unsigned int fixedDatumValue;
};


//-----------------------------------------------
// PDU Header Record

class PDUHeader{
public:
  void swapBytes(){
    timeStamp = convertUInt(timeStamp);
    length    = convertUShort((short)length);
  };
 
  friend std::ostream& operator << ( std::ostream& s, const PDUHeader& v )
  {
    s << "  protocolVersion:     " << int(v.protocolVersion) << std::endl
      << "  exerciseIdentifier:  " << int(v.exerciseIdentifier) << std::endl
      << "  PDUType:             " << int(v.PDUType) << std::endl
      << "  protocolFamily:      " << int(v.protocolFamily) << std::endl
      << "  timeStamp:           " << v.timeStamp << std::endl
      << "  length:              " << v.length << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const PDUHeader* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };
 
  unsigned char  protocolVersion;
  unsigned char  exerciseIdentifier;
  unsigned char  PDUType;
  unsigned char  protocolFamily;
  unsigned int timeStamp;
  unsigned short length;
  unsigned short padding;
};

//-----------------------------------------------
// Simulation Management PDU Header Record

class SimulationManagementPDUHeader{
public:
  void swapBytes(){
    header.swapBytes();
    originatingEntityID.swapBytes();
    receivingEntityID.swapBytes();
  };

  friend std::ostream& operator << ( std::ostream& s, const SimulationManagementPDUHeader& v )
  {
    s << "Header:" << std::endl
      << v.header << std::endl
      << "Originating Entity ID:" << std::endl
      << v.originatingEntityID << std::endl
      << "Receiving Entity ID:" << std::endl
      << v.receivingEntityID << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const SimulationManagementPDUHeader* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  PDUHeader        header;
  entityIdentifierDIS originatingEntityID;
  entityIdentifierDIS receivingEntityID;
};

//-----------------------------------------------
// Supply Quantity Record

class SupplyQuantity{
public:
  void swapBytes(){
    supplyType.swapBytes();
    quantity = convertFloat((float)quantity);
  };

  friend std::ostream& operator << ( std::ostream& s, const SupplyQuantity& v )
  {
    s << "Supply Type:" << std::endl
      << v.supplyType << std::endl
      << "Quantity:" << std::endl
      << v.quantity << std::endl;

    return s;
  };

  friend std::ostream& operator << ( std::ostream& s, const SupplyQuantity* v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  EntityType supplyType;
  float      quantity;
};


class SystemID{
public:
  void swapBytes(){
    systemType = convertUShort((short)systemType);
    systemName = convertUShort((short)systemName);
  };

  friend std::ostream& operator << ( std::ostream &s, const SystemID &v )
  {
    s << "  System Type:    " << v.systemType << std::endl
      << "  System Name:    " << v.systemName << std::endl;
    s.width(2);
    s.fill('0'); 
    s << "  System Mode:    " << std::hex << (long)v.systemMode << std::endl;
    s.width(2);
    s.fill('0'); 
    s << "  Change/Options: " << (long)v.options << std::dec << std::endl; 

    return s;
  };

  friend std::ostream& operator << ( std::ostream &s, const SystemID *v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  unsigned short systemType;
  unsigned short systemName;
  unsigned char  systemMode;
  unsigned char  options;
};

class FundamentalOpData{
public:

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

  void swapBytes(){
    param1 = convertUShort(param1);
    param2 = convertUShort(param2);
    param3 = convertUShort(param3);
    param4 = convertUShort(param4);
    param5 = convertUShort(param5);
    param6 = convertUShort(param6);
  };

  friend std::ostream& operator << ( std::ostream &s, const FundamentalOpData &v )
  {
    s << "  System Status:      " << (long)v.systemStatus << std::endl
      << "  Alternate Param4:   " << (long)v.alternateParam4 << std::endl
      << "  Information Layers: " << (long)v.informationLayers << std::endl
      << "  Modifier:           " << (long)v.modifier << std::endl;

    s.width(2);
    s.fill('0'); 
    s << "    Parameter 1:    " << std::hex << (long)v.param1 << std::endl;
    s.width(2);
    s.fill('0'); 
    s << "    Parameter 2:    " << std::hex << (long)v.param2 << std::endl;
    s.width(2);
    s.fill('0'); 
    s << "    Parameter 3:    " << std::hex << (long)v.param3 << std::endl;
    s.width(2);
    s.fill('0'); 
    s << "    Parameter 4:    " << std::hex << (long)v.param4 << std::endl;
    s.width(2);
    s.fill('0'); 
    s << "    Parameter 5:    " << std::hex << (long)v.param5 << std::endl;
    s.width(2);
    s.fill('0'); 
    s << "    Parameter 6:    " << std::hex << (long)v.param6 << std::endl;
    s << std::dec;
    return s;
  };

  friend std::ostream& operator << ( std::ostream &s, const FundamentalOpData *v )
  {

    if ( !v )
       return s;
    else
    {
       s << *v;
    }
    return s;
  };

  unsigned char   systemStatus;
  unsigned char   alternateParam4;
  unsigned char   informationLayers;
  unsigned char   modifier;
  unsigned short  param1;
  unsigned short  param2;
  unsigned short  param3;
  unsigned short  param4;
  unsigned short  param5;
  unsigned short  param6;
};

//-----------------------------------------------
// Variable Datum Record
struct VariableDatum {

   unsigned int   variableDatumID;
   unsigned int   variableDatumLength;
   LCuint64*      variableDatumValue;     // DPG ### pointer in a PDU structure?

   // Swap bytes 'to' or 'from' the network.
   void swapBytes(){
      variableDatumID     = convertUInt(variableDatumID);
      variableDatumLength = convertUInt(variableDatumLength);
   };

   friend std::ostream& operator << ( std::ostream& s, const VariableDatum&  )
   {
      #if 0  /* DPG -- "long long" */
         s << "  variableDatumID:       " << (long)v.variableDatumID << std::endl
         << "  variableDatumLength:   " << (long)v.variableDatumLength << std::endl
         << "  variableDatumValue:    " << (long long)v.variableDatumValue << std::endl;
      #endif

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const VariableDatum* const v )
   {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};

//-----------------------------------------------
// Entity Marking Record
//-----------------------------------------------
struct EntityMarking {

   unsigned char characterSet;      // Marking character set

   static const unsigned int BUFF_SIZE = 11;
   char marking[BUFF_SIZE];         // Entity marking buffer

   friend std::ostream& operator << ( std::ostream& s, const EntityMarking& v )
   {
      s << "  Character Set:   " << (long)v.characterSet << std::endl
        << "  Markings:        " << v.marking << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const EntityMarking* v )
   {
      if ( v == 0 )
         return s;
      else
      {
         s << *v;
      }
      return s;
   };
};


//-----------------------------------------------
// Radio Entity Type Record
//-----------------------------------------------
struct RadioEntityType {

   unsigned char  kind;                // Kind
   unsigned char  domain;              // Domain
   unsigned short country;             // Country
   unsigned char  category;            // Category
   unsigned char  nomenclatureVersion; // Nomenclature Version
   unsigned short nomenclature;        // Nomenclature

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
   void swapBytes(){
      country      = convertUShort(country);
      nomenclature = convertUShort(nomenclature);
   };

   friend std::ostream& operator << ( std::ostream& s, const RadioEntityType& v )
   {
      s  << "  kind:                " << (long)v.kind << std::endl
         << "  domain:              " << (long)v.domain << std::endl
         << "  country:             " << (long)v.country << std::endl
         << "  category:            " << (long)v.category << std::endl
         << "  nomenclatureVersion: " << (long)v.nomenclatureVersion << std::endl
         << "  nomenclature:        " << (long)v.nomenclature << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const RadioEntityType* const v )
   {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};

//-----------------------------------------------
// Modulation Type Record
//-----------------------------------------------
struct ModulationType{

   unsigned short spreadSpectrum;         // Spread Spectrum
   unsigned short majorModulationType;    // Major Modulation type
   unsigned short detail;                 // Detail
   unsigned short system;                 // system 

   // Constructor(s)
   ModulationType() : spreadSpectrum(0), majorModulationType(0), detail(0), system(0) {}

   // Assignmnent operator
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
      spreadSpectrum      = convertUShort(spreadSpectrum);
      majorModulationType = convertUShort(majorModulationType);
      detail              = convertUShort(detail);
      system              = convertUShort(system);
   };

   friend std::ostream& operator << ( std::ostream& s, const ModulationType& v )
   {
      s  << "  spreadSpectrum:      " << v.spreadSpectrum << std::endl
         << "  majorModulationType: " << v.majorModulationType << std::endl
         << "  detail:              " << v.detail << std::endl
         << "  system:              " << v.system << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const ModulationType* const v )
   {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   };

};



//-----------------------------------------------
// vectorDIS Record
//-----------------------------------------------
struct vectorDIS {

   float component[3];        // X, Y Z components of the vectory

   // Constructor(s)
   vectorDIS(const float x, const float y, const float z) {
      component[0] = x;
      component[1] = y;
      component[2] = z;
   }
   vectorDIS() {
      component[0] = 0;
      component[1] = 0;
      component[2] = 0;
   }

   // Compare to another vectorDIS structure
   bool operator==(const vectorDIS& s2) const {
      return (
         component[0] == s2.component[0] && 
         component[1] == s2.component[1] && 
         component[2] == s2.component[2]
         );
   }
   bool operator!=(const vectorDIS& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network
   void swapBytes() {
      component[0] = convertFloat(component[0]);
      component[1] = convertFloat(component[1]);
      component[2] = convertFloat(component[2]);
   }

   friend std::ostream& operator << ( std::ostream& s, const vectorDIS& v )
   {
      s << "  component[0]:       " << (float)v.component[0] << std::endl
        << "  component[1]:       " << (float)v.component[1] << std::endl
        << "  component[2]:       " << (float)v.component[2] << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const vectorDIS* const v )
   {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   }

   vectorDIS operator +(const WorldCoordinates& _s) const;
   vectorDIS operator -(const WorldCoordinates& _s) const;
   vectorDIS operator *(const float _s) const;
   vectorDIS operator *(const double _s) const;

   friend vectorDIS operator +(const vectorDIS& _s,const vectorDIS& _v) {
      return vectorDIS( _s.component[0] + _v.component[0], _s.component[1] + _v.component[1], _s.component[2] + _v.component[2] );
   }

   friend vectorDIS operator -(const vectorDIS& _s,const vectorDIS& _v) {
      return vectorDIS( _s.component[0] - _v.component[0], _s.component[1] - _v.component[1], _s.component[2] - _v.component[2] );
   }

};

inline vectorDIS vectorDIS::operator *(const float _s) const
{
   return vectorDIS( component[0] * (float)_s, component[1] *(float) _s, component[2] * (float)_s);
}

inline vectorDIS vectorDIS::operator *(const double _s) const
{
   return vectorDIS( component[0] * (float)_s, component[1] * (float)_s, component[2] * (float)_s);
}


//-----------------------------------------------
// World Coordinates Record
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
      X_coord = convertDouble((double)X_coord);
      Y_coord = convertDouble((double)Y_coord);
      Z_coord = convertDouble((double)Z_coord);
   }

   friend std::ostream& operator << ( std::ostream& s, const WorldCoordinates& v )
   {
      s  << "  X_coord:       " << (double)v.X_coord << std::endl
         << "  Y_coord:       " << (double)v.Y_coord << std::endl
         << "  Z_coord:       " << (double)v.Z_coord << std::endl;

      return s;
   };

   friend std::ostream& operator << ( std::ostream& s, const WorldCoordinates* const v )
   {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   };

   friend WorldCoordinates operator +(const WorldCoordinates& _s, const vectorDIS& _v) {
      return WorldCoordinates( _s.X_coord + _v.component[0], _s.Y_coord + _v.component[1], _s.Z_coord + _v.component[2] );
   };

   friend WorldCoordinates operator -(const WorldCoordinates& _s, const vectorDIS& _v) {
      return WorldCoordinates( _s.X_coord - _v.component[0], _s.Y_coord - _v.component[1], _s.Z_coord - _v.component[2] );
   };

/*
  friend inline WorldCoordinates operator +(const vectorDIS& _v,const WorldCoordinates& _s) {
      return WorldCoordinates( _s.X_coord + _v.component[0], _s.Y_coord + _v.component[1], _s.Z_coord + _v.component[2] );
  };

  friend inline WorldCoordinates operator -(const vectorDIS& _v,const WorldCoordinates& _s) {
      return WorldCoordinates( _s.X_coord - _v.component[0], _s.Y_coord - _v.component[1], _s.Z_coord - _v.component[2] );
  };
*/
  
};

inline vectorDIS vectorDIS::operator +(const WorldCoordinates& _s) const
{
   return vectorDIS( (float)_s.X_coord + component[0], (float)_s.X_coord + component[1], (float)_s.X_coord + component[2] );
}

inline vectorDIS vectorDIS::operator -(const WorldCoordinates& _s) const
{
   return vectorDIS( (float)_s.X_coord - component[0], (float)_s.X_coord - component[1], (float)_s.X_coord - component[2] );
}


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

   friend std::ostream& operator << ( std::ostream& s, const SimCoordinates& v )
   {
      s  << "  lat:       " << (double)v.lat << std::endl
         << "  lon:       " << (double)v.lon << std::endl
         << "  alt:       " << (double)v.alt << std::endl;

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


//-----------------------------------------------
// Fundamental Parameter Data Record
//-----------------------------------------------
struct TrackJamTargets {

   entityIdentifierDIS targetID; // Track/Jam target ID
   unsigned char emitterID;      // Target's emitter ID being jammed
   unsigned char beamID;         // Target's beam ID being jammed


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
   void swapBytes(){
      targetID.swapBytes();
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const TrackJamTargets& v )
   {
      s  << "Target ID:       " << std::endl << v.targetID
         << "  emitterID:     " << (long)v.emitterID << std::endl
         << "  beamID:        " << (long)v.beamID << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const TrackJamTargets* const v )
   {
      if ( v == 0 )
         return s;
      else
      {
         s << *v;
      }
      return s;
   }

};

//-----------------------------------------------
// Fundamental Parameter Data Record
//-----------------------------------------------
struct FundamentalParameterData {

   float frequency;                 // Center frequency (Hz)
   float frequencyRange;            // Beam width (i.e., at 3db) or zero (Hz)
   float effectiveRadiatedPower;    // Effective radiated power ( watts dB)
   float pulseRepetitiveFrequency;  // PRF (hz)
   float pulseWidth;                // Pulse Width (uSec)
   float beamAzimuthCenter;         // Azimuth center of sweep volume (radians)
   float beamAzimuthSweep;          // Azimuth sweep width, half angle (radians)
   float beamElevationCenter;       // Elevation center of sweep volume (radians)
   float beamElevationSweep;        // Elevation sweep height, half angle (radians)
   float beamSweepSync;             // Percent of sweep completed 

   // Constructor(s)
   FundamentalParameterData() : 
      frequency(0), frequencyRange(0), effectiveRadiatedPower(0), pulseRepetitiveFrequency(0),
      pulseWidth(0), beamAzimuthCenter(0), beamAzimuthSweep(0), beamElevationCenter(0),
      beamElevationSweep(0), beamSweepSync(0) { }

   // Assignment operator
   void operator=(const FundamentalParameterData& _v) {
      frequency = _v.frequency;
      frequencyRange = _v.frequencyRange;
      effectiveRadiatedPower = _v.effectiveRadiatedPower;
      pulseRepetitiveFrequency = _v.pulseRepetitiveFrequency;
      pulseWidth = _v.pulseWidth;
      beamAzimuthCenter = _v.beamAzimuthCenter;
      beamAzimuthSweep = _v.beamAzimuthSweep;
      beamElevationCenter = _v.beamElevationCenter;
      beamElevationSweep = _v.beamElevationSweep;
      beamSweepSync = _v.beamSweepSync;
   }

   // Compare to another FundamentalParameterData structure
   bool operator==(const FundamentalParameterData& s2) const
   {
      return (
         frequency == s2.frequency && 
         frequencyRange == s2.frequencyRange && 
         effectiveRadiatedPower == s2.effectiveRadiatedPower && 
         pulseRepetitiveFrequency == s2.pulseRepetitiveFrequency && 
         pulseWidth == s2.pulseWidth && 
         beamAzimuthCenter == s2.beamAzimuthCenter && 
         beamAzimuthSweep == s2.beamAzimuthSweep && 
         beamElevationCenter == s2.beamElevationCenter && 
         beamElevationSweep == s2.beamElevationSweep && 
         beamSweepSync == s2.beamSweepSync
         );
   }
   bool operator!=(const FundamentalParameterData& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      frequency                = convertFloat(frequency);
      frequencyRange           = convertFloat(frequencyRange);
      effectiveRadiatedPower   = convertFloat(effectiveRadiatedPower);
      pulseRepetitiveFrequency = convertFloat(pulseRepetitiveFrequency);
      pulseWidth               = convertFloat(pulseWidth);
      beamAzimuthCenter        = convertFloat(beamAzimuthCenter);
      beamAzimuthSweep         = convertFloat(beamAzimuthSweep);
      beamElevationCenter      = convertFloat(beamElevationCenter);
      beamElevationSweep       = convertFloat(beamElevationSweep);
      beamSweepSync            = convertFloat(beamSweepSync);
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const FundamentalParameterData& v ) {
      s  << "  frequency:                " << v.frequency << std::endl
         << "  frequencyRange:           " << v.frequencyRange << std::endl
         << "  effectiveRadiatedPower:   " << v.effectiveRadiatedPower << std::endl
         << "  pulseRepetitiveFrequency: " << v.pulseRepetitiveFrequency << std::endl
         << "  pulseWidth:               " << v.pulseWidth << std::endl
         << "  beamAzimuthCenter:        " << v.beamAzimuthCenter << std::endl
         << "  beamAzimuthSweep:         " << v.beamAzimuthSweep << std::endl
         << "  beamElevationCenter:      " << v.beamElevationCenter << std::endl
         << "  beamElevationSweep:       " << v.beamElevationSweep << std::endl
         << "  beamSweepSync:            " << v.beamSweepSync << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const FundamentalParameterData* const v ) {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};


//-----------------------------------
// Emitter Beam Data
//-----------------------------------
struct EmitterBeamData {

   // Enum for highDensityTracks, (see IST-CF-03-01, May 5, 2003, Sec 8.1.5)
   enum { NOT_SELECTED, SELECTED };

   unsigned char            beamDataLength;        // Length of this beam data, in 32bit words, including the track/jam targets
   unsigned char            beamIDNumber;          // Unique ID for this beam
   unsigned short           beamParameterIndex;    // Use for the lookup of stored database parameters
   FundamentalParameterData parameterData;         // Parameter data
   unsigned char            beamFunction;          // Beam function enum (see IST-CF-03-01, May 5, 2003, Sec 8.1.4)
   unsigned char            numberOfTargetsInTrack; // Number of track/jam targets that will follow
   unsigned char            highDensityTracks;     // High Density Tracks flag 
   unsigned char            padding2;
   unsigned int             jammingModeSequence;   // Jamming techniques

   // Constructor(s)
   EmitterBeamData() :
      beamDataLength(0), beamIDNumber(0), beamParameterIndex(0), parameterData(),
      beamFunction(0), numberOfTargetsInTrack(0), highDensityTracks(0), padding2(0),
      jammingModeSequence(0) { }

   // Assignment operator
   void operator=(const EmitterBeamData& _v) {
      beamDataLength = _v.beamDataLength;
      beamIDNumber = _v.beamIDNumber;
      beamParameterIndex = _v.beamParameterIndex;
      parameterData = _v.parameterData;
      beamFunction = _v.beamFunction;
      numberOfTargetsInTrack = _v.numberOfTargetsInTrack;
      highDensityTracks = _v.highDensityTracks;
      jammingModeSequence = _v.jammingModeSequence;
   }

   // Returns a pointer to the idx'th TrackJamTarget structure;
   // Note: This ONLY works after the numberOfTargetsInTrack have been initialized!
   // Note: 'idx' is zero based, so a idx == 1 will return the second TrackJamTargets structure
   TrackJamTargets* getTrackTarget(const int idx) {
      TrackJamTargets* tjt = 0;
      if (idx < numberOfTargetsInTrack) {
         unsigned char* p = ((unsigned char*)this) + sizeof(*this) + (sizeof(TrackJamTargets) * idx);
         tjt = (TrackJamTargets*) p;	   
      }
      return tjt;
   }
   const TrackJamTargets* getTrackTarget(const int idx) const {
      const TrackJamTargets* tjt = 0;
      if (idx < numberOfTargetsInTrack) {
         unsigned char* p = ((unsigned char*)this) + sizeof(*this) + (sizeof(TrackJamTargets) * idx);
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
         beamFunction == s2.beamFunction && 
         numberOfTargetsInTrack == s2.numberOfTargetsInTrack && 
         highDensityTracks == s2.highDensityTracks && 
         jammingModeSequence == s2.jammingModeSequence
         );
   }
   bool operator!=(const EmitterBeamData& a) const {
      return !(*this == a);
   }

   // Swap bytes 'to' or 'from' the network.
   void swapBytes() {
      // Swap our stuff first
      beamParameterIndex  = convertUShort(beamParameterIndex);
      parameterData.swapBytes();
      jammingModeSequence = convertUInt(jammingModeSequence);

      // Then swap the "targets in track" data
      for (int i = 0; i < numberOfTargetsInTrack; i++) {
         TrackJamTargets* tjt = getTrackTarget(i);
         if (tjt != 0) tjt->swapBytes();
      }
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const EmitterBeamData& v ) {
      s  << "  beamDataLength:         " << (long)v.beamDataLength << std::endl
         << "  beamIDNumber:           " << (long)v.beamIDNumber << std::endl
         << "  beamParameterIndex:     " << (long)v.beamParameterIndex << std::endl
         << "Parameter Data:           " << std::endl << v.parameterData << std::endl
         << "  beamFunction:           " << (long)v.beamFunction << std::endl
         << "  numberOfTargetsInTrack: " << (long)v.numberOfTargetsInTrack << std::endl
         << "  highDensityTracks:      " << (long)v.highDensityTracks << std::endl
         << "  jammingModeSequence:    " << (long)v.jammingModeSequence << std::endl;

      for(int i=0; i < v.numberOfTargetsInTrack; i++)
      {
         const TrackJamTargets* tgt = v.getTrackTarget(i); 
         if (tgt != 0) {
            s  << "-----------------------------------------" << std::endl
               << "Target: " << i
               << tgt  
               << "-----------------------------------------" << std::endl;
         }
      }

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const EmitterBeamData* const v ) {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   }
};


//-----------------------------------------------
// Emitter System Record
//-----------------------------------------------
struct EmitterSystem {

   unsigned short emitterName;                  // Emitter Name enum (see IST-CF-03-01, May 5, 2003, Sec 8.1.1.1)
   unsigned char  function;                     // Function enum (see IST-CF-03-01, May 5, 2003, Sec 8.1.2)
   unsigned char  emitterIdentificationNumber;  // Unique emitter system number

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
      emitterName = convertUShort(emitterName);
   }

   // Friendly print functions
   friend std::ostream& operator << ( std::ostream& s, const EmitterSystem& v ) {
      s  << "emitterName:                 " << (long)v.emitterName << std::endl
         << "function:                    " << (long)v.function << std::endl
         << "emitterIdentificationNumber: " << (long)v.emitterIdentificationNumber << std::endl;

      return s;
   }

   friend std::ostream& operator << ( std::ostream& s, const EmitterSystem* const v ) {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   }

};


//-----------------------------------
// Emission System Data
//-----------------------------------
struct EmissionSystem {

   unsigned char     systemDataLength;    // Length of this system data, in 32bit words, including the Emitter Beam Data
   unsigned char     numberOfBeams;       // Number of beams (EmitterBeamData's that follow)
   unsigned short    padding1;
   EmitterSystem     emitterSystem;       // Descrption of the Emitter System
   vectorDIS         location;            // Location of emitter system; entities ref system (meters)

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
      for(int i = 0; i < numberOfBeams; i++)
      {
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
      EmitterBeamData* bd = 0;
      if (idx < numberOfBeams) {
         unsigned char *p = ((unsigned char*)this) + sizeof(*this);

         // First Emitter beam data is just after this structure
         bd = (EmitterBeamData*) p;

         for (int i = 0; i < idx; i++) {
            // step down to the correct beam data using 'beamDataLength'
            p += (bd->beamDataLength * 4);
            bd = (EmitterBeamData*) p;
         }

      }
      return bd;
   }
   const EmitterBeamData* getEmitterBeamData(const int idx) const {
      const EmitterBeamData* bd = 0;
      if (idx < numberOfBeams) {
         unsigned char *p = ((unsigned char*)this) + sizeof(*this);

         // First Emitter beam data is just after this structure
         bd = (const EmitterBeamData*) p;

         for (int i = 0; i < idx; i++) {
            // step down to the correct beam data using 'beamDataLength'
            p += (bd->beamDataLength * 4);
            bd = (const EmitterBeamData*) p;
         }

      }
      return bd;
   }

   // Friendly print function
   friend std::ostream& operator << ( std::ostream& s, const EmissionSystem& v ) {
      s  << "  systemDataLength:       " << (long)v.systemDataLength << std::endl
         << "  numberOfBeams:          " << (long)v.numberOfBeams << std::endl
         << "Emitter System:           " << std::endl << v.emitterSystem << std::endl
         << "Reference Location:       " << std::endl << v.location << std::endl;

      for(int i = 0; i < v.numberOfBeams; i++){
         const EmitterBeamData* bd = v.getEmitterBeamData(i); 
         s << "-----------------------------------------" << std::endl
            << "Emitter Beam: " << i
            << bd  
            << "-----------------------------------------" << std::endl;
      }

      return s;
   }

   // Another friendly print functions
   friend std::ostream& operator << ( std::ostream& s, const EmissionSystem* const v ) {
      if ( v == 0 )
         return s;
      else {
         s << *v;
      }
      return s;
   }
};

} // End Dis namespace
} // End Network namespace
} // End Eaagles namespace

#endif
