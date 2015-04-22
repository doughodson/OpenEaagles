/****************************************************************************
 * geoTypes.h
 *
 *
 *
 * Description: Define some basic types for use with geographic data files
 *
 * Notes:
 *
 * Change History:
 *
 * Date          Name                  Description
 * -----------   -------------------   ----------------------------------------
 *
 *
 *****************************************************************************/

#ifndef GEOTYPES_H
#define GEOTYPES_H

#include <memory.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum { RADIANS=0, FEET, METERS, ARC_SECONDS, DEGREES } GEO_UNITS;

typedef unsigned char        uint_1;
typedef unsigned short int   uint_2;
typedef unsigned int         uint_4;
typedef char                 int_1;
typedef short int            int_2;
typedef int                  int_4;
typedef float                float_4;
typedef double               float_8;

#if defined(WIN32)
  #include <Wtypes.h>
    typedef ULONGLONG        uin_8;
    typedef LONGLONG         int_8;
#else
  #ifdef IRIX
  #include <sgidefs.h>
  #endif
  typedef long long          int_8  ;
  typedef unsigned long long uint_8 ;
#endif

typedef float_8 GEO_FLOAT;


typedef struct
{
  int_2   x;
  int_2   y;
  int_2   z;
} COORD_INT2_3D;

typedef struct
{
  float_4 x;
  float_4 y;
} COORDINATE_4_2D;

typedef struct
{
  float_8 x;
  float_8 y;
} COORDINATE_8_2D;

typedef struct
{
  float_4 x;
  float_4 y;
  float_4 z;
} COORDINATE_4_3D;

typedef struct
{
  GEO_FLOAT lon;
  GEO_FLOAT lat;
  GEO_FLOAT alt;
} GEO_COORD_3D;

typedef struct
{
  GEO_FLOAT lon;
  GEO_FLOAT lat;
} GEO_COORD_2D;

typedef struct
{
  GEO_FLOAT easting;
  GEO_FLOAT northing;
  GEO_FLOAT elevation;
} MAP_COORD_3D;

typedef struct
{
  GEO_FLOAT easting;
  GEO_FLOAT northing;
} MAP_COORD_2D;

typedef struct
{
  GEO_FLOAT x;
  GEO_FLOAT y;
  GEO_FLOAT z;
} VECTOR_3D;

//typedef char DATE[20];

typedef struct
{
  unsigned char reservedBits : 2;
  unsigned char extRowIdBits : 2;
  unsigned char tileIdBits   : 2;
  unsigned char rowIdBits    : 2;
} TYPEBYTE;

typedef union
{
  char  eight;
  int_2 sixteen;
  int_4 thirtytwo;
} TRIPLET;


typedef struct
{
  TYPEBYTE typeByte;
  TRIPLET  rowId;
  TRIPLET  tileId;
  TRIPLET  extRowId;
} TRIPLET_ID;

typedef struct
{
  float_8 x;
  float_8 y;
} POINT2D;

typedef struct
{
  float_8 x;
  float_8 y;
  float_8 z;
} POINT3D, VECTOR3D;

typedef struct
{
  // A, B, and C are indexes into a POINT*D array
  int_4 A;
  int_4 B;
  int_4 C;
} TRIANGLE;

typedef struct
{
  long p1, p2;
} EDGE;


#ifndef isnan
  #define isnan(x) ((x) != (x))
#endif

extern const int_2 NULL_S;
extern const int_4 NULL_I;
extern const float_4 NULL_F;
extern const float_8 NULL_R;

#define MAXVERTICES  500000
#define MAXTRIANGLES 100000
#define MAXEDGES     100000

#ifdef  __cplusplus
}
#endif


#endif


