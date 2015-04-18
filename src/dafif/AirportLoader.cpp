
#include "openeaagles/dafif/AirportLoader.h"
#include "openeaagles/basic/FileReader.h"

#include <cstring>
#include <cstdlib>
#include <cmath>

namespace Eaagles {
namespace Dafif {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(AirportLoader,"AirportLoader")
EMPTY_SERIALIZER(AirportLoader)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
AirportLoader::AirportLoader() : Database()
{
   STANDARD_CONSTRUCTOR()
   // default file
   db->setPathname("/eaagles3rdParty/data/dafif/fullall/");
   db->setFilename("file0");
   firstAirport = nullptr;
}

AirportLoader::AirportLoader(
                  const char* country,
                  const char* file,
                  const char* path)
               : Database()
{
   STANDARD_CONSTRUCTOR()

   db->setPathname(path);
   db->setFilename(file);
   load(country);
   firstAirport = nullptr;
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void AirportLoader::copyData(const AirportLoader& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void AirportLoader::deleteData()
{
}


//------------------------------------------------------------------------------
// load() --
//------------------------------------------------------------------------------
bool AirportLoader::load(const char* country)
{
   // ---
   // Make sure the database file is open
   // ---
   if ( !openDatabaseFile()  ) {
      // Something is wrong!
       if (isMessageEnabled(MSG_ERROR)) {
           std::cerr << "AirportLoader::load() -- Error; unable to load Airports!" << std::endl;
       }
      return false;
   }


// ---
// first look for airport records
// ---
   Airport airport;
   const char* r = db->getFirstRecord();
   airport.setRecord(r);
   while (r != nullptr && airport.formatCode() == 1) {

      // Is the airport in the correct country?
      int inArea = true;
      if ( country != nullptr ) inArea = airport.isCountryCode(country);

      if ( inArea ) {

         if (nrl > ncache) {
             if (isMessageEnabled(MSG_ERROR)) {
                 std::cerr << "AirportLoader: key table too small; ";
                 std::cerr << "increase AIRPORT_MAX_RECORDS" << std::endl;
             }
             std::exit(EXIT_FAILURE);
         }

         // The airport is in the correct country!

         // Create a new airport key
         rl[nrl] = new AirportKey( db->getRecordNumber(), airport );

         // update the airport key count
         nrl++;

      }

      r = db->getNextRecord();
      airport.setRecord(r);

   }


// ---
// Create an array of Key pointers for the results of the queries
// ---

   nql = 0;
   if (nrl > 0) {
      ql = new Key*[nrl];
   }


// ---
// Next look for runway records
// ---
   Runway runway;
   runway.setRecord(r);
   while (r != nullptr && runway.formatCode() == 2) {

      // Have the first record of a runway!
      char rwbuf[RUNWAY_RECORD_LEN];
      lcStrncpy(rwbuf, RUNWAY_RECORD_LEN, r, AIRPORT_RECORD_LEN);
      int idx = db->getRecordNumber();

      // Look for the second record
      r = db->getNextRecord();
      runway.setRecord(r);
      if (r == nullptr || runway.formatCode() != 3) {
         // something is wrong -- must have a second record
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "AirportLoader::load: missing second record";
              std::cerr << " of a runway description!" << std::endl;
          }
          std::exit(EXIT_FAILURE);
      }

      // Have the second record!
      lcStrncpy( &rwbuf[AIRPORT_RECORD_LEN], AIRPORT_RECORD_LEN, r, AIRPORT_RECORD_LEN);
      runway.setRecord(rwbuf);

      // Find this runway's airport key
      char apKey[AP_KEY_LEN+1];
      runway.airportKey(apKey);
      int n = queryByKey(apKey);
      if (n > 0) {

         // runway's airport key
         AirportKey* apk = static_cast<AirportKey*>(ql[0]);

         // create an runway key
         RunwayKey* rwk = new RunwayKey( idx, runway );

         // add the runway key to the airport key
         rwk->next = apk->runways;
         apk->runways = rwk;
         rwk->parent = apk;

      }

      r = db->getNextRecord();
      runway.setRecord(r);
   }



#ifndef ALT_ILS_FILE    /* read ILS records from DAFIF file */

// ---
// Next skip arresting systems records
// ---

   while (r != nullptr && runway.formatCode() == 4) {
      r = db->getNextRecord();
      runway.setRecord(r);
   }


// ---
// Next look for ils records
// ---
   Ils ils;
   ils.setRecord(r);
   while (r != nullptr && ils.formatCode() == 5) {

      // Have the first record of a ils!
      char ilsbuf[ILS_RECORD_LEN];
      lcStrncpy(ilsbuf, ILS_RECORD_LEN, r, AIRPORT_RECORD_LEN);
      int idx = db->getRecordNumber();

      // Look for the second record
      r = db->getNextRecord();
      ils.setRecord(r);
      if (r == nullptr || ils.formatCode() != 6) {
         // something is wrong -- must have a second record
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "AirportLoader::load: missing second record";
              std::cerr << " of an ils description!" << std::endl;
          }
          std::exit(EXIT_FAILURE);
      }

      // Have the second record!
      lcStrncpy( &ilsbuf[AIRPORT_RECORD_LEN], AIRPORT_RECORD_LEN, r, AIRPORT_RECORD_LEN);
      ils.setRecord(ilsbuf);

      // Find this ils's runway ident
      char rwId[ILS_RW_ID_LEN+1];
      ils.runwayIdent(rwId);
      int n = queryRunwayByIdent(rwId);
      if (n > 0) {

         // ils's runway key
         RunwayKey* rwk = static_cast<RunwayKey*>(ql[0]);

         // create an ils key
         IlsKey* ilsk = new IlsKey( idx, ils );

         // add the ils key to the runway key
         ilsk->next = rwk->ils;
         rwk->ils = ilsk;
         ilsk->parent = rwk;

      }

      r = db->getNextRecord();
      ils.setRecord(r);
   }

#else   /* read ILS records from alternate ILS file */

   {
      // Parser the file
      AltIlsRecord* ilsr = dsAIParse(altIlsFile);

      // process records until all are read
      while (ilsr != nullptr) {

         // Create the ils's runway identifier
         char rwId[ILS_RW_ID_LEN+1];
         lcStrcpy(rwId, (ILS_RW_ID_LEN+1), ilsr->apKey);
         fillSpaces(rwId,AP_KEY_LEN);
         lcStrcat(rwId, (ILS_RW_ID_LEN+1), ilsr->rwEndId);

         // find the runway with this identifier
         int n = queryRunwayByIdent(rwId);
         if (n > 0) {

            char key[ILS_KEY_LEN+1];
            lcStrcpy(key,ILS_KEY_LEN+1,rwId);
            fillSpaces(key,ILS_KEY_LEN);

            // We have the runway key ...
            RunwayKey* rwk = (RunwayKey*) ql[0];

            // Get the airport record from the database
            AirportKey* apk = rwk->parent;
            r = dbGetRecord(apk);
            airport.setRecord(r);

            // get the mag var from the airport record
            float magvar = airport.magVariance();

            // Get the runway record from the database
            r = dbGetRecord(rwk);
            runway.setRecord(r);

            // setup localizer component data
            {
               double nlat(0.0), nlon(0.0);
               float  elev(0.0);
               key[ILS_KEY_LEN-1] = 'Z';
               Ils::IlsType type = Ils::localizer;

               computeIlsLL(&nlat, &nlon, &elev, ilsr->rwEndId,
               runway, magvar, ilsr->lcd+runway.length());
               if (ilsr->lcd < 0.0f) {
                  nlat = ilsr->lcLat;
                  nlon = ilsr->lcLon;
                  elev = ilsr->lcElev;
               }

               // create an ils key for the localizer component
               IlsKey* lk = new IlsKey(long(ALT_ILS_IDX));
               lk->lat = nlat;
               lk->lon = nlon;
               dsGetString(lk->key, key, ILS_KEY_LEN);
               dsGetString(lk->id, ilsr->ident, ILS_IDENT_LEN);
               lk->freq = ilsr->freq;
               lk->chan = ilsr->chan;
               lk->type = type;
               lk->gs   = ilsr->gs;
               lk->magvar = magvar;
               lk->elev = elev;
               dsGetString(lk->name, ilsr->name, ILS_NAME_LEN);

               // add the localizer component key to the runway key
               lk->next = rwk->ils;
               rwk->ils = lk;
               lk->parent = rwk;
            }

            // setup glide slope component data
            {
               double nlat(0.0), nlon(0.0);
               float  elev(0.0);

               key[ILS_KEY_LEN-1] = 'G';
               Ils::IlsType type = Ils::glideSlope;
               computeIlsLL(&nlat, &nlon, &elev, ilsr->rwEndId,
               runway, magvar, ilsr->gsd);
               if (ilsr->gsd < 0.0f) {
                  nlat = ilsr->gsLat;
                  nlon = ilsr->gsLon;
                  elev = ilsr->gsElev;
               }

               // create an ils key for the glode slope component
               IlsKey* gsk = new IlsKey(long(ALT_ILS_IDX));
               gsk->lat = nlat;
               gsk->lon = nlon;
               dsGetString(gsk->key, key, ILS_KEY_LEN);
               dsGetString(gsk->id, ilsr->ident, ILS_IDENT_LEN);
               gsk->freq = ilsr->freq;
               gsk->chan = ilsr->chan;
               gsk->type = type;
               gsk->gs   = ilsr->gs;
               gsk->magvar = magvar;
               gsk->elev = elev;
               dsGetString(gsk->name, ilsr->name, ILS_NAME_LEN);

               // add the glide slope component key to the runway key
               gsk->next = rwk->ils;
               rwk->ils = gsk;
               gsk->parent = rwk;
            }

            // setup Inner marker component data
            if (ilsr->imd != 0.0f) {
               double nlat(0.0), nlon(0.0);
               float  elev(0.0);

               Ils::IlsType type = Ils::innerMarker;
               key[ILS_KEY_LEN-1] = 'I';
               computeIlsLL(&nlat, &nlon, &elev, ilsr->rwEndId, runway,
               magvar, static_cast<int>(ilsr->imd*NM2FT));
               if (ilsr->imd < 0.0f) {
                  nlat = ilsr->imLat;
                  nlon = ilsr->imLon;
                  elev = ilsr->imElev;
               }

               // create an ils key for the inner marker component
               IlsKey* mk = new IlsKey(static_cast<long>(ALT_ILS_IDX));
               mk->lat = nlat;
               mk->lon = nlon;
               dsGetString(mk->key, key, ILS_KEY_LEN);
               dsGetString(mk->id, ilsr->ident, ILS_IDENT_LEN);
               mk->freq = ilsr->freq;
               mk->chan = ilsr->chan;
               mk->type = type;
               mk->gs   = ilsr->gs;
               mk->magvar = magvar;
               mk->elev = elev;
               dsGetString(mk->name, ilsr->name, ILS_NAME_LEN);

               // add the inner marker component key to the runway key
               mk->next = rwk->ils;
               rwk->ils = mk;
               mk->parent = rwk;
            }

            // setup Middle marker component data
            if (ilsr->mmd != 0.0f) {
               double nlat(0.0), nlon(0.0);
               float  elev(0.0);

               Ils::IlsType type = Ils::middleMarker;
               key[ILS_KEY_LEN-1] = 'M';
               computeIlsLL(&nlat, &nlon, &elev, ilsr->rwEndId, runway,
               magvar, static_cast<int>(ilsr->mmd*NM2FT));
               if (ilsr->mmd < 0.0f) {
                  nlat = ilsr->mmLat;
                  nlon = ilsr->mmLon;
                  elev = ilsr->mmElev;
               }

               // create an ils key for the inner marker component
               IlsKey* mk = new IlsKey(static_cast<long>(ALT_ILS_IDX));
               mk->lat = nlat;
               mk->lon = nlon;
               dsGetString(mk->key, key, ILS_KEY_LEN);
               dsGetString(mk->id, ilsr->ident, ILS_IDENT_LEN);
               mk->freq = ilsr->freq;
               mk->chan = ilsr->chan;
               mk->type = type;
               mk->gs   = ilsr->gs;
               mk->magvar = magvar;
               mk->elev = elev;
               dsGetString(mk->name, ilsr->name, ILS_NAME_LEN);

               // add the inner marker component key to the runway key
               mk->next = rwk->ils;
               rwk->ils = mk;
               mk->parent = rwk;
            }

            // setup Outer marker component data
            if (ilsr->omd != 0.0f) {
               double nlat(0.0), nlon(0.0);
               float  elev(0.0);

               Ils::IlsType type = Ils::outerMarker;
               key[ILS_KEY_LEN-1] = 'O';

               computeIlsLL(&nlat, &nlon, &elev, ilsr->rwEndId, runway,
               magvar, static_cast<int>(ilsr->omd*NM2FT));
               if (ilsr->omd < 0.0f) {
                  nlat = ilsr->omLat;
                  nlon = ilsr->omLon;
                  elev = ilsr->omElev;
               }

               // create an ils key for the inner marker component
               IlsKey* mk = new IlsKey(static_cast<long>(ALT_ILS_IDX));
               mk->lat = nlat;
               mk->lon = nlon;
               dsGetString(mk->key, key, ILS_KEY_LEN);
               dsGetString(mk->id, ilsr->ident, ILS_IDENT_LEN);
               mk->freq = ilsr->freq;
               mk->chan = ilsr->chan;
               mk->type = type;
               mk->gs   = ilsr->gs;
               mk->magvar = magvar;
               mk->elev = elev;
               dsGetString(mk->name, ilsr->name, ILS_NAME_LEN);

               // add the inner marker component key to the runway key
               mk->next = rwk->ils;
               rwk->ils = mk;
               mk->parent = rwk;
            }

         }

         // get the next record
         ilsr = ilsr->next;
      }
   }

#endif


// ---
// create the ICAO list
// ---

   createIcaoList();

   dbLoaded = true;
   return true;
}


//------------------------------------------------------------------------------
// getRecordLength()
//------------------------------------------------------------------------------
int AirportLoader::getRecordLength()
{
   return Airport::RECORD_LENGTH;
}

//------------------------------------------------------------------------------
// getMaxRecords()
//------------------------------------------------------------------------------
int AirportLoader::getMaxRecords()
{
   return AIRPORT_MAX_RECORDS;
}


//------------------------------------------------------------------------------
// airport() -- get the airport by number: 0 ... numberOfRecords()-1
// getAirport() -- get airport record from the last query: 0 ... numberFound()-1
// getRunway() -- get a runway record from the last query: 0 ... numberFound()-1
// getIls() -- get an ILS record from the last query: 0 ... numberFound()-1
//------------------------------------------------------------------------------
Airport* AirportLoader::airport(const int n)
{
   Airport* ap = nullptr;
   const char* s = record(n);
   if (s != nullptr) {
      // when we have an airport record
      ap = new Airport(s);
   }
   return ap;
}

Airport* AirportLoader::getAirport(const int n)
{
   Airport* ap = nullptr;
   const char* s = getRecord(n);
   if (s != nullptr) {
      if ( Record::dsAtoln( &s[FORMAT_CODE_POS-1], FORMAT_CODE_LEN ) == 1 ) {
         // when we have an airport record
         ap = new Airport(s);
      }
   }
   return ap;
}

Runway* AirportLoader::getRunway(const int n)
{
   Runway* rw = nullptr;
   const char* s = getRecord(n);
   if (s != nullptr) {
      if ( Record::dsAtoln( &s[FORMAT_CODE_POS-1], FORMAT_CODE_LEN ) == 2 ) {
         // when we have a runway record
         rw = new Runway(s);
      }
   }
   return rw;
}

Ils* AirportLoader::getIls(const int n)
{
   Ils* ils = nullptr;
   const char* s = getRecord(n);
   if (s != nullptr) {
      if ( Record::dsAtoln( &s[FORMAT_CODE_POS-1], FORMAT_CODE_LEN ) == 5 ) {
         // when we have an ILS record
         ils = new Ils(s);
      }
   }
   return ils;
}


//------------------------------------------------------------------------------
// queryByRange() -- find all airports within search area.
//------------------------------------------------------------------------------
int AirportLoader::queryByRange()
{
   return queryAirport(Airport::ANY,0.0f);
}


//------------------------------------------------------------------------------
// queryByIdent() -- find airport(s) by identifier (same as key)
//------------------------------------------------------------------------------
int AirportLoader::queryByIdent(const char* id)
{
   return queryByKey(id);
}


//------------------------------------------------------------------------------
// queryByKey() -- find a airport by the airport record key
//------------------------------------------------------------------------------
int AirportLoader::queryByKey(const char* subkey)
{
   char apKey[AP_KEY_LEN+1];
   lcStrncpy(apKey,AP_KEY_LEN+1,subkey,AP_KEY_LEN);
   apKey[AP_KEY_LEN] = '\0';

   AirportKey key(apKey);
   Key* pkey = &key;
   return Database::sQuery(&pkey, rl, nrl, kl_cmp);
}


//------------------------------------------------------------------------------
// queryByLength() -- find all airports within search area with minimum
// runway length.
//------------------------------------------------------------------------------
int AirportLoader::queryByLength(const float minRwLen)
{
   return queryAirport(Airport::ANY,minRwLen);
}


//------------------------------------------------------------------------------
// queryByType() -- find all 'type' airports within search area.
//------------------------------------------------------------------------------
int AirportLoader::queryByType(const Airport::AirportType type)
{
   return queryAirport(type, 0.0f);
}


//------------------------------------------------------------------------------
// queryByFreq() -- find all airports within search area with 'freq' frequency
// ILS components.
//------------------------------------------------------------------------------
int AirportLoader::queryByFreq(const float freq)
{
   double mr2(FLT_MAX);
   if (mrng > 0.0f) mr2 = mrng*mrng;

   // compute range**2 to ref point and select all that have range less
   // than maxRange
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      Key* k = rl[i];
      k->rng2 = range2(k->lat,k->lon);
      if (k->rng2 < mr2) {
         if ( chkIlsFreq(static_cast<AirportKey*>(k), freq ) ) {
            ql[nql++] = k;
         }
      }
   }

