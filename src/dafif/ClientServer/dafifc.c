
#include "eaagles/dafifc.h"
#include <string.h>
#include <stdlib.h>


/* -------------------------------------------------------------------- */
/* dsFreq2chan() -- freq (MHz) to TACAN channel                         */
/* -------------------------------------------------------------------- */
int dsFreq2chan(const float f)
{
   long freq = (long)(f*10000.0f);
   int c = 0;

   if (freq < 1123000)
      c = (freq - 1063000) / 1000;
   else if (freq >= 1123000 && freq < 1180000)
      c = (freq - 1053000) / 1000;
   else if (freq >= 1133000 && freq < 1343000)
      c = (freq - 1333000) / 1000 + 60;
   else if (freq >= 1344000)
      c = (freq - 1344000) / 1000 + 1;

   return c;
}

/* -------------------------------------------------------------------- */
/* dsChan2freq() -- TACAN channel to freq (MHz)                         */
/* -------------------------------------------------------------------- */
float dsChan2freq(const int chan, const char type)
{
   long freq = 0;

   if (chan >= 1 && chan <= 16)
      freq = (chan - 1) * 1000 + 1344000;
   else if (chan >= 17 && chan <= 59)
      freq = (chan + 1063) * 1000;
   else if (chan >= 60 && chan <= 69)
      freq = (chan - 60) * 1000 + 1333000;
   else if (chan >= 70 && chan <= 126)
      freq = (chan + 1053) * 1000;

   if (type == 'Y')
      freq += 500;

   return ((float)freq)/10000.0f;
}


/* -------------------------------------------------------------------- */
/* dsAtofn() -- convert n-characters to float                           */
/* -------------------------------------------------------------------- */
double dsAtofn(const char* s, const int n)
{
   char buf[256];
   if (s == 0) return 0.0f;
   if (*s == 'U') return 0.0f;
   strncpy(buf, s, n);
   buf[n] = '\0';
   return atof(buf);
}


/* -------------------------------------------------------------------- */
/* dsAtoln() -- convert n-characters to long                            */
/* -------------------------------------------------------------------- */
long dsAtoln(const char* s, const int n)
{
   char buf[256];
   if (s == 0) return 0;
   if (*s == 'U') return 0;
   strncpy(buf, s, n);
   buf[n] = '\0';
   return atol(buf);
}

/* -------------------------------------------------------------------- */
/* dsGetString() -- like strncpy() with 0 char* checking and the     */
/* spaces removed from the tail of the string.                          */
/* -------------------------------------------------------------------- */
void dsGetString(char* d, const char *s, const int n)
{
   const char* p = s;
   char*       q = d;
   int	i;

   /* Make sure we have a place to copy it */
   if (d == 0) return;

   /* Make sure we have something to copy */
   if (s == 0 || n <= 0) {
      d[0] = '\0';
      return;
   }
   
   /* copy the string */
   for (i = 0; i < n; i++) { *q++ = *p++; }

   /* terminate the string */
   *q-- = '\0';

   /* strip spaces of the tail */
   for (; q >= d && *q == ' '; q--) { *q = '\0'; }
}


/* -------------------------------------------------------------------- */
/* dsIsString() -- true if string p matches refStr                      */
/* -------------------------------------------------------------------- */
int dsIsString(const char* p, const char* refStr)
{
   int len;
   if (p == 0 || refStr == 0) return 0;
   len = (int)strlen(refStr);
   return strncmp(p,refStr,len) == 0;
}

/* -------------------------------------------------------------------- */
/* dsLatitude() -- convert string to latitude                           */
/* -------------------------------------------------------------------- */
double dsLatitude(const char* p)
{
   double lat = 0.0f;
   if (p != 0) {
      char   h = *p;
      double d = dsAtofn(p+1, 2);
      double m = dsAtofn(p+3, 2);
      double s = dsAtofn(p+5, 2);
      double f = dsAtofn(p+7, 2);
      double v = d + m / 60.0 + (s + f / 100.0) / 3600.0;
      lat = (h == 'N' ? v : -v);
   }
   return lat;
}

