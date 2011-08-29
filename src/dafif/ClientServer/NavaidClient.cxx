#include <string.h>
#include "NavaidClient.h"


//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
DsNavaidClient::~DsNavaidClient()
{
}


//------------------------------------------------------------------------------
// numberOfRecords() -- returns the number of records in this database
// numberFound() -- returns the number of records found by last query
//------------------------------------------------------------------------------
int DsNavaidClient::numberOfRecords()
{
   int n = dcNumberOfRecords(dsNavaid);
   return n;
}

int DsNavaidClient::numberFound()
{
   return dcNumberFound();
}


//------------------------------------------------------------------------------
// Database access functions -- The strings are only valid until the next
// access.  The user should make a copy of the string if the record is to
// be saved.
//------------------------------------------------------------------------------
const char* DsNavaidClient::record(const int n, const int size)
{
   return dcRecord(dsNavaid,n,size);
}

const char* DsNavaidClient::getRecord(const int n, const int)
{
   return dcGetRecord(n);
}


//------------------------------------------------------------------------------
// queryByRange() -- find NAVAID record(s) less than maxRange from the
// ref point (sorted by range)
//------------------------------------------------------------------------------
int DsNavaidClient::queryByRange()
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsNavaid,dsGetByRange,0);
}


//------------------------------------------------------------------------------
// queryByIdent() -- find NAVAID record(s) by identifier
//------------------------------------------------------------------------------
int DsNavaidClient::queryByIdent(const char* id)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsNavaid,dsGetByIdent,id);
}

//------------------------------------------------------------------------------
// queryByFreq() -- find NAVAID record(s) by frequency/Channel
//------------------------------------------------------------------------------
int DsNavaidClient::queryByFreq(const float freq)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithFloatArg(dsNavaid,dsGetByFreq,freq);
}


//------------------------------------------------------------------------------
// queryByChannel() -- find NAVAID record(s) by channel number
//------------------------------------------------------------------------------
int DsNavaidClient::queryByChannel(const long chan, const char band)
{
   long chan1 = chan;
   if (band == 'Y') chan1 = -chan1;
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsNavaid,dsGetByChannel,chan1);
}


//------------------------------------------------------------------------------
// queryByType() -- find 't' type NAVAID record(s) less than maxRange from the
// ref point (sorted by range)
//------------------------------------------------------------------------------
int DsNavaidClient::queryByType(const DsNavaid::NavaidType t)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsNavaid,dsGetByType,t);
}


//------------------------------------------------------------------------------
// queryByKey() -- find a NAVAID record by the NAVAID record key
//------------------------------------------------------------------------------
int DsNavaidClient::queryByKey(const char* key)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsNavaid,dsGetByKey,key);
}


//------------------------------------------------------------------------------
// queryByIcao() -- find a NAVAID record by the ICAO code
//------------------------------------------------------------------------------
int DsNavaidClient::queryByIcao(const char* code)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsNavaid,dsGetByIcao,code);
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void DsNavaidClient::serialize() const
{
   DsNavaid navaid;
   int n = dcNumberOfRecords(dsNavaid);
   for (int i = 0; i < n; i++) {
      const char* s = dcRecord(dsNavaid,i);
      if (s != 0) {
	 navaid.setRecord( s );
	 navaid.print();
      }
   }
}

void DsNavaidClient::printFreqList()
{
}

void DsNavaidClient::printChannelList()
{
}

void DsNavaidClient::printResults()
{
   DsNavaid navaid;
   int n = dcNumberFound();
   for (int i = 0; i < n; i++) {
      const char* s = dcGetRecord(i);
      if (s != 0) {
	 navaid.setRecord( s );
	 navaid.print();
      }
   }
}