   // sort and limit by range
   rangeSort2();

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryByChannel() -- find all airports within search area with 'chan' channel
// ILS components.
//------------------------------------------------------------------------------
int AirportLoader::queryByChannel(const int chan)
{
   double mr2(FLT_MAX);
   if (mrng > 0.0f) mr2 = mrng*mrng;

   // compute range**2 to ref point and select all that have range less
   // than maxRange
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      Key* k = rl[i];
      k->rng2 = range2(k->lat,k->lon);
      if (k->rng2 < mr2) {
         if ( chkIlsChan(static_cast<AirportKey*>(k), chan ) ) {
            ql[nql++] = k;
         }
      }
   }

   // sort and limit by range
   rangeSort2();

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryAirport() -- find airports within search area with minimum runway
// length and airport type.
//------------------------------------------------------------------------------
int AirportLoader::queryAirport(const Airport::AirportType type, const float minRwLen)
{
   double mr2(FLT_MAX);
   if (mrng > 0.0f) mr2 = mrng*mrng;

   // compute range**2 to ref point and select all that have range less
   // than maxRange
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      AirportKey* k = static_cast<AirportKey*>(rl[i]);
      if ( type == k->type || type == Airport::ANY ) {
         k->rng2 = range2(k->lat,k->lon);
         if (k->rng2 < mr2) {
            if ( chkRwLen( k, minRwLen ) ) {
               ql[nql++] = k;
            }
         }
      }
   }

