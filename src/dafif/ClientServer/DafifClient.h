#ifndef __DsDafifClient_H__
#define __DsDafifClient_H__

#ifdef __cplusplus
//------------------------------------------------------------------------------
// Class:	DsDafifClient
// Base class:	<This is a base class>
//
// Description: Abstract class used by derived DAFIF client classes.
//
//
// Protected member functions:
//
//
//	DsDafifClient(const DsType type, const int rsize)
//	DsDafifClient(const DsType type, const int rsize, const char* server)
//		Constructor: where 'type' is the dafifd data type defined
//		in dafifc.h and 'rsize' is the standard record size in bytes.
//		If the server name, server, is not provided, the default
//		DAFIF server is used.
//
//	int dcNumberOfRecords(DsType type) const
//		Returns the number of records loaded from the server file.
//
//	int dcNumberFound() const
//		Returns the number of records found by the last query.
//
//	dcSetArea(float lat, float lon)
//	dcSetArea(float lat, float lon, float mrng)
//		Sets the search area (ref point) for queries to lat and lon.
//		If mrng is provided, then query results are limited to mrng.
//
//	dcSetQueryLimit()
//	dcSetQueryLimit(int mrec)
//		Sets a limit on the number of records that can be returned
//		to 'mrec' or to default.
//
//	const char* dcRecord(DsType type, int n)
//	const char* dcRecord(DsType type, int n, int size)
//		Returns a pointer to n'th record loaded.  This points to an
//		internal buffer and is only valid until the next file access.
//		Range: 0 .. numberOfRecords()-1
//
//	const char* dcGetRecord(int n)
//		Get a pointer to the n'th record found by last query.  This
//		points to an internal buffer that is only valid until the
//		next file access.   Range: 0 .. numberFound()-1
//
//	float dcMagvar(double latitude, double longitude,
//		       float altitude, time_t time)
//		Returns the magnetic variance for latitude, longitude, altitude,
//		and time.
//
//------------------------------------------------------------------------------

#include "Dafif.h"
#include <unistd.h>
#include <iostream.h>

#define DEFAULT_QUERY_LIMIT	40

class DsDafifClient {
protected:
   DsDafifClient(const int rsize, const char* server = 0);
   virtual ~DsDafifClient();

   int dcNumberOfRecords(const DsType type);
   int dcNumberFound() const;
   
   void dcSetArea(const float lat, const float lon, const float mrng = 0.0f);
   void dcSetQueryLimit(const int mrec);
   void dcSetRecordNumber(const int n);

   const char* dcRecord(const DsType dtype, const int n,
			const int size = DS_DEFAULT_SIZE);
   const char* dcGetRecord(const int n);

   int getWithIntArg(const DsType dtype, const DsRequest request,
		     const int n, const int size = DS_DEFAULT_SIZE);

   int getWithFloatArg(const DsType dtype, const DsRequest request,
		       const float x, const int size = DS_DEFAULT_SIZE);

   int getWithStringArg(const DsType dtype, const DsRequest request,
			const char* id, const int size = DS_DEFAULT_SIZE);

   void setupReqMsg(const DsType dtype, const DsRequest request,
		    const int id1 = 1);


   float dcMagvar(const double lat, const double lon, const float alt,
		  const time_t tim);
   int query();

   int isRespMsgOk();

   DsRequestMsg	reqMsg;		// Request message
   DsResponseMsg* respMsg;	// Pointer to the response message

private:
   static void msgHandler();

   int  seq;			// Sequence number

   int	rsize;			// Size of a standard record
   int  sock_id;		// Socket ID (clients)

   int	 dcQlimit;		// Max number of records found by query
   int	 dcQlimit1;		// Max value of qlimit
   int   respMsgBuffSize;	// Size of the response messabe buffer
   char* respMsgBuffer;		// Response message buffer;
   
   long  nrl;			// Number of Records in database
   int   nql;			// Number of records found by last query

   float dcRefLat;		// Ref point latitude
   float dcRefLon;		// Ref point longitude
   float dcMrng;		// Max search range (nm)
   int   dcNum;			// Record number

   int   dcRespMsgOk;		// true of response message was received

};


//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
inline
DsDafifClient::DsDafifClient(const int rs, const char* server)
{
   // Parameters
   rsize = rs;

   seq = 0;
   
   // Open  the socket
   sock_id = dsInitClient(server,DsDafifClient::msgHandler);
   
   // default query limit and buffer size
   dcQlimit  = DEFAULT_QUERY_LIMIT;
   dcQlimit1 = dcQlimit;
   respMsgBuffSize = (dcQlimit*rsize) + sizeof(DsResponseMsg);
   respMsgBuffer = new char[respMsgBuffSize];
   respMsg = (DsResponseMsg*) respMsgBuffer;
   
   nql = 0;
   nrl = -1;

   dcRefLat = 0.0f;
   dcRefLon = 0.0f;
   dcMrng   = 0.0f;
   dcNum    = 0;

   dcRespMsgOk = 0;
}


//------------------------------------------------------------------------------
// inline support functions
//------------------------------------------------------------------------------

