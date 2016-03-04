//------------------------------------------------------------------------------
// Class: EarthModel
//------------------------------------------------------------------------------
#ifndef __oe_base_EarthModel_H__
#define __oe_base_EarthModel_H__

#include "openeaagles/base/Object.h"

namespace oe {
namespace basic {
   class Distance;
   class Number;

//------------------------------------------------------------------------------
// Class: EarthModel
// Description: Contains the major axis, minor axis and flattening terms used to
//              describe an ellipsoidal or a spherical earth model. (See Nav.h)
//
// Factory name: EarthModel
// Slots:
//     a    <Distance>  ! Semi major axis
//     a    <Number>    ! -- in meters
//
//     b    <Distance>  ! Semi minor axis
//     b    <Number>    ! -- in meters
//
//     f    <Number>    ! Flattening
//
//------------------------------------------------------------------------------
class EarthModel : public Object
{
    DECLARE_SUBCLASS(EarthModel,Object)

public:
   // Some pre-defined earth models             -- getEarthModel() by name list
   static const EarthModel airy;                // "airy"
   static const EarthModel australianNational;  // "australianNational"
   static const EarthModel bessel1841;          // "bessel1841"
   static const EarthModel clark1866;           // "clark1866"
   static const EarthModel clark1880;           // "clark1880"
   static const EarthModel everest;             // "everest"
   static const EarthModel fischer1960;         // "fischer1960"
   static const EarthModel fischer1968;         // "fischer1968"
   static const EarthModel grs1967;             // "grs1967"
   static const EarthModel grs1980;             // "grs1980"
   static const EarthModel helmert1906;         // "helmert1906"
   static const EarthModel hough;               // "hough"
   static const EarthModel international;       // "international"
   static const EarthModel kravosky;            // "kravosky"
   static const EarthModel modAiry;             // "modairy"
   static const EarthModel modEverest;          // "modeverest"
   static const EarthModel modFischer;          // "modfischer"
   static const EarthModel southAmerican1969;   // "southAmerican1969"
   static const EarthModel wgs60;               // "wgs60"
   static const EarthModel wgs66;               // "wgs66"
   static const EarthModel wgs72;               // "wgs72"
   static const EarthModel wgs84;               // "wgs84"

public:
   EarthModel();   // Defaults to WGS-84
   EarthModel(const double a0, const double f0);

   double getA() const;     // meters
   double getB() const;     // meters
   double getF() const;
   double getE2() const;

   bool setA(const double meters);
   bool setB(const double meters);
   bool setF(const double);

   // Get an earth model by name
   // (see the above list of pre-defined earth models)
   static const EarthModel* getEarthModel(const char* const name);

protected:
   bool setSlotA(const Distance* const msg);
   bool setSlotA(const Number* const msg);
   bool setSlotB(const Distance* const msg);
   bool setSlotB(const Number* const msg);
   bool setSlotF(const Number* const msg);

private:
   void initData(const double a0, const double f0);

   double a;   // semi major axis (meters)
   double b;   // semi minor axis (meters)
   double f;   // flattening
   double e2;  // Eccentricity Squared
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------
inline double EarthModel::getA() const  { return a; }
inline double EarthModel::getB() const  { return b; }
inline double EarthModel::getF() const  { return f; }
inline double EarthModel::getE2() const { return e2; }

} // End basic namespace
} // End oe namespace

#endif