   // sort and limit by range
   rangeSort2();

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// getNumRunwayRecords() -- get the number of runway records
//------------------------------------------------------------------------------
int AirportLoader::getNumRunwayRecords(const char* apkey)
{
   // Use queryByKey() find the airport
   queryByKey(apkey);

   // keep a pointer to the airport
   AirportKey* apk = nullptr;
   if (nql == 1) apk = static_cast<AirportKey*>(ql[0]);
   nql = 0;

   // count the number of runway records
   int n = 0;
   if (apk != nullptr) {
      for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
         n++;
      }
   }

   return n;
}


//------------------------------------------------------------------------------
// queryRunwayByNumber() -- find runway record by number
//------------------------------------------------------------------------------
int AirportLoader::queryRunwayByNumber(const char* apkey, const int n)
{
   // Use queryByKey() find the airport
   queryByKey(apkey);

   // keep a pointer to the airport
   AirportKey* apk = nullptr;
   if (nql == 1) apk = static_cast<AirportKey*>(ql[0]);

   // find the n'th runway record
   nql = 0;
   if (apk != nullptr) {
      int i = 0;
      for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
         if (i == n) {
            ql[0] = rwk;
            nql = 1;
            break;
         }
         i++;
      }
   }

   return nql;
}


//------------------------------------------------------------------------------
// queryRunwayByIdent() -- find runway record by its identifier (airport_id +
// runway end identifier).  Therefore, each runway records will respond to two
// identifiers: airport_id + high_end_id and airport_id + low_end_id.
//------------------------------------------------------------------------------
int AirportLoader::queryRunwayByIdent(const char* id)
{
   // Use queryByKey() find the airport
   queryByKey(id);

   // keep a pointer to the airport
   AirportKey* apk = nullptr;
   if (nql == 1) apk = static_cast<AirportKey*>(ql[0]);

   // find the runway that matches the identifier
   nql = 0;
   if (apk != nullptr) {

      char rwId[RW_XE_IDENT_LEN+1];
      lcStrncpy(rwId,RW_XE_IDENT_LEN+1,&id[AP_KEY_LEN],RW_XE_IDENT_LEN);
      rwId[RW_XE_IDENT_LEN] = '\0';
      fillSpaces(rwId,RW_XE_IDENT_LEN);

      char rwId2[RW_XE_IDENT_LEN+1];
      lcStrncpy(rwId2,RW_XE_IDENT_LEN+1,&id[AP_KEY_LEN],RW_XE_IDENT_LEN);
      rwId2[RW_XE_IDENT_LEN] = '\0';
      stripSpaces(rwId2,RW_XE_IDENT_LEN);

      for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
         if ( (std::strncmp( &rwk->key[AP_KEY_LEN], rwId, RW_XE_IDENT_LEN ) == 0) ||
              (std::strncmp( &rwk->key[AP_KEY_LEN+RW_XE_IDENT_LEN],
            rwId2, RW_XE_IDENT_LEN ) == 0) ) {
            ql[nql++] = rwk;
         }
      }
   }

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryRunwayBySubkey() -- find runway record(s) by the key/subkey
//------------------------------------------------------------------------------
int AirportLoader::queryRunwayBySubkey(const char* subkey)
{
   // truncate to length of runway key
   char rwKey[RW_KEY_LEN+1];
   lcStrncpy(rwKey,RW_KEY_LEN+1,subkey,RW_KEY_LEN);
   rwKey[RW_KEY_LEN] = '\0';
   stripSpaces(rwKey,RW_KEY_LEN);

   // Use queryByKey() find the airport
   queryByKey(rwKey);

   // keep a pointer to the airport
   AirportKey* apk = nullptr;
   if (nql == 1) apk = static_cast<AirportKey*>(ql[0]);

   // find all runways that have matching keys
   nql = 0;
   if (apk != nullptr) {
      int len = static_cast<int>(strlen(rwKey));
      for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
         if (std::strncmp(rwk->key,rwKey,len) == 0) ql[nql++] = rwk;
      }
   }

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryRunwayByRange() -- find runway records by range
//------------------------------------------------------------------------------
int AirportLoader::queryRunwayByRange()
{
   return queryRunwayByLength(0.0f);
}


