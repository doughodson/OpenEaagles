
#ifdef sgi
#define _BSD_SIGNALS
#endif

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <bstring.h>
#include <string.h>
#include <signal.h>
#include "AirportLoader.h"
#include "NavaidLoader.h"
#include "WaypointLoader.h"
#include "Magvar.h"
#include "eaagles/dafifc.h"


// Network Information
static const char* service = DS_SERVICE;
static const char* proto = "udp";
static int sock_id;


// ###### Parameters 
#define MAX_RECORDS	400		/* Maximum number of standard records */
#define	STD_RECORD_SIZE	142		/* Standard DAFIF record size (bytes) */
// ######


// Response message buffer
#define MAX_BUFF_SIZE	(MAX_RECORDS*STD_RECORD_SIZE)
static char respMsgBuffer[MAX_BUFF_SIZE+sizeof(DsResponseMsg)];



//------------------------------------------------------------------------------
// inline support functions
//------------------------------------------------------------------------------

// maxRecords: determine the maximum number of records we can return
inline int maxRecords(DsResponseMsg* respMsg, const DsRequestMsg* reqMsg)
{
   // default to zero records
   int mrec = 0;

   // first, limit as a function of our MAX_BUFF_SIZE and the record size.
   if (respMsg->rsize > 0) mrec = MAX_BUFF_SIZE/respMsg->rsize;

   // second, limit to the maximum number of records requested (if > zero)
   if (mrec > reqMsg->mrec && reqMsg->mrec > 0) mrec = reqMsg->mrec;

   return mrec;
}


// copyRecord: copy a record to the response message
inline void copyRecord(DsResponseMsg* respMsg, const char* record)
{
   if (record != 0) {
      strncpy(respMsg->value.buff,record,respMsg->rsize);
      respMsg->nrec = 1;
   }
}


// copyRecords: copy record(s) to the response message
inline void copyRecords(DsResponseMsg* respMsg, DsDafifDatabase* db,
			const int n)
{
   for (int i = 0; i < n; i++) {
      const char* record = db->getRecord(i);
      if (record != 0) {
        strncpy(&respMsg->value.buff[i*respMsg->rsize], record, respMsg->rsize);
      }
   }
   respMsg->nrec = n;
}


// getByNumber: get a record from a loader by number
inline void getByNumber(DsResponseMsg* respMsg,
			const DsRequestMsg* reqMsg, DsDafifDatabase* db)
{
   const char* record = db->record(reqMsg->num);
   copyRecord(respMsg,record);
}


// getByIdent: get record(s) from a loader by identifier
inline void getByIdent(DsResponseMsg* respMsg,
		       const DsRequestMsg* reqMsg, DsDafifDatabase* db)
{
   int n = db->queryByIdent(reqMsg->arg.ident);
   copyRecords(respMsg,db,n);
}


// getByRange: query record(s) by range from a ref point
inline void getByRange(DsResponseMsg* respMsg, DsDafifDatabase* db)
{
   int n = db->queryByRange();
   copyRecords(respMsg,db,n);
}


// getByKey: query for a record by the DAFIF key
inline void getByKey(DsResponseMsg* respMsg,
		     const DsRequestMsg* reqMsg, DsDafifDatabase* db)
{
   db->queryByKey( reqMsg->arg.ident );
   const char* record = db->getRecord(0);
   if (record != 0) {
      strncpy(respMsg->value.buff,record,respMsg->rsize);
      respMsg->nrec = 1;
   }
}


// getByIcao: query for a record by the ICAO code
inline void getByIcao(DsResponseMsg* respMsg,
		     const DsRequestMsg* reqMsg, DsDafifDatabase* db)
{
   db->queryByIcao( reqMsg->arg.ident );
   const char* record = db->getRecord(0);
   if (record != 0) {
      strncpy(respMsg->value.buff,record,respMsg->rsize);
      respMsg->nrec = 1;
   }
}


//------------------------------------------------------------------------------
// msg_handler() -- signal (interrupt) handler for incoming messages.
//------------------------------------------------------------------------------
static void msg_handler(...)
{
}