/* -------------------------------------------------------------------- */
/* dsLongitude() --  convert string to longitude                        */
/* -------------------------------------------------------------------- */
double dsLongitude(const char* p)
{
   double lon = 0.0;
   if (p != 0) {
      char   h = *p;
      double d = dsAtofn(p+1, 3);
      double m = dsAtofn(p+4, 2);
      double s = dsAtofn(p+6, 2);
      double f = dsAtofn(p+8, 2);
      double v = d + m / 60.0 + (s + f / 100.0) / 3600.0;
      lon = (h == 'E' ? v : -v);
   }
   return lon;
}

/* -------------------------------------------------------------------- */
/* dsMagVariance() --  convert string to magnetic variation             */
/* -------------------------------------------------------------------- */
float dsMagVariance(const char* p)
{
   double mv = 0.0;
   if (p != 0) {
      double d = dsAtofn(p+1, 3);
      double m = dsAtofn(p+4, 2);
      double f = dsAtofn(p+6, 1);
      double v = d + (m + f / 10.0f) / 60.0f;
      mv = ( (*p == 'E') ? v : -v );
   }
   return (float) mv;
}

/* -------------------------------------------------------------------- */
/* dsSlaveVariance() -- convert string to slave magnetic variation      */
/* -------------------------------------------------------------------- */
float dsSlaveVariance(const char* p)
{
   double sv = 0.0;
   if (p != 0) {
      if (*p != ' ') {
         double v = dsAtofn(p+1, 3);
         sv = ( (*p == 'E') ? v : -v );
      }
   }
   return (float) sv;
}

/* -------------------------------------------------------------------- */
/* dsMagHeading() -- magnetic heading                                   */
/* -------------------------------------------------------------------- */
float dsMagHeading(const char* p)
{
   char ct;
   double mh = 0.0;
   if (p != 0) {
      mh = dsAtofn(p,3);	/* units */
      ct = *(p+3);		/* tenths */
      if (ct != '/') mh += (float)(ct - '0')/10.0f;
   }
   return (float) mh;
}

/* -------------------------------------------------------------------- */
/* dsElevation() -- format#1: convert string to elevation (to nearest foot) */
/* -------------------------------------------------------------------- */
float dsElevation(const char* p)
{
   if (p == 0) return 0.0f;
   if (*p == '\0') return 0.0f;

   if (*p == '-') return (float) -dsAtofn(p+1, 4);
   else return (float) dsAtofn(p, 5);
}

/* -------------------------------------------------------------------- */
/* dsElevation1() -- format#2: convert string to elevation (to tenths   */
/* of feet)                                                             */
/* -------------------------------------------------------------------- */
float dsElevation1(const char* p)
{
   double e, et;
   char ct;

   if (p == 0) return 0.0f;
   if (*p == '\0') return 0.0f;

   e = 0.0;
   if (*p == '-') e = dsAtofn(p+1, 4);
   else e = dsAtofn(p, 5);

   et = 0.0;
   ct = *(p+5);
   if (ct != ' ') et = ct - '0';

   if (*p == '-')
      return (float) -(e + et/10.0);
   else 
      return (float)  (e + et/10.0);
}


/* -------------------------------------------------------------------- */
/* dsFrequency() -- convert string to frequency  (MHz)                  */
/* -------------------------------------------------------------------- */
float dsFrequency(const char* p)
{
   long freq = 0;

   if (*p != 'U') {
      int i;
      for (i = 0; i < 7 && p[i] == ' '; i++)
      if (i < 7) {
         freq = dsAtoln(p, 7);
         if (*(p+7) == 'M') freq *= 1000;
      }
   }

   return ((float)freq)/1000000.0f;
}