//------------------------------------------------------------------------------
// queryRunwayByLength() -- find runway records by min length
//------------------------------------------------------------------------------
int AirportLoader::queryRunwayByLength(const float minRwLen)
{
   int oql = getQueryLimit();
   setQueryLimit(nrl);
   queryAirport(Airport::ANY,minRwLen);
   setQueryLimit(oql);

   // select the ILS components from these airports with the correct frequency.
   if (nql > 0) {
      makeSimpleLinkedList();
      nql = 0;
      for (AirportKey* apk = firstAirport; apk != nullptr; apk = apk->next) {
         for (RunwayKey* rwk = apk->runways; rwk != nullptr; rwk = rwk->next) {
            if (rwk->rwlen >= minRwLen) {
               ql[nql++] = rwk;
            }
         }
      }
   }

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryRunwayByFreq() -- find all runways within search area with 'freq'
// frequency ILS components.
//------------------------------------------------------------------------------
int AirportLoader::queryRunwayByFreq(const float freq)
{
   double mr2(FLT_MAX);
   if (mrng > 0.0f) mr2 = mrng*mrng;

   // compute range**2 to ref point and select all that have range less
   // than maxRange
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      AirportKey* k = static_cast<AirportKey*>(rl[i]);
      k->rng2 = range2(k->lat,k->lon);
      if (k->rng2 < mr2) {
         for (RunwayKey* rwk = k->runways; rwk != 0; rwk = rwk->next) {
            if ( chkRwIlsFreq( rwk, freq ) ) {
               ql[nql++] = rwk;
            }
         }
      }
   }

   // sort and limit by range
   rangeSort2();

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryRunwayByChannel() -- find all airports within search area with 'chan'
// channel ILS components.
//------------------------------------------------------------------------------
int AirportLoader::queryRunwayByChannel(const int chan)
{
   double mr2(FLT_MAX);
   if (mrng > 0.0f) mr2 = mrng*mrng;

   // compute range**2 to ref point and select all that have range less
   // than maxRange
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      AirportKey* k = static_cast<AirportKey*>(rl[i]);
      k->rng2 = range2(k->lat,k->lon);
      if (k->rng2 < mr2) {
         for (RunwayKey* rwk = k->runways; rwk != 0; rwk = rwk->next) {
            if ( chkRwIlsChan( rwk, chan ) ) {
               ql[nql++] = rwk;
            }
         }
      }
   }

   // sort and limit by range
   rangeSort2();

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// getNumIlsRecords() -- get the number of ILS records
//------------------------------------------------------------------------------
int AirportLoader::getNumIlsRecords(const char* rwId)
{
   // Use queryRunwayByIdent() find the runway
   queryRunwayByIdent(rwId);

   // keep a pointer to the runway
   RunwayKey* rwk = 0;
   if (nql == 1) rwk = static_cast<RunwayKey*>(ql[0]);
   nql = 0;

   // count the number of ILS records
   int n = 0;
   if (rwk != 0) {
      for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
         n++;
      }
   }

   return n;
}


