
#include <iostream.h>
#include "MagvarClient.h"


//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
DsMagvarClient::~DsMagvarClient()
{
}


//------------------------------------------------------------------------------
// magvar() --
//------------------------------------------------------------------------------
float DsMagvarClient::magvar(const double lat, const double lon,
			     const float alt, const time_t tim)
{
   return dcMagvar(lat,lon,alt,tim);
}
