#ifndef __DsMagvarClient_H__
#define __DsMagvarClient_H__

#ifdef __cplusplus
//------------------------------------------------------------------------------
// Class:	DsMagvarClient
// Base class:	<This is a base class>
//
// Description: Returns the mag variance for a given location.
//
//
// Public member functions:
//
//	DsMagvarClient()
//	DsMagvarClient(const char* server)
//		Constructors; 'server' is the name of the DAFIF server
//
//
//	float magvar(double latitude, double longitude,
//		     float altitude, time_t time)
//		Returns the magnetic variance for latitude, longitude, altitude,
//		and time.
//
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "Magvar.h"
#include "DafifClient.h"


class DsMagvarClient : public DsMagvar, public DsDafifClient {
public:
   DsMagvarClient(const char* server = 0);
   virtual ~DsMagvarClient();

   virtual float magvar(const double lat, const double lon, const float alt,
			const time_t tim);

};


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
inline DsMagvarClient::DsMagvarClient(const char* server)
			   	: DsMagvar(0), DsDafifClient(0, server)
{
}

#endif
#endif
