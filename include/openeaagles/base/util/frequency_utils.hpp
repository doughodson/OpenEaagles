
#ifndef __oe_base_util_frequency_utils_H__
#define __oe_base_util_frequency_utils_H__

namespace oe {
namespace base {
namespace frequency {

// conversion constants
const double KHz2Hz = 1000.0f;                   // KiloHertz => Hertz
const double Hz2KHz = (1.0f / 1000.0f);          // Hertz => KiloHertz

const double MHz2Hz = 1000000.0f;                // MegaHertz => Hertz
const double Hz2MHz = (1.0f / 1000000.0f);       // Hertz => MegaHertz

const double GHz2Hz = 1000000000.0f;             // GigaHertz => Hertz
const double Hz2GHz = (1.0f / 1000000000.0f);    // Hertz => GigaHertz

const double THz2Hz = 1000000000000.0f;          // TeraHertz => Hertz
const double Hz2THz = (1.0f / 1000000000000.0f); // Hertz => TeraHertz

}
}
}

#endif
