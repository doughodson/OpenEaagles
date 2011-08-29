#ifdef __cplusplus
//------------------------------------------------------------------------------
// Class:	DsNavaidClient
// Base class:	DsDafifDatabase -> DsNavaidLoader -> DsNavaidClient
//
// Description: DAFIF NAVAID network client class 
//
//
// Public member functions:
//
//	[all public members from the base classes]
//
//	DsNavaidClient()
//	DsNavaidClient(const char* server)
//		Constructors: where 'server' is the name of the DAFIF server
//		computer.  If 'server' is not provided, not connection is
//		make.
//
//------------------------------------------------------------------------------
#ifndef __DsNavaidClient_H__
#define __DsNavaidClient_H__

#include "NavaidLoader.h"
#include "DafifClient.h"


class DsNavaidClient : public DsNavaidLoader, public DsDafifClient {
public:

   DsNavaidClient(const char* server = 0);
   virtual ~DsNavaidClient();

   virtual int numberOfRecords();
   virtual int numberFound();
   
   virtual const char* record(const int n, const int size = DS_DEFAULT_SIZE);
   virtual const char* getRecord(const int n, const int size = DS_DEFAULT_SIZE);

   virtual int queryByRange();
   virtual int queryByIdent(const char* id);
   virtual int queryByKey(const char* key);
   virtual int queryByIcao(const char* code);
   virtual int queryByType(const DsNavaid::NavaidType type);
   virtual int queryByFreq(const float freq);
   virtual int queryByChannel(const long chan, const char band = 'X');
   
   virtual void print();
   virtual void printResults();

   virtual void printFreqList();
   virtual void printChannelList();

};


//------------------------------------------------------------------------------
// Constructor(s)
//------------------------------------------------------------------------------
inline
DsNavaidClient::DsNavaidClient(const char* server)
				: DsNavaidLoader(0,0),
				  DsDafifClient(recordSize(), server)
{
}

#endif
#endif
