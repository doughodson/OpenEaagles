#ifdef __cplusplus
//------------------------------------------------------------------------------
// Class:	DsWaypointClient
// Base class:	DsDafifDatabase -> DsWaypointLoader -> DsWaypointClient
//
// Description: DAFIF waypoint network client class 
//
//
// Public member functions:
//
//	[all public members from the base classes]
//
//	DsWaypointClient()
//	DsWaypointClient(const char* server)
//		Constructors: where 'server' is the name of the DAFIF server
//		computer.  If 'server' is not provided, not connection is
//		make.
//
//------------------------------------------------------------------------------
#ifndef __DsWaypointClient_H__
#define __DsWaypointClient_H__

#include "WaypointLoader.h"
#include "DafifClient.h"


class DsWaypointClient : public DsWaypointLoader, public DsDafifClient {
public:

   DsWaypointClient(const char* server = 0);
   virtual ~DsWaypointClient();

   virtual int numberOfRecords();
   virtual int numberFound();
   
   virtual const char* record(const int n, const int size = DS_DEFAULT_SIZE);
   virtual const char* getRecord(const int n, const int size = DS_DEFAULT_SIZE);

   virtual int queryByRange();
   virtual int queryByIdent(const char* id);
   virtual int queryByIcao(const char* code);
   virtual int queryByKey(const char* key);
   
   virtual void print();
   virtual void printResults();

};


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
inline
DsWaypointClient::DsWaypointClient(const char* server)
				: DsWaypointLoader(0,0),
				 DsDafifClient(recordSize(), server)
{
}

#endif
#endif