// dcSetArea: set the search area limits
inline void
DsDafifClient::dcSetArea(const float lat, const float lon, const float mr)
{
   dcRefLat = lat;
   dcRefLon = lon;
   dcMrng   = mr;
}


// dcSetQueryLimit: set the maximum number of records returned by a query
inline void DsDafifClient::dcSetQueryLimit(const int mrec)
{
   dcQlimit = DEFAULT_QUERY_LIMIT;
   if (mrec != 0) dcQlimit = mrec;
   
   if (dcQlimit > dcQlimit1) {
      dcQlimit1 = dcQlimit;
      delete[] respMsgBuffer;
      respMsgBuffSize = (dcQlimit*rsize) + sizeof(DsResponseMsg);
      respMsgBuffer = new char[respMsgBuffSize];
      respMsg = (DsResponseMsg*) respMsgBuffer;
   }
}

// dcSetRecordNumber: set the record number 
inline void
DsDafifClient::dcSetRecordNumber(const int n)
{
   dcNum = n;
}


// setupReqMsg: init the request message for next request
inline void 
DsDafifClient::setupReqMsg(const DsType dtype, const DsRequest request,
			   const int id1)
{
   reqMsg.seq = seq;
   reqMsg.id = id1;
   
   reqMsg.type = dtype;
   reqMsg.request = request;
   
   reqMsg.reflat = dcRefLat;
   reqMsg.reflon = dcRefLon;
   reqMsg.mrng = dcMrng;
   reqMsg.num =   dcNum;
   
   reqMsg.mrec = dcQlimit;
   reqMsg.rsize = DS_DEFAULT_SIZE;
}


// isRespMsgOk: returns the status of the last response message
inline int DsDafifClient::isRespMsgOk()
{
   return dcRespMsgOk;
}


// dcNumberOfRecords: returns the number of records in this database
inline int DsDafifClient::dcNumberOfRecords(const DsType dtype) 
{
   // Get it if we don't already have it
   if (nrl <= 0) {
      nrl = 0;
      setupReqMsg(dtype,dsGetNumRecords);
      query();
      nrl = respMsg->value.nrl;
   }
   return nrl;
}



// getWithIntArg: handle queries with integer arguments.
inline int 
DsDafifClient::getWithIntArg(const DsType dtype, const DsRequest request,
			     const int n, const int size)
{
   nql = 0;
   if (dcNumberOfRecords(dtype) > 0) {
      setupReqMsg(dtype,request);
      reqMsg.arg.len = n;
      if (size != DS_DEFAULT_SIZE) reqMsg.rsize = size;
      query();
   }
   return nql;
}


// getWithFloatArg: handle queries with float arguments.
inline int
DsDafifClient::getWithFloatArg(const DsType dtype, const DsRequest request,
			       const float x, const int size)
{
   nql = 0;
   if (dcNumberOfRecords(dtype) > 0) {
      setupReqMsg(dtype,request);
      reqMsg.arg.freq = x;
      if (size != DS_DEFAULT_SIZE) reqMsg.rsize = size;
      query();
   }
   return nql;
}


// getWithStringArg: handle queries with string arguments.
inline int
DsDafifClient::getWithStringArg(const DsType dtype, const DsRequest request,
				const char* id, const int size)
{
   nql = 0;
   if (dcNumberOfRecords(dtype) > 0) {
      setupReqMsg(dtype,request);
      strncpy(reqMsg.arg.ident,id,15);
      reqMsg.arg.ident[15] = '\0';
      if (size != DS_DEFAULT_SIZE) reqMsg.rsize = size;
      query();
   }
   return nql;
}


// dcNumberFound: returns the number of records found by last query
inline int DsDafifClient::dcNumberFound() const
{
   return nql;
}


// dcGetRecord: returns the n'th record found by the last query
inline const char* DsDafifClient::dcGetRecord(const int n)
{
   dcNum = n;
   const char* s = 0;
   if (n >= 0 && n < nql) {
      int i = respMsg->rsize*n;
      s = &respMsg->value.buff[i];
   }

   return s;
}


// dcRecord: returns the n'th record in the database
inline const char* DsDafifClient::dcRecord(const DsType dtype,
					   const int n, const int size)
{
   const char* p = 0;
   if (n >= 0 && n < dcNumberOfRecords(dtype)) {
      dcNum = n;
      nql = 0;
      if (dcNumberOfRecords(dtype) > 0) {
         setupReqMsg(dtype,dsGetByNumber);
         if (size != DS_DEFAULT_SIZE) reqMsg.rsize = size;
         query();
      }
      p = dcGetRecord(0);   
   }
   return p;
}


// dcMagvar: returns the number of records in this database
inline float DsDafifClient::dcMagvar(const double lat, const double lon,
				     const float, const time_t tim)
{
   float magvar = 0.0f;
   dcSetArea(lat,lon);
   setupReqMsg(dsMagVar,DsRequest(0));
   reqMsg.arg.time = tim;
   if (query()) magvar = respMsg->value.magvar;
   return magvar;
}

#endif
#endif