//------------------------------------------------------------------------------
// initServer() -- initialize the network socket 
//------------------------------------------------------------------------------
static int initServer()
{
   // Get the port address of the service we provide
   struct servent* sv = getservbyname(service, proto);
   if (sv == 0) {
      cerr << "Service: " << service << ":" << proto << " not found!" << endl;
      return 0;
   }

   // Create a socket
   int sid = socket(PF_INET, SOCK_DGRAM, 0);
   if ( sid <= 0 ) {
      perror("Unable to create socket!");
      return 0;
   }

   // Bind the socket
   struct sockaddr_in sname;
   int snlen = sizeof(sname);
   bzero( (char*)&sname, snlen );
   sname.sin_family = AF_INET;
   sname.sin_port = sv->s_port;
   sname.sin_addr.s_addr = INADDR_ANY;
   int b_ok = bind(sid, &sname, snlen);
   if ( b_ok != 0 ) {
      perror("bind socket to port failed!");
      return 0;
   }


   // Setup signal handler
   signal(SIGIO, msg_handler);
   if (fcntl(sid,  F_SETOWN, getpid()) < 0) {
      perror("Unable to set owner id");
      return 0;
   }

   // Async/non-blocked I/O
   int fl = fcntl(sid,F_GETFL);
   fl |= FASYNC;
   fl |= FNONBLK;
   if (fcntl(sid, F_SETFL, fl) < 0) {
      perror("Unable to set the FASYNC and FNONBLK flags");
      return 0;
   }

   return sid;
}


//------------------------------------------------------------------------------
// queryMagVar() -- Handle request for magnetic variation
//------------------------------------------------------------------------------
static int queryMagVar(DsResponseMsg* respMsg,
		       const DsRequestMsg* reqMsg, DsMagvar* mvar)
{
   respMsg->nrec  = 1;
   respMsg->rsize = 0;

   respMsg->value.magvar =
         mvar->magvar(reqMsg->reflat, reqMsg->reflon, 0.0f, reqMsg->arg.time);

   return sizeof(DsResponseMsg);
}


//------------------------------------------------------------------------------
// queryAPdb() -- Handle queries to the Airport database
//------------------------------------------------------------------------------
static int queryAPdb(DsResponseMsg* respMsg,
		     const DsRequestMsg* reqMsg, DsAirportLoader* apdb)
{
   int n;

   // Set the record size 
   if (reqMsg->rsize != DS_DEFAULT_SIZE)
      respMsg->rsize = reqMsg->rsize;			// Client defined
   else
      respMsg->rsize = apdb->recordSize();		// Loader defined

   // Set search parameters
   apdb->setArea(reqMsg->reflat,reqMsg->reflon,reqMsg->mrng);
   apdb->setQueryLimit( maxRecords(respMsg,reqMsg) );

   // default to nothing found
   respMsg->nrec = 0;

   switch (reqMsg->request) {

   // dsGetNumRecords -- return the number of records loader
   case dsGetNumRecords :
      respMsg->value.nrl = apdb->numberOfRecords();
      break;

   // dsGetByNumber -- query for a airport record by number
   case dsGetByNumber :
      getByNumber(respMsg,reqMsg,apdb);
      break;

   // dsGetByIdent -- query for a airport by identifier
   case dsGetByIdent :
      getByIdent(respMsg,reqMsg,apdb);
      break;

   // dsGetByKey -- query for a airport by DAFIF key
   case dsGetByKey :
      getByKey(respMsg,reqMsg,apdb);
      break;

   // dsGetByIcao -- query for a airport by ICAO code
   case dsGetByIcao :
      getByIcao(respMsg,reqMsg,apdb);
      break;

   // dsGetByRange -- query for airport records by range from a ref point
   case dsGetByRange :
      getByRange(respMsg,apdb);
      break;

   // dsGetByType -- query airport records by type
   // from a ref point
   case dsGetByType :
      n = apdb->queryByType(DsAirport::AirportType(reqMsg->arg.type));
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByLength -- query for airport records by runway length and range
   // from a ref point
   case dsGetByLength :
      n = apdb->queryByLength(reqMsg->arg.len);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByFreq -- query for airports that have IlS components with freq
   case dsGetByFreq :
      n = apdb->queryByFreq(reqMsg->arg.freq);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByChannel -- query for airports that have IlS components with chan
   case dsGetByChannel :
      n = apdb->queryByChannel(reqMsg->arg.chan);
      copyRecords(respMsg,apdb,n);
      break;

   default :
      break;

   }
   return sizeof(DsResponseMsg) + (respMsg->nrec * respMsg->rsize);
}