//------------------------------------------------------------------------------
// queryIlsByNumber() -- find ILS record by number
//------------------------------------------------------------------------------
int AirportLoader::queryIlsByNumber(const char* rwId, const int n)
{
   // Use queryRunwayByIdent() find the runway
   queryRunwayByIdent(rwId);

   // keep a pointer to the runway
   RunwayKey* rwk = 0;
   if (nql == 1) rwk = static_cast<RunwayKey*>(ql[0]);
   nql = 0;

   // find the n'th ils record
   nql = 0;
   if (rwk != 0) {
      int i = 0;
      for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
         if (i == n) {
            ql[0] = ilsk;
            nql = 1;
            break;
         }
         i++;
      }
   }

   return nql;
}


//------------------------------------------------------------------------------
// queryIlsByIdent() -- find ILS record by its identifier
//
// ###### currently using a linear search -- need up be upgraded to a
// ###### binary search.
//------------------------------------------------------------------------------
int AirportLoader::queryIlsByIdent(const char* id)
{
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      AirportKey* apk = static_cast<AirportKey*>(rl[i]);
      for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
         for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
            if (std::strncmp(ilsk->id,id,ILS_IDENT_LEN) == 0) ql[nql++] = ilsk;
         }
      }
   }
   return nql;
}


//------------------------------------------------------------------------------
// queryIlsBySubkey() -- find ILS record(s) by the key/subkey
//------------------------------------------------------------------------------
int AirportLoader::queryIlsBySubkey(const char* subkey)
{
   // Use queryRunwayBySubkey() find the runway
   queryRunwayByIdent(subkey);

   // keep a pointer to the runway
   RunwayKey* rwk = 0;
   if (nql == 1) rwk = static_cast<RunwayKey*>(ql[0]);
   nql = 0;

   // find all ILS components that have matching keys
   nql = 0;
   if (rwk != 0) {
      int len = static_cast<int>(strlen(subkey));
      for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
         if (std::strncmp(ilsk->key,subkey,len) == 0) ql[nql++] = ilsk;
      }
   }

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryIlsByRange() -- find all ILS records in search area
//------------------------------------------------------------------------------
int AirportLoader::queryIlsByRange()
{
   return queryIlsByType(Ils::ANY);
}


