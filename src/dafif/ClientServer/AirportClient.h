#ifdef __cplusplus
//------------------------------------------------------------------------------
// Class:	DsAirportClient
// Base class:	DsDafifDatabase -> DsAirportLoader -> DsAirportClient
//
// Description: DAFIF airport network client class 
//
//
// Public member functions:
//
//	[all public members from the base classes]
//
//	DsAirportClient()
//	DsAirportClient(const char* server)
//		Constructors: where 'server' is the name of the DAFIF server
//		computer.  If 'server' is not provided, not connection is
//		make.
//
//------------------------------------------------------------------------------
#ifndef __DsAirportClient_H__
#define __DsAirportClient_H__

#include "AirportLoader.h"
#include "DafifClient.h"


class DsAirportClient : public DsAirportLoader, public DsDafifClient {
public:

   DsAirportClient(const char* server = 0);
   virtual ~DsAirportClient();

   virtual int numberOfRecords();
   virtual int numberFound();
   
   virtual const char* record(const int n, const int size = DS_DEFAULT_SIZE);
   virtual const char* getRecord(const int n, const int size = DS_DEFAULT_SIZE);

   virtual int queryByRange();
   virtual int queryByIdent(const char* id);
   virtual int queryByKey(const char* key);
   virtual int queryByIcao(const char* code);
   virtual int queryByLength(const float minRwLen);
   virtual int queryByType(const DsAirport::AirportType type);
   virtual int queryByFreq(const float freq);
   virtual int queryByChannel(const int chan);

   virtual int getNumRunwayRecords(const char* key);
   virtual int queryRunwayByNumber(const char* key, const int n);
   virtual int queryRunwayByIdent(const char* id);
   virtual int queryRunwayBySubkey(const char* subkey);
   virtual int queryRunwayByRange();
   virtual int queryRunwayByLength(const float length);
   virtual int queryRunwayByFreq(const float freq);
   virtual int queryRunwayByChannel(const int chan);

   virtual int getNumIlsRecords(const char* key);
   virtual int queryIlsByNumber(const char* key, const int n);
   virtual int queryIlsByIdent(const char* id);
   virtual int queryIlsBySubkey(const char* subkey);
   virtual int queryIlsByRange();
   virtual int queryIlsByType(const DsIls::IlsType type);
   virtual int queryIlsByFreq(const float freq);
   virtual int queryIlsByChannel(const int chan);
   
   virtual void print();
   virtual void printResults();

};


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
inline
DsAirportClient::DsAirportClient(const char* server)
				: DsAirportLoader(0,0),
				 DsDafifClient(recordSize(), server)
{
}

#endif
#endif