//------------------------------------------------------------------------------
// queryNAVdb() -- Handle queries to the NAVAID database
//------------------------------------------------------------------------------
static int queryNAVdb(DsResponseMsg* respMsg,
		      const DsRequestMsg* reqMsg, DsNavaidLoader* ndb)
{
   int n;

   // Set the record size 
   if (reqMsg->rsize != DS_DEFAULT_SIZE)
      respMsg->rsize = reqMsg->rsize;			// Client defined
   else
      respMsg->rsize = ndb->recordSize();		// Loader defined

   // Set search parameters
   ndb->setArea(reqMsg->reflat,reqMsg->reflon,reqMsg->mrng);
   ndb->setQueryLimit( maxRecords(respMsg,reqMsg) );

   // default to nothing found
   respMsg->nrec = 0;

   switch (reqMsg->request) {

   // dsGetNumRecords -- return the number of records loader
   case dsGetNumRecords :
      respMsg->value.nrl = ndb->numberOfRecords();
      break;

   // dsGetByNumber -- query for a NAVAID record by number
   case dsGetByNumber :
      getByNumber(respMsg,reqMsg,ndb);
      break;

   // dsGetByIdent -- query for a NAVAID by identifier
   case dsGetByIdent :
      getByIdent(respMsg,reqMsg,ndb);
      break;

   // dsGetByKey -- query for a NAVAID by DAFIF key
   case dsGetByKey :
      getByKey(respMsg,reqMsg,ndb);
      break;

   // dsGetByIcao -- query for a NAVAID by ICAO code
   case dsGetByIcao :
      getByIcao(respMsg,reqMsg,ndb);
      break;

   // dsGetByRange -- query for NAVAID records by range from a ref point
   case dsGetByRange :
      getByRange(respMsg,ndb);
      break;

   // dsGetByType -- query for NAVAID records by type
   case dsGetByType :
      n = ndb->queryByType(DsNavaid::NavaidType(reqMsg->arg.type));
      copyRecords(respMsg,ndb,n);
      break;

   // dsGetByFreq -- query for a NAVAID by freq
   case dsGetByFreq :
      n = ndb->queryByFreq(reqMsg->arg.freq);
      copyRecords(respMsg,ndb,n);
      break;

   // dsGetByChannel -- query for a NAVAID by channel number
   case dsGetByChannel :
      n = ndb->queryByChannel(reqMsg->arg.chan);
      copyRecords(respMsg,ndb,n);
      break;

   default :
      break;

   }

   return sizeof(DsResponseMsg) + (respMsg->nrec * respMsg->rsize);
}


//------------------------------------------------------------------------------
// queryWPdb() -- Handle queries to the Waypoint database
//------------------------------------------------------------------------------
static int queryWPdb(DsResponseMsg* respMsg,
		     const DsRequestMsg* reqMsg, DsWaypointLoader* wpdb)
{
   // Set the record size 
   if (reqMsg->rsize != DS_DEFAULT_SIZE)
      respMsg->rsize = reqMsg->rsize;			// Client defined
   else
      respMsg->rsize = wpdb->recordSize();		// Loader defined

   // Set search parameters
   wpdb->setArea(reqMsg->reflat,reqMsg->reflon,reqMsg->mrng);
   wpdb->setQueryLimit( maxRecords(respMsg,reqMsg) );

   // default to nothing found
   respMsg->nrec = 0;

   switch (reqMsg->request) {

   // dsGetNumRecords -- return the number of records loader
   case dsGetNumRecords :
      respMsg->value.nrl = wpdb->numberOfRecords();
      break;

   // dsGetByNumber -- query for a waypoint record by number
   case dsGetByNumber :
      getByNumber(respMsg,reqMsg,wpdb);
      break;

   // dsGetByIdent -- query for a waypoint by identifier
   case dsGetByIdent :
      getByIdent(respMsg,reqMsg,wpdb);
      break;

   // dsGetByKey -- query for a waypoint DAFIF by key
   case dsGetByKey :
      getByKey(respMsg,reqMsg,wpdb);
      break;

   // dsGetByIcao -- query for a waypoint by ICAO code
   case dsGetByIcao :
      getByIcao(respMsg,reqMsg,wpdb);
      break;

   // dsGetByRange -- query for waypoint records by range from a ref point
   case dsGetByRange :
      getByRange(respMsg,wpdb);
      break;

   default :
      break;

   }
   return sizeof(DsResponseMsg) + (respMsg->nrec * respMsg->rsize);
}


