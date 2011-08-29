#include <string.h>
#include "WaypointClient.h"


//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
DsWaypointClient::~DsWaypointClient()
{
}


//------------------------------------------------------------------------------
// numberOfRecords() -- returns the number of records in this database
// numberFound() -- returns the number of records found by last query
//------------------------------------------------------------------------------
int DsWaypointClient::numberOfRecords()
{
   return dcNumberOfRecords(dsWaypoint);
}

int DsWaypointClient::numberFound()
{
   return dcNumberFound();
}


//------------------------------------------------------------------------------
// Database access functions -- The strings are only valid until the next
// access.  The user should make a copy of the string if the record is to
// be saved.
//------------------------------------------------------------------------------
const char* DsWaypointClient::record(const int n, const int size)
{
   return dcRecord(dsWaypoint,n,size);
}

const char* DsWaypointClient::getRecord(const int n, const int)
{
   return dcGetRecord(n);
}


//------------------------------------------------------------------------------
// queryByRange() -- find waypoint record(s) less than maxRange from the
// ref point (sorted by range)
//------------------------------------------------------------------------------
int DsWaypointClient::queryByRange()
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithIntArg(dsWaypoint,dsGetByRange,0);
}


//------------------------------------------------------------------------------
// queryByIdent() -- find waypoint record(s) by identifier
//------------------------------------------------------------------------------
int DsWaypointClient::queryByIdent(const char* id)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsWaypoint,dsGetByIdent,id);
}


//------------------------------------------------------------------------------
// queryByKey() -- find a waypoint record by the waypoint record key
//------------------------------------------------------------------------------
int DsWaypointClient::queryByKey(const char* key)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsWaypoint,dsGetByKey,key);
}


//------------------------------------------------------------------------------
// queryByIcao() -- find a waypoint record by the ICAO code
//------------------------------------------------------------------------------
int DsWaypointClient::queryByIcao(const char* code)
{
   dcSetArea( refLat, refLon, mrng );
   dcSetQueryLimit( qlimit );
   return getWithStringArg(dsWaypoint,dsGetByIcao,code);
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void DsWaypointClient::serialize() const
{
   DsWaypoint waypoint;
   int n = dcNumberOfRecords(dsWaypoint);
   for (int i = 0; i < n; i++) {
      const char* s = dcRecord(dsWaypoint,i);
      if (s != 0) {
	 waypoint.setRecord( s );
	 waypoint.print();
      }
   }
}

void DsWaypointClient::printResults()
{
   DsWaypoint waypoint;
   int n = dcNumberFound();
   for (int i = 0; i < n; i++) {
      const char* s = dcGetRecord(i);
      if (s != 0) {
	 waypoint.setRecord( s );
	 waypoint.print();
      }
   }
}
