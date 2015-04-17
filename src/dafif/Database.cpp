
#include "openeaagles/dafif/Database.h"
#include "openeaagles/dafif/Record.h"
#include "openeaagles/basic/FileReader.h"
#include "openeaagles/basic/Nav.h"
#include "openeaagles/basic/String.h"
#include "openeaagles/basic/units/Angles.h"
#include "openeaagles/basic/units/Distances.h"

#include <cstring>
#include <cmath>
#include <cstdlib>

namespace Eaagles {
namespace Dafif {

IMPLEMENT_ABSTRACT_SUBCLASS(Database,"Database")

//------------------------------------------------------------------------------
// Slot table for this form type
//------------------------------------------------------------------------------
BEGIN_SLOTTABLE(Database)
    "pathname",      // 1) Path to the file
    "filename",      // 2) File name (appended to pathname)
END_SLOTTABLE(Database)

// Map slot table to handles
BEGIN_SLOT_MAP(Database)
    ON_SLOT(1,setSlotPathname,Basic::String)
    ON_SLOT(2,setSlotFilename,Basic::String)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Database::Database()
{
   STANDARD_CONSTRUCTOR()

   db = new Basic::FileReader();

   ncache = 0;
   rl = nullptr;
   nrl = 0;

   ol = nullptr;
   nol = 0;

   ql = nullptr;
   nql = 0;
   qlimit = 0;

   refLat = 0.0f;
   refLon = 0.0f;
   coslat = 1.0f;
   mrng = 0.0f;
   dbInUse = false;
   dbLoaded = false;
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void Database::copyData(const Database& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
      db = new Basic::FileReader();
   }

   ncache = 0;
   rl = nullptr;
   nrl = 0;

   ol = nullptr;
   nol = 0;

   ql = nullptr;
   nql = 0;
   qlimit = 0;

   refLat = 0.0f;
   refLon = 0.0f;
   coslat = 1.0f;
   mrng = 0.0f;
   dbInUse = false;
   dbLoaded = false;
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void Database::deleteData()
{
   if (db != nullptr) {
      db->unref();
      db = nullptr;
   }
   dbInUse = false;
   dbLoaded = false;

   //for (int i=0; i < nrl; i++)
   //   delete rl[i];

   //if (rl != 0) free(rl);
   //if (ql != 0) delete[] ql;
}

//------------------------------------------------------------------------------
// openDatabaseFile() -- creates and opens the database file
//------------------------------------------------------------------------------
bool Database::openDatabaseFile()
{
   // Close any old files and set the record length
   db->setRecordLength(getRecordLength());

   // create keys
   ncache = getMaxRecords();
   if (rl != nullptr) delete[] rl;
   rl = new Key*[ncache];
   if (isMessageEnabled(MSG_DEBUG)) {
      std::cout << "db = " << this << ", ncache = " << ncache << std::endl;
   }

   // open the database file
   db->open();
   return db->isReady();
}


//------------------------------------------------------------------------------
// getArea(), setArea() -- get/set the search area (ref point)
//------------------------------------------------------------------------------
void Database::getArea(double* const lat, double* const lon)
{
   *lat = refLat;
   *lon = refLon;
}

void Database::getArea(double* const lat, double* const lon, double* const mr)
{
   *lat = refLat;
   *lon = refLon;
   *mr  = mrng;
}

void Database::setArea(const double lat, const double lon, const double mr)
{
   refLat = lat;
   refLon = lon;
   coslat = std::cos(lat * Basic::Angle::D2RCC);
   mrng   = mr;
}


//------------------------------------------------------------------------------
// getQueryLimit(), setQueryLimit() -- get/set query limit
//------------------------------------------------------------------------------
void Database::setQueryLimit(const int mrec)
{
   qlimit = mrec;
}

int Database::getQueryLimit()
{
   return qlimit;
}


//------------------------------------------------------------------------------
// numberOfRecords() -- returns the number of records in this database
// numberFound() -- returns the number of records found by last query
//------------------------------------------------------------------------------
int Database::numberOfRecords()
{
   return nrl;
}

int Database::numberFound()
{
   return nql;
}

//------------------------------------------------------------------------------
// range2() -- returns the square of the range between lat,lon and
// the reference point.
//------------------------------------------------------------------------------
double Database::range2(const double lat, const double lon) const
{
   const double x = (lat - refLat) * 60.0;
   const double y = (lon - refLon) * coslat * 60.0;
   return (x * x + y * y);
}

//------------------------------------------------------------------------------
// Database access functions -- The strings are only valid until the next
// access.  The user should make a copy of the string if the record is to
// be saved.
//------------------------------------------------------------------------------
const char* Database::record(const int n, const int size)
{
   const char* p = nullptr;
   if (n >= 0 && n < nrl) {
      p = dbGetRecord( rl[n], size );
   }
   return p;
}

const char* Database::getRecord(const int n, const int size)
{
   const char* p = nullptr;
   if (n >= 0 && n < nql) {
      p = dbGetRecord( ql[n], size );
   }
   return p;
}


// dbGetRecord() -- get record from the database
const char* Database::dbGetRecord( const Key* key, const int size )
{
   const char* p = nullptr;

   // size of record to read
   int ssize = key->size;
   if (size != 0) ssize = size;

#ifndef ALT_ILS_FILE

   p = db->getRecord( key->idx, ssize );

#else    /* Using Alternate ILS file */

   if (key->idx != ALT_ILS_IDX || ssize != ILS_RECORD_LEN)
      p = db->getRecord( key->idx, ssize );
   else {
      // must be an ILS record from an Airport loader
      DsAirportLoader* me = (DsAirportLoader*) this;
      p = me->createIlsRecord(key);
   }

#endif

   return p;

}

// getPathname() -- get the path name from the database file reader
const char* Database::getPathname() const
{
   return db->getPathname();
}

// getFilename() -- get the file name from the database file reader
const char* Database::getFilename() const
{
   return db->getFilename();
}

// getPathname() -- get the pathname from the database file reader
bool Database::setPathname(const char* path)
{
   return db->setPathname(path);
}

// getPathname() -- get the pathname from the database file reader
bool Database::setFilename(const char* file)
{
   return db->setFilename(file);
}


//------------------------------------------------------------------------------
// stripSpaces() -- strips spaces off end of the string
//------------------------------------------------------------------------------
void Database::stripSpaces(char buff[], const int n)
{
   int l = n-1;
   while (l >= 0 && buff[l] == ' ') buff[l--] = '\0';
}

//------------------------------------------------------------------------------
// fillSpaces() -- add spaces to the end of the string to make it n characters
// long plus a null character.
//------------------------------------------------------------------------------
void Database::fillSpaces(char buff[], const int n)
{
   for (int i = static_cast<int>(std::strlen(buff)); i < n; buff[i++] = ' ');
   buff[n] = '\0';
}

//------------------------------------------------------------------------------
// createIcaoList() -- creates a list in ICAO code order from the record list.
//------------------------------------------------------------------------------
void Database::createIcaoList()
{
   nol = 0;
   if (nrl > 0) {

      // allocate space for the ICAO list pointers
      ol = new Key*[nrl];

      // copy keys with a valid ICAO code to the ICAO list and sort
      for (int i = 0; i < nrl; i++) {
         if ( std::strlen(rl[i]->icao) > 2 ) {       // at least three characters
            ol[nol++] = rl[i];
         }
      }

      // sort the ICAO list keys
      std::qsort(ol,nol,sizeof(Key*),ol_cmp);
   }
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool Database::setSlotPathname(Basic::String* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setPathname( *msg );
   }
   return ok;
}

bool Database::setSlotFilename(Basic::String* const msg)
{
   bool ok = false;
   if (msg != nullptr) {
      ok = setFilename( *msg );
   }
   return ok;
}


//------------------------------------------------------------------------------
// query functions ---
//------------------------------------------------------------------------------

// Single query -- when you want only a single instance of a record that
// matches the key.
int Database::sQuery(Key** key, Key** base,
                            size_t n, int (*cmp)(const void*, const void*))
{
   nql = 0;
   // search the record list for matches and sort by range
   if (n != 0) {
      Key** k = static_cast<Key**>(bsearch(key, base, n, sizeof(Key*), cmp));
      if (k != nullptr) {
         ql[0] = *k;
         nql = 1;
      }
   }

   return nql;
}


// Multi-query -- when you want the full range of records that match the key.
int Database::mQuery(Key** key, Key** base,
                            size_t n, int (*cmp)(const void*, const void*))
{
   nql = 0;

   // search the record list for matches and sort by range
   if (n != 0) {
      Key** k = static_cast<Key**>(bsearch(key, base, n, sizeof(Key*), cmp));
      if (k != nullptr) {
         expandResults(key,k,cmp,base,n);
      }
   }

   // sort by range from ref point
   if (nql > 1) rangeSort();

   // limit results
   if (qlimit > 0 && nql > qlimit) nql = qlimit;

   return nql;
}


//------------------------------------------------------------------------------
// queryByIcao() -- find a record by the ICAO code
//------------------------------------------------------------------------------
int Database::queryByIcao(const char* code)
{
   Key key(code);
   Key* pkey = &key;
   return mQuery(&pkey, static_cast<Key**>(ol), nol, ol_cmp);
}


//------------------------------------------------------------------------------
// expandResults() -- The record we found by the last bsearch, keyPtr, may
// have found a record somewhere in the middle of a set that matched our
// search pattern; this function looks above and below 'keyPtr' for more
// records and put the final results in 'ql'.
//------------------------------------------------------------------------------
void Database::expandResults(Key** key, Key** keyPtr,
                                    int (*cmp)(const void*, const void*),
                                    Key** base, size_t n)
{
   // Look for the bottom end
   Key** b = keyPtr - 1;
   int rr = 0;
   while (rr == 0 && b >= base) {
      rr = (*cmp)(key,b);
      if (rr == 0) --b;
   }
   b++;

   // Look for the top end
   Key** t = keyPtr + 1;
   Key** vt = base + n;
   rr = 0;
   while (rr == 0 && t < vt) {
      rr = (*cmp)(key,t);
      if (rr == 0) t++;
   }
   --t;

   // move the results to ql
   nql = 0;
   for (Key** p = b; p <= t; p++) {
      ql[nql++] = *p;
   }
}


//------------------------------------------------------------------------------
// rangeSort() -- first compute range and then uses rangeSort2() to sort.
// rangeSort2() -- sort results by range; range must have already been computed.
//------------------------------------------------------------------------------

// rangeSort2: sort results by range; range must have already been computed.
int Database::rangeSort2()
{
   // if more than one key, sort them by range
   if (nql > 1) std::qsort(ql, nql, sizeof(Key*), rlqs);

   // reject records with a range greater than mrng
   if (mrng > 0.0f) {
      const double mr2 = mrng*mrng;
      int done = 0;
      while (nql > 0 && !done) {
         Key* k = ql[nql-1];
         if (k->rng2 > mr2) nql--;
         else done = true;
      }
   }

   return nql;
}

// rangeSort: first compute range and then uses rangeSort2() to sort.
int Database::rangeSort()
{
   // compute ranges
   for (int i = 0; i < nql; i++) {
      Key* k = ql[i];
      k->rng2 = range2(k->lat,k->lon);
   }

   return rangeSort2();
}


//------------------------------------------------------------------------------
// bsearch and qsort callbacks
//------------------------------------------------------------------------------
int Database::rlqs(const void* p1, const void* p2)
{
   const Key* k1 = *(static_cast<const Key**>(const_cast<void*>(p1)));
   const Key* k2 = *(static_cast<const Key**>(const_cast<void*>(p2)));

   int result = 0;
   if (k1->rng2 < k2->rng2)      result = -1;
   else if (k1->rng2 > k2->rng2) result =  1;

   return result;
}

int Database::ol_cmp(const void* p1, const void* p2)
{
   Key* k1 = *(static_cast<Key**>(const_cast<void*>(p1)));
   Key* k2 = *(static_cast<Key**>(const_cast<void*>(p2)));

   return std::strcmp(k1->icao,k2->icao);
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void Database::printLoaded(std::ostream& sout)
{
   for (int i=0; i < nrl; i++) {
      rl[i]->serialize(sout);
   }
}

void Database::printResults(std::ostream& sout)
{
   for (int i=0; i < nql; i++) {
      ql[i]->serialize(sout);
   }
}

void Database::printIcaoList(std::ostream& sout)
{
   std::cout << "printIcaoList: nol = " << nol << std::endl;
   for (int i = 0; i < nol; i++) {
      ol[i]->serialize(sout);
   }
}


//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* Database::getSlotByIndex(const int si)
{
    return BaseClass::getSlotByIndex(si);
}

//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
std::ostream& Database::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
    int j = 0;
    if ( !slotsOnly ) {
        sout << "( " << getFactoryName() << std::endl;
        j = 4;
    }

    if (strlen(getPathname()) > 0) {
        indent(sout,i+j);
        sout << "pathname: \"";
        sout << getPathname();
        sout << "\"" << std::endl;
    }

    if (strlen(getFilename()) > 0) {
        indent(sout,i+j);
        sout << "filename: \"";
        sout << getFilename();
        sout << "\"" << std::endl;
    }

    if ( !slotsOnly ) {
        indent(sout,i);
        sout << ")" << std::endl;
    }

    return sout;
}

//------------------------------------------------------------------------------
// Database::Key
//------------------------------------------------------------------------------
Database::Key::Key(const int idx1)
{
   idx = idx1;
   size = 0;

   lat  = 0.0f;
   lon  = 0.0f;
   rng2 = 0.0f;

   icao[0] = '\0';
}

Database::Key::Key(const float lat1, const float lon1)
{
   idx = 0;
   size = 0;

   lat  = lat1;
   lon  = lon1;
   rng2 = 0.0f;

   icao[0] = '\0';
}

Database::Key::Key(const char* code)
{
   idx = 0;
   size = 0;

   lat  = 0.0f;
   lon  = 0.0f;
   rng2 = 0.0f;

   Record::dsGetString(icao,code,ICAO_CODE_LEN);
}

Database::Key::~Key()
{
}

void Database::Key::serialize(std::ostream& sout) const
{
   sout << idx << ": ";

   sout << icao << ": ";

   sout << lat << ", ";
   sout << lon << "  ";

   sout << rng2;

   sout << std::endl;
}

} // End Dafif namespace
} // End Eaagles namespace
