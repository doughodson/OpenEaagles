#include <string.h>
#include "AirportClient.h"


//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
DsAirportClient::~DsAirportClient()
{
}


//------------------------------------------------------------------------------
// numberOfRecords() -- returns the number of records in this database
// numberFound() -- returns the number of records found by last query
//------------------------------------------------------------------------------
int DsAirportClient::numberOfRecords()
{
   return dcNumberOfRecords(dsAirport);
}

int DsAirportClient::numberFound()
{
   return dcNumberFound();
}


//------------------------------------------------------------------------------
// Database access functions -- The strings are only valid until the next
// access.  The user should make a copy of the string if the record is to
// be saved.
//------------------------------------------------------------------------------
const char* DsAirportClient::record(const int n, const int size)
{
   return dcRecord(dsAirport,n,size);
}

const char* DsAirportClient::getRecord(const int n, const int)
{
   return dcGetRecord(n);
}


//------------------------------------------------------------------------------
// queryByRange() -- find all airports within search area.
//------------------------------------------------------------------------------
int DsAirportClient::queryByRange()
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsAirport,dsGetByRange,0);
}


//------------------------------------------------------------------------------
// queryByIdent() -- find airport record(s) by identifier
//------------------------------------------------------------------------------
int DsAirportClient::queryByIdent(const char* id)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsAirport,dsGetByIdent,id);
}


//------------------------------------------------------------------------------
// queryByKey() -- find a airport record by the airport record key
//------------------------------------------------------------------------------
int DsAirportClient::queryByKey(const char* key)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsAirport,dsGetByKey,key);
}


//------------------------------------------------------------------------------
// queryByIcao() -- find a airport record by the ICAO code
//------------------------------------------------------------------------------
int DsAirportClient::queryByIcao(const char* code)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsAirport,dsGetByIcao,code);
}


//------------------------------------------------------------------------------
// queryByLength() -- find all airports within search area with minimum
// runway length.
//------------------------------------------------------------------------------
int DsAirportClient::queryByLength(const float minRwLen)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsAirport,dsGetByLength,int(minRwLen));
}


//------------------------------------------------------------------------------
// queryByType() -- find all 'type' airports within search area.
//------------------------------------------------------------------------------
int DsAirportClient::queryByType(const DsAirport::AirportType type)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsAirport,dsGetByType,type);
}


//------------------------------------------------------------------------------
// queryByFreq() -- find all airports within search area with 'freq' frequency
// ILS components.
//------------------------------------------------------------------------------
int DsAirportClient::queryByFreq(const float freq)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithFloatArg(dsAirport,dsGetByFreq,freq);
}


//------------------------------------------------------------------------------
// queryByChannel() -- find all airports within search area with 'chan' channel
// ILS components.
//------------------------------------------------------------------------------
int DsAirportClient::queryByChannel(const int chan)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsAirport,dsGetByChannel,chan);
}


//------------------------------------------------------------------------------
// getNumRunwayRecords() -- get the number of runway records
//------------------------------------------------------------------------------
int DsAirportClient::getNumRunwayRecords(const char* apkey)
{
   int n = 0;
   getWithStringArg(dsRunway,dsGetNumRecords,apkey);
   if (isRespMsgOk()) n = respMsg->value.nrl;
   return n;
}


//------------------------------------------------------------------------------
// queryRunwayByNumber() -- find runway record by number
//------------------------------------------------------------------------------
int DsAirportClient::queryRunwayByNumber(const char* apkey, const int n)
{
   dcSetRecordNumber(n);
   return getWithStringArg(dsRunway,dsGetByNumber,apkey);
}


//------------------------------------------------------------------------------
// queryRunwayByIdent() -- find runway record by its identifier
//------------------------------------------------------------------------------
int DsAirportClient::queryRunwayByIdent(const char* id)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsRunway,dsGetByIdent,id);
}


//------------------------------------------------------------------------------
// queryRunwayBySubkey() -- find runway record(s) by the key/subkey
//------------------------------------------------------------------------------
int DsAirportClient::queryRunwayBySubkey(const char* subkey)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsRunway,dsGetByKey,subkey);
}


//------------------------------------------------------------------------------
// queryRunwayByRange() -- find runway records by range
//------------------------------------------------------------------------------
int DsAirportClient::queryRunwayByRange()
{
   return queryRunwayByLength(0.0f);
}


