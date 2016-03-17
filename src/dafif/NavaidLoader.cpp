
#include "openeaagles/dafif/NavaidLoader.h"
#include "openeaagles/base/FileReader.h"
#include <cstring>
#include <cstdlib>
#include <cfloat>

namespace oe {
namespace dafif {

IMPLEMENT_SUBCLASS(NavaidLoader,"NavaidLoader")
EMPTY_SLOTTABLE(NavaidLoader)
EMPTY_SERIALIZER(NavaidLoader)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
NavaidLoader::NavaidLoader() : Database()
{
   STANDARD_CONSTRUCTOR()

   // default file
   db->setPathname("/eaagles3rdParty/data/dafif/fullall/");
   db->setFilename("file2");

   fl = 0;
   nfl = 0;
   cl = 0;
   ncl = 0;
}

NavaidLoader::NavaidLoader(
                  const char* country,
                  const char* file,
                  const char* path)
               : Database()
{
   STANDARD_CONSTRUCTOR()

   db->setPathname(path);
   db->setFilename(file);

   fl = 0;
   nfl = 0;
   cl = 0;
   ncl = 0;

   load(country);
}


//------------------------------------------------------------------------------
// copyData() -- copy this object's data
//------------------------------------------------------------------------------
void NavaidLoader::copyData(const NavaidLoader& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) {
      db->setPathname("/eaagles3rdParty/data/dafif/fullall/");
      db->setFilename("file2");
      fl = 0;
      nfl = 0;
      cl = 0;
      ncl = 0;
   }
}

//------------------------------------------------------------------------------
// deleteData() -- delete this object's data
//------------------------------------------------------------------------------
void NavaidLoader::deleteData()
{
   delete[] fl;
   nfl = 0;
   delete[] cl;
   ncl = 0;
}

//------------------------------------------------------------------------------
// load() --
//------------------------------------------------------------------------------
bool NavaidLoader::load(const char* country)
{
   // ---
   // Make sure the database file is open
   // ---
   if ( !openDatabaseFile()  ) {
      // Something is wrong!
       if (isMessageEnabled(MSG_ERROR)) {
           std::cerr << "NavaidLoader::load() -- Error; unable to load Navaids!" << std::endl;
       }
      return false;
   }

   // ---
   // Read the file and create NavaidKey's for all records in our country
   // ---
   Navaid navaid;
   for (const char* r=db->getFirstRecord(); r != nullptr; r=db->getNextRecord()) {

      navaid.setRecord(r);

      int inArea = true;
      if ( country != nullptr ) inArea = navaid.isCountryCode(country);

      if ( inArea && nrl < ncache ) {

         if (nrl > ncache) {
             if (isMessageEnabled(MSG_ERROR)) {
                 std::cerr << "NavaidLoader: key table too small; ";
                 std::cerr << "increase NAVAID_MAX_RECORDS" << std::endl;
             }
             std::exit(EXIT_FAILURE);
         }

         rl[nrl] = new NavaidKey( db->getRecordNumber(), navaid );
         nrl++;
      }
   }

   // create the ICAO list
   createIcaoList();

   // allocate space for the freq and channel lists
   nql = 0;
   if (nrl > 0) {
      ql = new Key*[nrl];
      fl = new NavaidKey*[nrl];
      cl = new NavaidKey*[nrl];
   }


   // copy keys with a frequency > 0 to the frequency list and sort
   nfl = 0;
   for (int i = 0; i < nrl; i++) {
      fl[nfl] = static_cast<NavaidKey*>(rl[i]);
      if ( fl[nfl]->freq > 0.0f ) nfl++;
   }
   std::qsort(fl,nfl,sizeof(NavaidKey*),fl_cmp);


   // copy keys with a channel number != 0 to the channel list and sort
   ncl = 0;
   for (int i = 0; i < nrl; i++) {
      cl[ncl] = static_cast<NavaidKey*>(rl[i]);
      if ( cl[ncl]->channel != 0 ) ncl++;
   }
   std::qsort(cl,ncl,sizeof(NavaidKey*),cl_cmp);

   dbLoaded = true;
   return true;
}


//------------------------------------------------------------------------------
// getRecordLength()
//------------------------------------------------------------------------------
int NavaidLoader::getRecordLength()
{
   return Navaid::RECORD_LENGTH;
}

//------------------------------------------------------------------------------
// getMaxRecords()
//------------------------------------------------------------------------------
int NavaidLoader::getMaxRecords()
{
   return NAVAID_MAX_RECORDS;
}


//------------------------------------------------------------------------------
// navaid() -- get the NAVAID by number: 0 ... numberOfRecords()-1
// getNavaid() -- get a NAVAID record from the last query: 0 ... numberFound()-1
//------------------------------------------------------------------------------
Navaid* NavaidLoader::navaid(const int n)
{
   const char* s = record(n);
   if (s != nullptr)
      return new Navaid(s);
   else
      return nullptr;
}

Navaid* NavaidLoader::getNavaid(const int n)
{
    const char* s = getRecord(n);
    if (s != nullptr)
       return new Navaid(s);
    else
       return nullptr;
}


//------------------------------------------------------------------------------
// queryByRange() -- find NAVAID record(s) less than mrng from the
// ref point (sorted by range)
//------------------------------------------------------------------------------
int NavaidLoader::queryByRange()
{
   return queryByType(Navaid::ANY);
}


//------------------------------------------------------------------------------
// queryByIdent() -- find NAVAID record(s) by identifier
//------------------------------------------------------------------------------
int NavaidLoader::queryByIdent(const char* id)
{
   // Search for the NAVAID record(s)
   NavaidKey key(id, nullptr);
   Key* pkey = &key;
   return Database::mQuery(&pkey, rl, nrl, il_cmp);
}


//------------------------------------------------------------------------------
// queryByKey() -- find a NAVAID record by the NAVAID record key
//------------------------------------------------------------------------------
int NavaidLoader::queryByKey(const char* navaidkey)
{
   NavaidKey key(navaidkey);
   Key* pkey = &key;
   return Database::sQuery(&pkey, rl, nrl, kl_cmp);
}


//------------------------------------------------------------------------------
// queryByFreq() -- find NAVAID record(s) by frequency/Channel
//------------------------------------------------------------------------------
int NavaidLoader::queryByFreq(const float freq)
{
   // Search for the NAVAID record(s)
   NavaidKey key(freq);
   Key* pkey = &key;
   return Database::mQuery(&pkey, reinterpret_cast<Key**>(fl), nfl, fl_cmp);
}


//------------------------------------------------------------------------------
// queryByChannel() -- find NAVAID record(s) by channel number
//------------------------------------------------------------------------------
int NavaidLoader::queryByChannel(const long chan, const char band)
{
   // Search for the NAVAID record(s)
   long chan1 = chan;
   if (band == 'Y') chan1 = -chan1;
   NavaidKey key(chan1);
   Key* pkey = &key;
   return Database::mQuery(&pkey, reinterpret_cast<Key**>(cl), ncl, cl_cmp);
}


//------------------------------------------------------------------------------
// queryByType() -- find 't' type NAVAID record(s) less than mrng from the
// ref point (sorted by range)
//------------------------------------------------------------------------------
int NavaidLoader::queryByType(const Navaid::NavaidType t)
{
   double mr2(FLT_MAX);
   if (mrng > 0.0f) mr2 = mrng*mrng;

   // compute range**2 to ref point and select all that have range less
   // than mrng**2 and type 't'
   nql = 0;
   for (int i = 0; i < nrl; i++) {
      NavaidKey* k = static_cast<NavaidKey*>(rl[i]);
      if (t == k->type || t == Navaid::ANY) {
         k->rng2 = range2(k->lat,k->lon);
         if (k->rng2 < mr2) {
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
// qsort and bsearch callbacks
//------------------------------------------------------------------------------

// kl_cmp() -- key list compare function
int NavaidLoader::kl_cmp(const void* p1, const void* p2)
{
   const NavaidKey* k1 = *(static_cast<const NavaidKey**>(const_cast<void*>(p1)));
   const NavaidKey* k2 = *(static_cast<const NavaidKey**>(const_cast<void*>(p2)));

   // compare the keys
   int result = std::strcmp(k1->key, k2->key);
   return result;
}


// il_cmp() -- identifier list compare function
int NavaidLoader::il_cmp(const void* p1, const void* p2)
{
   const NavaidKey* k1 = *(static_cast<const NavaidKey**>(const_cast<void*>(p1)));
   const NavaidKey* k2 = *(static_cast<const NavaidKey**>(const_cast<void*>(p2)));

   // compare the id's
   int result = std::strcmp(k1->ident, k2->ident);

   // if equal then check the country codes (only if k1 has one)
   if (result == 0 && k1->countryCode[0] != '\0') {

      // compare country codes
      result = std::strcmp(k1->countryCode,k2->countryCode);

   }
   return result;
}

// fl_cmp() -- frequency list compare function
int NavaidLoader::fl_cmp(const void* p1, const void* p2)
{
   const NavaidKey* k1 = *(static_cast<const NavaidKey**>(const_cast<void*>(p1)));
   const NavaidKey* k2 = *(static_cast<const NavaidKey**>(const_cast<void*>(p2)));

   int result = 0;

   // compare frequencies
   int h1 = MHZ2HHZ(k1->freq);
   int h2 = MHZ2HHZ(k2->freq);
   if (h1 < h2) result = -1;
   else if (h1 > h2) result = 1;

   return result;
}


// cl_cmp() -- channel list compare function
int NavaidLoader::cl_cmp(const void* p1, const void* p2)
{
   const NavaidKey* k1 = *(static_cast<const NavaidKey**>(const_cast<void*>(p1)));
   const NavaidKey* k2 = *(static_cast<const NavaidKey**>(const_cast<void*>(p2)));

   int result = 0;

   // compare channels
   if (k1->channel < k2->channel) result = -1;
   else if (k1->channel > k2->channel) result = 1;

   return result;
}


//------------------------------------------------------------------------------
// printing functions
//------------------------------------------------------------------------------
void NavaidLoader::printLoaded(std::ostream& sout)
{
   Navaid navaid;
   for (int i=0; i < nrl; i++) {
      navaid.setRecord( db->getRecord( rl[i]->idx ) ) ;
      navaid.printRecord(sout);
   }
}

void NavaidLoader::printFreqList(std::ostream& sout)
{
   Navaid navaid;
   for (int i=0; i < nfl; i++) {
      navaid.setRecord( db->getRecord( fl[i]->idx ) ) ;
      navaid.printRecord(sout);
   }
}

void NavaidLoader::printChannelList(std::ostream& sout)
{
   Navaid navaid;
   for (int i=0; i < ncl; i++) {
      navaid.setRecord( db->getRecord( cl[i]->idx ) ) ;
      navaid.printRecord(sout);
   }
}

void NavaidLoader::printResults(std::ostream& sout)
{
   Navaid navaid;
   for (int i=0; i < nql; i++) {
      navaid.setRecord( db->getRecord( ql[i]->idx ) ) ;
      navaid.printRecord(sout);
   }
}

//------------------------------------------------------------------------------
// NavaidLoader::NavaidKey
//------------------------------------------------------------------------------
NavaidLoader::NavaidKey::NavaidKey(const long idx, const Navaid& navaid): Key(idx)
{
   size = NAVAID_RECORD_LEN;
   lat = navaid.latitude();
   lon = navaid.longitude();

   navaid.icaoCode(icao);
   navaid.key(key);
   navaid.ident(ident);
   freq = navaid.frequency();
   channel = navaid.channel();
   if (navaid.channelType() == 'Y') channel = -channel;
   navaid.countryCode(countryCode);
   type = navaid.navaidType();
}

NavaidLoader::NavaidKey::NavaidKey(const char* id, const char* ccode) : Key(0)
{
   size = NAVAID_RECORD_LEN;
   if (id != nullptr)
      base::utStrcpy(ident,NA_IDENT_LEN+1,id);
   else
      ident[0] = '\0';

   if (ccode != nullptr)
      base::utStrcpy(countryCode,NA_CCODE_LEN+1,ccode);
   else
      countryCode[0] = '\0';

   freq = 0.0f;
   channel = 0;
   key[0] = '\0';
   type = Navaid::ANY;
}

NavaidLoader::NavaidKey::NavaidKey(const float freq1) : Key(0)
{
   size = NAVAID_RECORD_LEN;
   ident[0] = '\0';
   countryCode[0] = '\0';
   freq = freq1;
   channel = 0;
   key[0] = '\0';
   type = Navaid::ANY;
}

NavaidLoader::NavaidKey::NavaidKey(const long chan) : Key(0)
{
   size = NAVAID_RECORD_LEN;
   ident[0] = '\0';
   countryCode[0] = '\0';
   freq = 0.0f;
   channel = chan;
   key[0] = '\0';
   type = Navaid::ANY;
}


NavaidLoader::NavaidKey::NavaidKey(const char* key1) : Key(0)
{
   size = NAVAID_RECORD_LEN;
   Record::dsGetString(ident,key1,4);
   type = Navaid::NavaidType(key1[4]);
   Record::dsGetString(countryCode,&key1[5],2);
   Record::dsGetString(key,key1,NA_KEY_LEN);
   freq = 0.0f;
   channel = 0;
}


void NavaidLoader::NavaidKey::serialize(std::ostream& sout) const
{
   sout << idx << ": ";
   sout << key << ": ";

   sout << ident << "-";
   sout << type << "-";
   sout << countryCode << " ";

   sout << lat << ", ";
   sout << lon << "  ";
   sout << freq << "-";
   sout << channel;

   sout << std::endl;
}

NavaidLoader::NavaidKey::~NavaidKey()
{
}

} // End dafif namespace
} // End oe namespace