//------------------------------------------------------------------------------
// queryRwdb() -- Handle queries for runways from the Airport database
//------------------------------------------------------------------------------
static int queryRwdb(DsResponseMsg* respMsg,
		     const DsRequestMsg* reqMsg, DsAirportLoader* apdb)
{
   int n;

   // Set the record size 
   if (reqMsg->rsize != DS_DEFAULT_SIZE)
      respMsg->rsize = reqMsg->rsize;			// Client defined
   else
      respMsg->rsize = DsRunway::recordLength;		// ILS records size

   // Set search parameters
   apdb->setArea(reqMsg->reflat,reqMsg->reflon,reqMsg->mrng);
   apdb->setQueryLimit( maxRecords(respMsg,reqMsg) );

   // default to nothing found
   respMsg->nrec = 0;

   switch (reqMsg->request) {

   // dsGetNumRecords -- return the number of records loader
   case dsGetNumRecords :
      respMsg->value.nrl = apdb->getNumRunwayRecords(reqMsg->arg.ident);
      break;

   // dsGetByNumber -- query for a runway record by number
   case dsGetByNumber :
      n = apdb->queryRunwayByNumber(reqMsg->arg.ident, reqMsg->num);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByIdent -- query for a runway by ident (high or low end)
   case dsGetByIdent :
      n = apdb->queryRunwayByIdent(reqMsg->arg.ident);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByKey -- query for a runway by key
   case dsGetByKey :
      n = apdb->queryRunwayBySubkey(reqMsg->arg.ident);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByRange -- query for runways by range
   case dsGetByRange :
      n = apdb->queryRunwayByRange();
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByLength -- query for runways by minimum length
   case dsGetByLength :
      n = apdb->queryRunwayByLength(reqMsg->arg.len);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByFreq -- query for runways that have IlS components with freq
   case dsGetByFreq :
      n = apdb->queryRunwayByFreq(reqMsg->arg.freq);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByChannel -- query for runways that have IlS components with chan
   case dsGetByChannel :
      n = apdb->queryRunwayByChannel(reqMsg->arg.chan);
      copyRecords(respMsg,apdb,n);
      break;

   default :
      break;

   }
   return sizeof(DsResponseMsg) + (respMsg->nrec * respMsg->rsize);
}


//------------------------------------------------------------------------------
// queryIlsdb() -- Handle queries for ILS components from the Airport database
//------------------------------------------------------------------------------
static int queryIlsdb(DsResponseMsg* respMsg,
		     const DsRequestMsg* reqMsg, DsAirportLoader* apdb)
{
   int n;

   // Set the record size 
   if (reqMsg->rsize != DS_DEFAULT_SIZE)
      respMsg->rsize = reqMsg->rsize;		// Client defined
   else
      respMsg->rsize = DsIls::recordLength;	// ILS records size

   // Set search parameters
   apdb->setArea(reqMsg->reflat,reqMsg->reflon,reqMsg->mrng);
   apdb->setQueryLimit( maxRecords(respMsg,reqMsg) );

   // default to nothing found
   respMsg->nrec = 0;

   switch (reqMsg->request) {

   // dsGetNumRecords -- return the number of records loader
   case dsGetNumRecords :
      respMsg->value.nrl = apdb->getNumIlsRecords(reqMsg->arg.ident);
      break;

   // dsGetByNumber -- query for a runway record by number
   case dsGetByNumber :
      n = apdb->queryIlsByNumber(reqMsg->arg.ident, reqMsg->num);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByIdent -- query for a ILS by ident
   case dsGetByIdent :
      n = apdb->queryIlsByIdent(reqMsg->arg.ident);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByKey -- query for an ILS by key
   case dsGetByKey :
      n = apdb->queryIlsBySubkey(reqMsg->arg.ident);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByRange -- query for ILS components by type
   case dsGetByRange :
      n = apdb->queryIlsByRange();
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByType -- query for ILS components by type
   case dsGetByType :
      n = apdb->queryIlsByType(DsIls::IlsType(reqMsg->arg.type));
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByFreq -- query for ILS components by freq
   case dsGetByFreq :
      n = apdb->queryIlsByFreq(reqMsg->arg.freq);
      copyRecords(respMsg,apdb,n);
      break;

   // dsGetByChannel -- query for ILS components by channel number
   case dsGetByChannel :
      n = apdb->queryIlsByChannel(reqMsg->arg.chan);
      copyRecords(respMsg,apdb,n);
      break;

   default :
      break;

   }
   return sizeof(DsResponseMsg) + (respMsg->nrec * respMsg->rsize);
}


