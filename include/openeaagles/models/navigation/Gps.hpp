
#ifndef __oe_models_Gps_H__
#define __oe_models_Gps_H__

#include "openeaagles/models/navigation/Navigation.hpp"

namespace oe {
namespace models {

//------------------------------------------------------------------------------
// Class: Gps
// Description: Generic Global Positioning System (GPS) Data
// Factory name: Gps
//------------------------------------------------------------------------------
class Gps : public Navigation
{
    DECLARE_SUBCLASS(Gps, Navigation)

public:

    // Operating modes
    enum OperMode { INIT = 1, NAV = 2, TEST = 3, OFF = 4 };

    // Receiver Status
    enum RcvrStat { RCVR_INVALID = 1, RCVR_VALID = 2 };

    // Bus Switch Relay Status
    enum BsrStat { PRIMARY = 1, SECONDARY = 2 };

    // Load Status
    enum LoadStat { NO_LOAD = 1, VALID_LOAD = 2, INVALID_LOAD = 3 };

    // Overall Quality Codes
    enum QualCode { A = 1, B = 2, C = 3, D = 4 };

    // Crypto Variable Types
    enum KeyType { CVW = 1, GUV = 2 };

public:
    Gps();

    OperMode getOperatingMode() const       { return mode; }
    BsrStat getBsrStatus() const            { return bsr; }
    RcvrStat getRcvrStatus() const          { return rcvr; }
    bool isGpsAidedOn() const               { return aided; }
    bool isKeyOvrdEnabled() const           { return keyOvrd; }
    LoadStat getCryptoLoadStatus() const    { return crypto; }
    LoadStat getAlmanacLoadStatus() const   { return almanac; }
    QualCode getQuality() const             { return quality; }
    long getState() const                   { return state; }
    long getEstHorzError() const            { return ehe; }
    long getEstVertError() const            { return eve; }
    long getFigureOfMerit() const           { return fom; }
    long getCarrierToNoise() const          { return cnr; }
    long getEffectiveDate() const           { return date; }
    KeyType getKeyType() const              { return keyType; }
    long getDaysRemaining() const           { return days; }

    virtual int getCryptoKeys(double ck[], const int week, const int max) const;
    virtual void setCryptoKeys(double ck[], const int week, const int num);

   // Control Functions
   virtual void setOperatingMode(const OperMode mode);
   virtual void setGpsAidedOn(const bool aided);
   virtual void setKeyOvrdEnabled(const bool keyOvrd);

private:
    static const unsigned int MAX_CRYPTO_KEYS = 8; // max GPS Crypto variables

   // GPS Status Data
   RcvrStat rcvr;       // Receiver status
   OperMode mode;       // Operation mode
   bool      aided;     // GPS Aiding YES/NO
   BsrStat  bsr;        // Bus Switch Relay Status
   bool      keyOvrd;   // Key Override DSBL/ENBL

   // Load Status Data
   LoadStat crypto;     // Status of crypto key data files
   LoadStat almanac;    // status of GPS almanac

   // GPS Quality Data
   QualCode quality;    // Data quality code
   long     state;      // GPS state
   long     ehe;        // Estimated horizontal error
   long     eve;        // Estimated vertical error
   long     fom;        // Figure of merit
   long     cnr;        // Carrier to noise ratio, in dB

   // Crypto Variable Data
   long     date;       // Effective date of keys
   KeyType  keyType;    // Type of keys loaded
   long     days;       // Number of days the keys remain valid
   double   cryptoKeys[2][MAX_CRYPTO_KEYS]; // Crypto Variables
};

}
}

#endif