//------------------------------------------------------------------------------
// queryIlsByType() -- find ILS records in search area by type
//------------------------------------------------------------------------------
int AirportLoader::queryIlsByType(const Ils::IlsType type)
{
   // find all the airports within the search area
   int oql = getQueryLimit();
   setQueryLimit(nrl);
   queryAirport(Airport::ANY,0.0f);
   setQueryLimit(oql);

   // select our requested 'type' ILS components from these airports
   if (nql > 0) {
      makeSimpleLinkedList();
      nql = 0;
      for (AirportKey* apk = firstAirport; apk != 0; apk = apk->next) {
         for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
            for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
               if (ilsk->type == type || type == Ils::ANY) {
                  ql[nql++] = ilsk;
               }
            }
         }
      }
   }

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryIlsByFreq() -- find all ILS records in search area by frequency
//------------------------------------------------------------------------------
int AirportLoader::queryIlsByFreq(const float freq)
{
   // find all the airports within the search area
   int oql = getQueryLimit();
   setQueryLimit(nrl);
   queryAirport(Airport::ANY,0.0f);
   setQueryLimit(oql);

   // select the ILS components from these airports with the correct frequency.
   if (nql > 0) {
      makeSimpleLinkedList();
      nql = 0;
      for (AirportKey* apk = firstAirport; apk != 0; apk = apk->next) {
         for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
            for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
               if ( CMP_FREQ(ilsk->freq,freq) ) {
                  ql[nql++] = ilsk;
                  if (ilsk->type == Ils::LOCALIZER) {
                     findGlideSlope(rwk,ilsk);
                  }
               }
            }
         }
      }
   }

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryIlsByChannel() -- find all ILS records in search area by channel
//------------------------------------------------------------------------------
int AirportLoader::queryIlsByChannel(const int chan)
{
   // find all the airports within the search area
   int oql = getQueryLimit();
   setQueryLimit(nrl);
   queryAirport(Airport::ANY,0.0f);
   setQueryLimit(oql);

   // select the ILS components from these airports with the correct channel#
   if (nql > 0) {
      makeSimpleLinkedList();
      nql = 0;
      for (AirportKey* apk = firstAirport; apk != 0; apk = apk->next) {
         for (RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
            for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
               if (ilsk->chan == chan) {
                  ql[nql++] = ilsk;
                  if (ilsk->type == Ils::LOCALIZER) {
                     findGlideSlope(rwk,ilsk);
                  }
               }
            }
         }
      }
   }

   // limit number of result records
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// findGlideSlope() -- find matching glideslope record
//------------------------------------------------------------------------------
void
AirportLoader::findGlideSlope(const RunwayKey* rwk, const IlsKey* lk)
{
   // get the runway key without component type
   int locKeyLen = ILS_KEY_LEN - 1;
   char locKey[ILS_KEY_LEN];
   lcStrncpy(locKey,ILS_KEY_LEN,lk->key,locKeyLen);
   locKey[locKeyLen] = '\0';
   stripSpaces(locKey,locKeyLen);

   // find glide slope record that matches the localizer key
   for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
      if (ilsk->type == Ils::GLIDESLOPE) {
         if (std::strncmp(locKey,ilsk->key,locKeyLen) == 0) {
            ql[nql++] = ilsk;
         }
      }
   }
}


//------------------------------------------------------------------------------
// qsort and bsearch callbacks
//------------------------------------------------------------------------------
int AirportLoader::kl_cmp(const void* p1, const void* p2)
{
   const AirportKey* k1 = *(static_cast<const AirportKey**>(const_cast<void*>(p1)));
   const AirportKey* k2 = *(static_cast<const AirportKey**>(const_cast<void*>(p2)));

   // compare the keys
   int result = std::strcmp(k1->key, k2->key);

   return result;
}

//------------------------------------------------------------------------------
// chkRwLen() -- checks if the airport has a runway of at least minRwLen.
//------------------------------------------------------------------------------
int AirportLoader::chkRwLen(const AirportKey* key, const float minRwLen)
{
   if (minRwLen <= 0) return true;
   for (const RunwayKey* rwk = key->runways; rwk != 0; rwk = rwk->next) {
      if (rwk->rwlen >= minRwLen) return true;
   }
   return 0;
}


//------------------------------------------------------------------------------
// chkRwIlsFreq() -- checks if the runway has an ILS component with 'freq'
// chkIlsFreq() -- checks if the airport has an ILS component with 'freq'
//------------------------------------------------------------------------------
int AirportLoader::chkRwIlsFreq(const RunwayKey* rwk, const float freq)
{
   if (freq <= 0.0f || rwk == nullptr) return 0;
   for (const IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
      if ( CMP_FREQ(ilsk->freq,freq) ) return true;
   }
   return 0;
}

int AirportLoader::chkIlsFreq(const AirportKey* apk, const float freq)
{
   if (freq <= 0.0f || apk == nullptr) return true;
   for (const RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
      if ( chkRwIlsFreq(rwk,freq) ) return true;
   }
   return 0;
}


//------------------------------------------------------------------------------
// chkRwIlsChan() -- checks if the runway has an ILS component with 'chan'
// chkIlsChan() -- checks if the airport has an ILS component with 'chan'
//------------------------------------------------------------------------------
int AirportLoader::chkRwIlsChan(const RunwayKey* rwk, const int chan)
{
   if (chan <= 0 || rwk == nullptr) return 0;
   for (const IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
      if (ilsk->chan == chan) return true;
   }
   return 0;
}

int AirportLoader::chkIlsChan(const AirportKey* apk, const int chan)
{
   if (chan <= 0 || apk == nullptr) return true;
   for (const RunwayKey* rwk = apk->runways; rwk != 0; rwk = rwk->next) {
      if ( chkRwIlsChan(rwk,chan) ) return true;
   }
   return 0;
}


