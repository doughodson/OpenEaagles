
#include <cstring>
#include <fstream>

#include "openeaagles/base/util/system.h"

namespace oe {
namespace base {

//------------
// UTC time of day (seconds since midnight) and optional date (day/month/year)
//------------
double getTimeUtc(unsigned int* const day, unsigned int* const month, unsigned int* const year)
{
   unsigned long seconds = 0;
   unsigned long uSec = 0;
   getTime(&seconds, &uSec);

   unsigned int hour = 0;
   unsigned int min = 0;
   unsigned int sec = 0;
   convertSec2Ymdhms(seconds, year, month, day, &hour, &min, &sec);

   // Return time since midnight (sec)
   return ( hour * 3600.0 + min * 60.0 + sec + static_cast<double>(uSec) / 1000000.0 );
}

//------------
// Get UTC time since midnight (00:00:00), January 1, 1970
//------------
double getTime()
{
   unsigned long sec(0), uSec(0);
   getTime(&sec, &uSec);
   return static_cast<double>(sec) + static_cast<double>(uSec)/1000000.0;
}

// Convert seconds since midnight (00:00:00), January 1, 1970 to NTP time stamp
bool convertSec2Ntp(
      const unsigned long seconds,  // (IN) whole seconds since midnight (00:00:00), January 1, 1970
      const unsigned long uSec,     // (IN) microseconds seconds
      unsigned long* const ntpSec,  // (OUT) NTP seconds
      unsigned long* const ntpSecF  // (OUT) NTP fraction of seconds
   )
{
   static const double P32 = 4294967296.0; // 2^32

   // Adjust by 2208988800 seconds (1900 to 1970)
   if (ntpSec != nullptr) {
      static const unsigned long ADJ = 0x83AA7E80;   // 2208988800 seconds
      *ntpSec = seconds + ADJ;
   }

   // Microseconds to fractional second
   if (ntpSecF != nullptr) {
      const double fsec = static_cast<double>(uSec) / 1000000.0;
      *ntpSecF = static_cast<unsigned long>((fsec * P32) + 0.5);
   }
   return true;
}

// Convert NTP time stamp to seconds since midnight (00:00:00), January 1, 1970
bool convertNtp2Sec(
      const unsigned long ntpSec,   // (IN) NTP seconds
      const unsigned long ntpSecF,  // (IN) NTP fraction of seconds
      unsigned long* const seconds, // (OUT) whole seconds since midnight (00:00:00), January 1, 1970
      unsigned long* const uSec     // (OUT) microseconds seconds
   )
{
   static const double P32 = 4294967296.0; // 2^32

   // Adjust by 2208988800L seconds (1900 to 1970)
   if (seconds != nullptr) {
      static const unsigned long ADJ = 0x83AA7E80;   // 2208988800 seconds
      *seconds = ntpSec - ADJ;
   }

   // fractional second to microseconds
   if (uSec != nullptr) {
      const double fsec = static_cast<double>(ntpSecF) / P32;
      *uSec = static_cast<unsigned long>((fsec * 1000000.0) + 0.5);
   }
   return true;
}

//------------
// doesFileExist() -- Returns true if the file already exists
//------------
bool doesFileExist(const char* const fullname)
{
   bool result = false;
   if (fullname != nullptr) {
      // Test open the file
      std::ifstream t;
      t.open(fullname);
      if (t.is_open()) {
         // It's open, it exists!
         result = true;
         t.close();
      }
   }
   return result;
}

}
}
