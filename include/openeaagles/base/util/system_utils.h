
#ifndef __oe_base_util_system_utils_H__
#define __oe_base_util_system_utils_H__

namespace oe {
namespace base {

// Sleep for 'msec' milliseconds
void lcSleep(const unsigned int msec);

// Computer time (seconds)
double getComputerTime();

// UTC time of day (seconds since midnight) and optional date (day/month/year)
double getTimeUtc(unsigned int* const day=0, unsigned int* const month=0, unsigned int* const year=0);

// Get UTC time since midnight (00:00:00), January 1, 1970
double getTime();

// Get UTC time since midnight (00:00:00), January 1, 1970
void getTime(
      unsigned long* const sec,  // (OUT) whole seconds
      unsigned long* const uSec  // (OUT) microseconds seconds
   );

// Convert seconds since midnight (00:00:00), January 1, 1970 to year:month:day:hour:minute:second
bool convertSec2Ymdhms(
      const unsigned long seconds,  // (IN) whole seconds since midnight (00:00:00), January 1, 1970
      unsigned int* const year,     // (OUT) year YYYY
      unsigned int* const month,    // (OUT) month of the year [ 1 .. 12 ]
      unsigned int* const day,      // (OUT) day of the month   [ 1 .. 31 ]
      unsigned int* const hour,     // (OUT) hours since midnight  [ 0 .. 23 ]
      unsigned int* const min,      // (OUT) minutes after the hour [ 0 .. 59 ]
      unsigned int* const sec       // (OUT) seconds after the minute [ 0 .. 59 ]
   );

// Convert year:month:day:hour:minute:second to seconds since midnight (00:00:00), January 1, 1970
bool convertYmdhms2Sec(
      const unsigned int year,      // (IN) year YYYY
      const unsigned int month,     // (IN) month of the year [ 1 .. 12 ]
      const unsigned int day,       // (IN) day of the month   [ 1 .. 31 ]
      const unsigned int hour,      // (IN) hours since midnight  [ 0 .. 23 ]
      const unsigned int min,       // (IN) minutes after the hour [ 0 .. 59 ]
      const unsigned int sec,       // (IN) seconds after the minute [ 0 .. 59 ]
      unsigned long* const seconds  // (OUT) whole seconds since midnight (00:00:00), January 1, 1970
   );

// Convert seconds since midnight (00:00:00), January 1, 1970 to NTP time stamp
bool convertSec2Ntp(
      const unsigned long seconds,  // (IN) whole seconds since midnight (00:00:00), January 1, 1970
      const unsigned long uSec,     // (IN) microseconds seconds
      unsigned long* const ntpSec,  // (OUT) NTP seconds
      unsigned long* const ntpSecF  // (OUT) NTP fraction of seconds
   );

// Convert NTP time stamp to seconds since midnight (00:00:00), January 1, 1970
bool convertNtp2Sec(
      const unsigned long ntpSec,   // (IN) NTP seconds
      const unsigned long ntpSecF,  // (IN) NTP fraction of seconds
      unsigned long* const seconds, // (OUT) whole seconds since midnight (00:00:00), January 1, 1970
      unsigned long* const uSec     // (OUT) microseconds seconds
   );

// doesFileExist -- returns true if file exists
bool doesFileExist(const char* const fullname);

}
}

#endif