//------------------------------------------------------------------------------
// makeLinkedList() -- make the found airports into a linked list.
//------------------------------------------------------------------------------
void AirportLoader::makeSimpleLinkedList()
{
   firstAirport = nullptr;
   if (nql > 0) {
      firstAirport = static_cast<AirportKey*>(ql[0]);
      AirportKey* lastAP = firstAirport;
      lastAP->next = nullptr;
      for (int i = 1; i < nql; i++) {
         lastAP->next = static_cast<AirportKey*>(ql[i]);
         lastAP = lastAP->next;
         lastAP->next = nullptr;
      }
   }
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void AirportLoader::printLoaded(std::ostream& sout)
{
   Airport airport;
   Runway runway;
   Ils ils;
   for (int i = 0; i < nrl; i++) {

      // print the AIRPORT record
      AirportKey* apk = static_cast<AirportKey*>(rl[i]);
      airport.setRecord( db->getRecord( apk->idx ) ) ;
      airport.printRecord(sout);

      // print the airport's RUNWAY records
      for (RunwayKey* rwk = apk->runways; rwk != nullptr; rwk = rwk->next) {

         runway.setRecord( dbGetRecord( rwk ) );
         sout << "Runway: ";
         runway.printRecord(sout);

         // print the runway's ILS records
         for (IlsKey* ilsk = rwk->ils; ilsk != 0; ilsk = ilsk->next) {
            ils.setRecord( dbGetRecord( ilsk ) );
            sout << "ILS:    ";
            ils.printRecord(sout);
         }

      }

   }
}


void AirportLoader::printResults(std::ostream& sout)
{
   Airport airport;
   Runway runway;
   Ils ils;
   for (int i = 0; i < nql; i++) {

      // print the AIRPORT record
      AirportKey* apk = static_cast<AirportKey*>(ql[i]);
      airport.setRecord( db->getRecord( apk->idx ) ) ;
      airport.printRecord(sout);

      // print the airport's RUNWAY records
      for (RunwayKey* rwk = apk->runways; rwk != nullptr; rwk = rwk->next) {

         runway.setRecord( dbGetRecord( rwk ) );
         sout << "Runway: ";
         runway.printRecord(sout);

         // print the runway's ILS records
         for (IlsKey* ilsk = rwk->ils; ilsk != nullptr; ilsk = ilsk->next) {
            ils.setRecord( dbGetRecord( ilsk ) );
            sout << "ILS:    ";
            ils.printRecord(sout);
         }

      }

   }
}



//------------------------------------------------------------------------------
// AirportLoader::IlsKey
//------------------------------------------------------------------------------
AirportLoader::IlsKey::IlsKey(const long idx, const Ils& ils) : Key(idx)
{
   size = ILS_RECORD_LEN;
   lat = ils.latitude();
   lon = ils.longitude();

   ils.key(key);
   ils.ident(id);
   next = 0;
   freq = ils.frequency();
   chan = ils.channel();
   type = ils.ilsType();
}

AirportLoader::IlsKey::IlsKey(const char* key1) : Key(0)
{
   size = ILS_RECORD_LEN;
   Record::dsGetString(key,key1,ILS_KEY_LEN);
   id[0] = '\0';
   next = nullptr;
   freq = 0.0f;
   chan = 0;
   type = Ils::ANY;
}

#ifdef ALT_ILS_FILE
AirportLoader::IlsKey::IlsKey(const long idx) : Key(idx)
{
   size = ILS_RECORD_LEN;
   key[0] = '\0';
   id[0] = '\0';
   name[0] = '\0';
}
#endif

AirportLoader::IlsKey::~IlsKey()
{
}

void AirportLoader::IlsKey::serialize(std::ostream& sout) const
{
   sout << idx;
   sout << ": ";
   sout << key;
   sout << ": ";

   sout << lat;
   sout << ", ";
   sout << lon;

   sout << std::endl;
}


//------------------------------------------------------------------------------
// AirportLoader::RunwayKey
//------------------------------------------------------------------------------
AirportLoader::RunwayKey::RunwayKey(const long idx, const Runway& runway) : Key(idx)
{
   runway.key(key);
   rwlen = runway.length();
   size = RUNWAY_RECORD_LEN;

   lat = runway.latitude(Runway::HIGH_END);
   lon = runway.longitude(Runway::HIGH_END);

   next = nullptr;
   ils = nullptr;
}

AirportLoader::RunwayKey::RunwayKey(const char* key1) : Key(0)
{
   size = RUNWAY_RECORD_LEN;
   Record::dsGetString(key,key1,RW_KEY_LEN);
   rwlen = 0;
   next = nullptr;
   ils = nullptr;
}


AirportLoader::RunwayKey::~RunwayKey()
{
   for (IlsKey* ilsk = ils; ilsk != 0; ilsk = ilsk->next) {
      delete ilsk;
   }
}


void AirportLoader::RunwayKey::serialize(std::ostream& sout) const
{
   sout << idx;
   sout << ": ";
   sout << key;
   sout << ": ";

   sout << lat;
   sout << ", ";
   sout << lon;
   sout << "  ";;

   sout << rwlen;

   sout << std::endl;
}

//------------------------------------------------------------------------------
// AirportLoader::AirportKey
//------------------------------------------------------------------------------
AirportLoader::AirportKey::AirportKey(const long idx,
                  const Airport& airport) : Key(idx)
{
   size = AIRPORT_RECORD_LEN;
   airport.icaoCode(icao);
   airport.key(key);

   lat = airport.latitude();
   lon = airport.longitude();
   type = airport.airportType();

   runways = nullptr;
}

AirportLoader::AirportKey::AirportKey(const char* key1) : Key(0)
{
   size = AIRPORT_RECORD_LEN;
   Record::dsGetString(key,key1,AP_KEY_LEN);
   runways = nullptr;
   type = Airport::ANY;
}


AirportLoader::AirportKey::~AirportKey()
{
   for (RunwayKey* rwk = runways; rwk != 0; rwk = rwk->next) {
      delete rwk;
   }
}


void AirportLoader::AirportKey::serialize(std::ostream& sout) const
{
   sout << idx;
   sout << ": ";
   sout << key;
   sout << ": ";

   sout << lat;
   sout << ", ";
   sout << lon;

   sout << std::endl;
}

#ifdef ALT_ILS_FILE    /* Alternate ILS support functions */
//------------------------------------------------------------------- ## alt ILS
// computeIlsLL() -- computes the ILS component latitude and longitude
//------------------------------------------------------------------------------
static void computeIlsLL(double* nlat, double* nlon, float* elev,
                           const char* rwEndId, const Runway& runway,
                           const float magvar, const int loc)
{
   // which end of the runway are we approaching?
   Runway::WhichEnd we = Runway::highEnd;
   if ( !runway.isIdent(rwEndId,we) ) we = Runway::lowEnd;

   // get the data we need
   double lat = runway.latitude(we);
   double lon = runway.longitude(we);
   double thdg = runway.magHeading(we) + magvar;
   double dist = static_cast<double>(loc);

   // compute delta latitude (degrees)
   double dlat = dist * std::cos(thdg*DEG2RAD) * FT2DEG;

   // compute delta longitude (degrees)
   double dlon = dist * std::sin(thdg*DEG2RAD) * FT2DEG * cos(lat*DEG2RAD);

   // compute new latitude/longitude/elevation
   *nlat = lat + dlat;
   *nlon = lon + dlon;
   *elev =runway.elevation(we);
}


//------------------------------------------------------------------- ## alt ILS
// printLatitude() -- converts latitude to DAFIF format
//------------------------------------------------------------------------------
void printLatitude( char* buff, double lat )
{
   buff[0] = 'N';
   double dlat = lat;
   if (dlat < 0.0) {
      dlat = -dlat;
      buff[0] = 'S';
   }

   int deg = static_cast<int>(dlat);

   double dmin = (dlat - static_cast<double>(deg)) * 60.0;
   int min = static_cast<int>(dmin);

   double dsec = (dmin - static_cast<double>(min)) * 60.0;
   int sec = static_cast<int>(dsec);

   int fsec = static_cast<int>((dsec - static_cast<double>(sec)) * 100.0 );

   std::sprintf( &buff[1], "%2d%02d%02d%02d", deg, min, sec, fsec );
}


//------------------------------------------------------------------- ## alt ILS
// printLongitude() -- converts longitude to DAFIF format
//------------------------------------------------------------------------------
void printLongitude( char* buff, double lon )
{
   buff[0] = 'E';
   double dlon = lon;
   if (dlon < 0.0) {
      dlon = -dlon;
      buff[0] = 'W';
   }

   int deg = static_cast<int>(dlon);

   double dmin = (dlon - static_cast<double>(deg)) * 60.0;
   int min = static_cast<int>(dmin);

   double dsec = (dmin - static_cast<double>(min)) * 60.0;
   int sec = static_cast<int>(dsec);

   int fsec = static_cast<int>((dsec - static_cast<double>(sec)) * 100.0 );

   std::sprintf( &buff[1], "%3d%02d%02d%02d", deg, min, sec, fsec );
}


//------------------------------------------------------------------- ## alt ILS
// printMagvar() -- converts magvar to DAFIF format
//------------------------------------------------------------------------------
void printMagvar( char* buff, float magvar )
{
   buff[0] = 'E';
   float dmv = magvar;
   if (dmv < 0.0) {
      dmv = -dmv;
      buff[0] = 'W';
   }

   int deg = static_cast<int>(dmv);

   float dmin = (dmv - static_cast<float>(deg)) * 60.0;
   int min = static_cast<int>(dmin);

   int fmin = static_cast<int>((dmin - static_cast<float>(min)) * 10.0 );

   std::sprintf( &buff[1], "%3d%02d%01d", deg, min, fmin );
}

const char* AirportLoader::createIlsRecord(const Key* key)
{
   IlsKey* ilsk = (IlsKey*) key;

// ---
// first record (format == 5)
// ---

   // format code
   dsGetString(&irec[FORMAT_CODE_POS-1], "05", FORMAT_CODE_LEN );

   // transaction code
   fillSpaces(irec,TRANSACTION_CODE_POS-1);
   dsGetString(&irec[TRANSACTION_CODE_POS-1], "1", TRANSACTION_CODE_LEN);

   // key -- Airport id + runway id + component type
   fillSpaces(irec, ILS_KEY_POS-1);
   dsGetString(&irec[ILS_KEY_POS-1], ilsk->key, ILS_KEY_LEN);

   // Name
   fillSpaces(irec, ILS_NAME_POS-1);
   dsGetString(&irec[ILS_NAME_POS-1], ilsk->name, ILS_NAME_LEN);

   // Frequency
   fillSpaces(irec, ILS_FREQUENCY_POS-1);
   int ifreq = static_cast<int>( ilsk->freq * 1000.0f + 0.5f );
   std::sprintf( &irec[ILS_FREQUENCY_POS-1], "%7dM", ifreq );

   // Channel
   fillSpaces(irec, ILS_CHANNEL_POS-1);
   std::sprintf( &irec[ILS_CHANNEL_POS-1], "%3dX", ilsk->chan );

   // Glide slope
   fillSpaces(irec, ILS_GSA_POS-1);
   int igs = static_cast<int>( ilsk->gs * 100.0f + 0.5f );
   std::sprintf( &irec[ILS_GSA_POS-1], "%3d", igs );

   // Location of localizer or glide slope
   fillSpaces(irec, ILS_LOC_POS-1);
   if ( ilsk->type == Ils::glideSlope ) {
      irec[ILS_LOC_POS-1] = ' ';
   }
   else {
      irec[ILS_LOC_POS-1] = '+';
   }
   std::sprintf( &irec[ILS_LOC_POS], "%05d", ilsk->loc );

   // Elevation
   fillSpaces(irec, ILS_ELEVATION-1);
   std::sprintf( &irec[ILS_ELEVATION-1], "%5d", ilsk->elev );

   // Latitude
   fillSpaces(irec, ILS_LATITUDE-1);
   printLatitude( &irec[ILS_LATITUDE-1], ilsk->lat );

   // Longitude
   fillSpaces(irec, ILS_LONGITUDE-1);
   printLongitude( &irec[ILS_LONGITUDE-1], ilsk->lon );

// ---
// second record (format == 6)
// ---

   // format code
   fillSpaces(irec,FORMAT_CODE_POS+142-1);
   dsGetString(&irec[FORMAT_CODE_POS+142-1], "06", FORMAT_CODE_LEN );

   // transaction code
   fillSpaces(irec,TRANSACTION_CODE_POS+142-1);
   dsGetString(&irec[TRANSACTION_CODE_POS+142-1], "1", TRANSACTION_CODE_LEN);

   // key -- Airport id + runway id + component type
   fillSpaces(irec, ILS_KEY_POS+142-1);
   dsGetString(&irec[ILS_KEY_POS+142-1], ilsk->key, ILS_KEY_LEN);

   // identifier
   fillSpaces(irec, ILS_IDENT_POS-1);
   dsGetString(&irec[ILS_IDENT_POS-1], ilsk->id, ILS_IDENT_LEN);

   // Mag Var
   fillSpaces(irec, ILS_MAGVAR-1);
   printMagvar( &irec[ILS_MAGVAR-1], ilsk->magvar );

   // all finished
   fillSpaces(irec,ILS_RECORD_LEN);

   return irec;
}
#endif

} // End Dafif namespace
} // End Eaagles namespace
