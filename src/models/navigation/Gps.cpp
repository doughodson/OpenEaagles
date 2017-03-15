
#include "openeaagles/models/navigation/Gps.hpp"
#include <iostream>

namespace oe {
namespace models {

IMPLEMENT_SUBCLASS(Gps, "Gps")
EMPTY_SLOTTABLE(Gps)
EMPTY_SERIALIZER(Gps)
EMPTY_DELETEDATA(Gps)

Gps::Gps()
{
    STANDARD_CONSTRUCTOR()
}

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
    unsigned int n = static_cast<unsigned int>(max);
    if (n > MAX_CRYPTO_KEYS) n = MAX_CRYPTO_KEYS;
    for (unsigned int i = 0; i < n; i++) {
        ck[i] = cryptoKeys[week][i];
    }
    return static_cast<int>(n);
}

void Gps::setCryptoKeys(double ck[], const int week, const int num)
{
    unsigned int n = static_cast<unsigned int>(num);
    if (n > MAX_CRYPTO_KEYS) n = MAX_CRYPTO_KEYS;
    for (unsigned int i = 0; i < n; i++) {
        cryptoKeys[week][i] = ck[i];
    }
}

}
}