//------------------------------------------------------------------------------
// queryRunwayByLength() -- find runway records by min length
//------------------------------------------------------------------------------
int DsAirportClient::queryRunwayByLength(const float minRwLen)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsRunway,dsGetByLength,int(minRwLen));
}


//------------------------------------------------------------------------------
// queryRunwayByFreq() -- find all runwayss within search area with 'freq'
// frequency ILS components.
//------------------------------------------------------------------------------
int DsAirportClient::queryRunwayByFreq(const float freq)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithFloatArg(dsRunway,dsGetByFreq,freq);
}


//------------------------------------------------------------------------------
// queryRunwayByChannel() -- find all airports within search area with 'chan'
// channel ILS components.
//------------------------------------------------------------------------------
int DsAirportClient::queryRunwayByChannel(const int chan)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsRunway,dsGetByChannel,chan);
}


//------------------------------------------------------------------------------
// getNumIlsRecords() -- get the number of ILS records
//------------------------------------------------------------------------------
int DsAirportClient::getNumIlsRecords(const char* apkey)
{
   int n = 0;
   getWithStringArg(dsIls,dsGetNumRecords,apkey);
   if (isRespMsgOk()) n = respMsg->value.nrl;
   return n;
}


//------------------------------------------------------------------------------
// queryIlsByNumber() -- find ILS record by number
//------------------------------------------------------------------------------
int DsAirportClient::queryIlsByNumber(const char* key, const int n)
{
   dcSetRecordNumber(n);
   return getWithStringArg(dsIls,dsGetByNumber,key);
}


//------------------------------------------------------------------------------
// queryIlsByIdent() -- find ILS record by its identifier
//------------------------------------------------------------------------------
int DsAirportClient::queryIlsByIdent(const char* id)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsIls,dsGetByIdent,id);
}


//------------------------------------------------------------------------------
// queryIlsBySubkey() -- find ILS record(s) by the key/subkey
//------------------------------------------------------------------------------
int DsAirportClient::queryIlsBySubkey(const char* subkey)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsIls,dsGetByKey,subkey);
}


//------------------------------------------------------------------------------
// queryIlsByRange() -- find ILS records by range
//------------------------------------------------------------------------------
int DsAirportClient::queryIlsByRange()
{
   return queryIlsByType(DsIls::any);
}


//------------------------------------------------------------------------------
// queryIlsByType() -- find ILS records by type
//------------------------------------------------------------------------------
int DsAirportClient::queryIlsByType(const DsIls::IlsType type)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsIls,dsGetByType,type);
}


//------------------------------------------------------------------------------
// queryIlsByFreq() -- find ILS records by freq
//------------------------------------------------------------------------------
int DsAirportClient::queryIlsByFreq(const float freq)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithFloatArg(dsIls,dsGetByFreq,freq);
}


//------------------------------------------------------------------------------
// queryIlsByChannel() -- find ILS records by channel
//------------------------------------------------------------------------------
int DsAirportClient::queryIlsByChannel(const int chan)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsIls,dsGetByChannel,chan);
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void DsAirportClient::serialize() const
{
   DsAirport airport;
   DsRunway  runway;
   DsIls     ils;
   int n = dcNumberOfRecords(dsAirport);
   for (int i = 0; i < n; i++) {
      const char* s = dcRecord(dsAirport,i);
      if (s != 0) {
	 airport.setRecord( s );
	 airport.print();

	 char apkey[AP_KEY_LEN+1];
	 airport.key(apkey);

         int nr = getNumRunwayRecords(apkey);
         for (int i1 = 0; i1 < nr; i1++) {
	    queryRunwayByNumber(apkey,i1);
	    const char* ss = dcGetRecord(0);
	    runway.setRecord(ss);
	    cout << "Runway: ";
	    runway.print();
	 }

         int ni = getNumIlsRecords(apkey);
         for (int i2 = 0; i2 < ni; i2++) {
	    queryIlsByNumber(apkey,i2);
	    const char* ss = dcGetRecord(0);
	    ils.setRecord(ss);
	    cout << "ILS:    ";
	    ils.print();
	 }

      }
   }
}


void DsAirportClient::printResults()
{
   DsAirport airport;
   int n = dcNumberFound();
   for (int i = 0; i < n; i++) {
      const char* s = dcGetRecord(i);
      if (s != 0) {
	 airport.setRecord( s );
	 airport.print();
      }
   }
}