//------------------------------------------------------------------------------
// main() -- main server routine
//------------------------------------------------------------------------------
void main(int argc, char* argv[], char**)
{

// ---
// Check command line arguments
// ---

   char* ccode = 0;
   if (argc > 1) {
     ccode = argv[1];
   }


// ---
// Load the databases
// ---

   // Load the DAFIF Airport file  
   cout << "Loading airports ..." << endl;
   DsAirportLoader* apdb = new DsAirportLoader(ccode);
   cout << "Airports loaded: " << apdb->numberOfRecords() << endl;

   // Load the DAFIF NAVAID file  
   cout << "Loading navaids ..." << endl;
   DsNavaidLoader* ndb = new DsNavaidLoader(ccode);
   cout << "Navaids loaded: " << ndb->numberOfRecords() << endl;

   // Load the DAFIF Waypoint file  
   cout << "Loading waypoints ..." << endl;
   DsWaypointLoader* wpdb = new DsWaypointLoader(ccode);
   cout << "Waypoints loaded: " << wpdb->numberOfRecords() << endl;

   // Load the Magvar file  
   cout << "Loading magvar ..." << endl;
   DsMagvar* mvar = new DsMagvar();
   cout << "Magvar loaded!" << endl;


// ---
// Init the server network socket
// ---

   if ( (sock_id = initServer()) > 0) {
      // report that we're ready, willing and able!
      cout << "server: online!" << endl;
   }
   else {
      exit(EXIT_FAILURE);
   }


// ---
// Loop forever (more or less) while looking for request messages to process.
// ---

   while (1) {

      // Wait for interrupt
      pause();

      DsRequestMsg reqMsgBuff;

      struct sockaddr from;
      int fromlen = sizeof(from);

      // Try to read a message
      int n = recvfrom(sock_id, &reqMsgBuff, sizeof(DsRequestMsg), 0,
		    &from, &fromlen);

      // Did we receive any a request message?
      while (n > 0) {

	 // Yes we did!

         int respMsgSize;

         // Pointer to our response message
         DsResponseMsg* respMsg = (DsResponseMsg*) &respMsgBuffer;

         // Copy the msg identification stuff
         respMsg->seq = reqMsgBuff.seq;
         respMsg->id  = reqMsgBuff.id;

	 // Handle the query by type
         switch (reqMsgBuff.type) {

         case dsAirport:
	    respMsgSize = queryAPdb(respMsg,&reqMsgBuff,apdb);
	    break;

         case dsNavaid:
	    respMsgSize = queryNAVdb(respMsg,&reqMsgBuff,ndb);
	    break;

         case dsWaypoint :
	    respMsgSize = queryWPdb(respMsg,&reqMsgBuff,wpdb);
	    break;

         case dsMagVar:
	    respMsgSize = queryMagVar(respMsg,&reqMsgBuff,mvar);
	    break;

         case dsRunway:
	    respMsgSize = queryRwdb(respMsg,&reqMsgBuff,apdb);
	    break;

         case dsIls:
	    respMsgSize = queryIlsdb(respMsg,&reqMsgBuff,apdb);
	    break;

         default :
	    respMsg->nrec  = 0;
	    respMsg->rsize = 0;
	    respMsgSize = sizeof(DsResponseMsg);
	    break;
         }

         // send response
         sendto(sock_id, respMsg, respMsgSize, 0, &from, fromlen);

         // try to read another message
         fromlen = sizeof(from);
         n = recvfrom(sock_id, &reqMsgBuff, sizeof(DsRequestMsg), 0,
		      &from, &fromlen);

      }
   }
}

