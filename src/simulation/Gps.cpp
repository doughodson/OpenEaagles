//------------------------------------------------------------------------------
// class: Gps
//------------------------------------------------------------------------------

#include "openeaagles/simulation/Gps.h"

namespace Eaagles {
namespace Simulation {


//==============================================================================
// class: Gps
//==============================================================================

IMPLEMENT_SUBCLASS(Gps,"Gps")
EMPTY_SLOTTABLE(Gps)
EMPTY_SERIALIZER(Gps)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Gps::Gps()
{
    STANDARD_CONSTRUCTOR()

    mode    = INIT;
    bsr     = PRIMARY;
    rcvr    = RCVR_INVALID;
    aided   = true;
    keyOvrd = false;

    crypto  = NO_LOAD;
    almanac = NO_LOAD;

    quality = A;
    state   = 0;
    ehe     = 0;
    eve     = 0;
    fom     = 0;
    cnr     = 0;

    date    = 123199;
    keyType = GUV;
    days    = 5;
}

//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Gps::copyData(const Gps& org, const bool)
{
    BaseClass::copyData(org);

    mode    = org.mode;
    bsr     = org.bsr;
    rcvr    = org.rcvr;
    aided   = org.aided;
    keyOvrd = org.keyOvrd;

    crypto  = org.crypto;
    almanac = org.almanac;

    quality = org.quality;
    state   = org.state;
    ehe     = org.ehe;
    eve     = org.eve;
    fom     = org.fom;
    cnr     = org.cnr;

    date    = org.date;
    keyType = org.keyType;
    days    = org.days;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Gps::deleteData()
{
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
void Gps::setOperatingMode(const OperMode m)
{
   mode = m;
}

void Gps::setGpsAidedOn(const bool af)
{
   aided = af;
}

void Gps::setKeyOvrdEnabled(const bool kof)
{
   keyOvrd = kof;
}

//------------------------------------------------------------------------------
// getCryptoKeys(), setCryptoKeys(), clearCryptoKeys() --
//------------------------------------------------------------------------------
int Gps::getCryptoKeys(double ck[], const int week, const int max) const
{
    unsigned int n = (unsigned int)max;
    if (n > MAX_CRYPTO_KEYS) n = MAX_CRYPTO_KEYS;
    for (unsigned int i = 0; i < n; i++) {
        ck[i] = cryptoKeys[week][i];
    }
    return (int)n;
}

void Gps::setCryptoKeys(double ck[], const int week, const int num)
{
    unsigned int n = (unsigned int)num;
    if (n > MAX_CRYPTO_KEYS) n = MAX_CRYPTO_KEYS;
    for (unsigned int i = 0; i < n; i++) {
        cryptoKeys[week][i] = ck[i];
    }
}

} // End Simulation namespace
} // End Eaagles namespace
