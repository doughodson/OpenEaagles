//------------------------------------------------------------------------------
// Classes: AircraftIrSignature
//------------------------------------------------------------------------------
#ifndef __oe_simulation_AircraftIrSignature_H__
#define __oe_simulation_AircraftIrSignature_H__

#include "openeaagles/simulation/IrSignature.h"

namespace oe {
   namespace basic {
      class Number;
      class List;
      class Table1;
      class Table2;
      class Table3;
      class Table4;
      class Table5;
   }

namespace simulation {
   class AirVehicle;
   class IrQueryMsg;
   class IrShape;

//------------------------------------------------------------------------------
// Class: AircraftIrSignature
//
// Descriptions: Base class for an aircraft IR signature
//
//
// Factory name: AircraftIrSignature
// Slots:
//    airframeSignatureTable          <Table4>      !
//    airframeWavebandFactorTable     <Table2>      !
//    plumeSignatureTable             <Table5>      !
//    plumeWavebandFactorTable        <Table2>      !
//    hotPartsSignatureTable          <Table5>      !
//    hotPartsWavebandFactorTable     <Table2>      !
//
// Public member functions:
//      LCreal getIrSignature(IrQueryMsg* msg)
//          Computes the IR signature for the emission.
//------------------------------------------------------------------------------
class AircraftIrSignature : public IrSignature
{
    DECLARE_SUBCLASS(AircraftIrSignature, IrSignature)

public:
   AircraftIrSignature();

   bool getIrSignature(IrQueryMsg* const em) override;

   // slot operations
   virtual bool setSlotAirframeSignatureTable(const basic::Table4* const tbl);
   virtual bool setSlotAirframeWavebandFactorTable(const basic::Table2* const tbl);
   virtual bool setSlotPlumeSignatureTable(const basic::Table5* const tbl);
   virtual bool setSlotPlumeWavebandFactorTable(const basic::Table2* const tbl);
   virtual bool setSlotHotPartsSignatureTable(const basic::Table5* const tbl);
   virtual bool setSlotHotPartsWavebandFactorTable(const basic::Table2* const tbl);

protected:
   virtual LCreal* getHeatSignature(IrQueryMsg* msg);

   virtual LCreal getAirframeSignature(LCreal velocity, LCreal altitude, LCreal azimuth, LCreal elevation);
   virtual LCreal getAirframeWavebandFactor(LCreal midpoint, LCreal width);

   virtual LCreal getPlumeSignature(LCreal pla, LCreal velocity, LCreal altitude, LCreal azimuth, LCreal elevation);
   virtual LCreal getPlumeWavebandFactor(LCreal midpoint, LCreal width);

   virtual LCreal getHotPartsSignature(LCreal pla, LCreal velocity, LCreal altitude, LCreal azimuth, LCreal elevation);
   virtual LCreal getHotPartsWavebandFactor(LCreal midpoint, LCreal width);

   virtual LCreal getCalculatedAirframeHeatSignature(const IrQueryMsg* const msg);

   // FAB - added to parallel plume and hotparts calculations
   virtual void getAirframeSignatures(const IrQueryMsg* const msg, const LCreal lowerBound, const LCreal upperBound);

   virtual LCreal getPlumeRadiation(const IrQueryMsg* const msg);

   virtual void getPlumeSignatures(const IrQueryMsg* const msg,
                                   const LCreal lowerBound,
                                   const LCreal upperBound);

   virtual LCreal getHotPartsRadiation(const IrQueryMsg* const msg);

   virtual void getHotPartsSignatures(const IrQueryMsg* const msg,
                                      const LCreal lowerBound,
                                      const LCreal upperBound);

   virtual LCreal getPLA(const AirVehicle* const airModel);

private:
   void initData();

   const basic::Table4* airframeSignatureTable; // mapping of
            // signature  x is the velocity (in mach #) and y is altitude (in sim prevailing units --
            // meters, I believe), z is azimuth in radians, za is
            // elevation in radians. Data is the heat signature in watts per steradian.

   const basic::Table2*  airframeWavebandFactorTable;
            // airframe wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base airframe signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0  .

   const basic::Table5* plumeSignatureTable;
            // mapping of signature in watts/steradian to
            // x- pla % from 0 - 100.
            // y - velocity (mach #)
            // z - altitude (meters)
            // azimuth (radians)
            // elevation (radians)

   const basic::Table2*  plumeWavebandFactorTable;
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0

   const basic::Table5* hotPartsSignatureTable;
            // mapping of signature in watts/steradian to
            // x- pla % from 0 - 100.
            // y - velocity (knots)
            // z - altitude (meters)
            // azimuth (radians)
            // elevation (radians)

   const basic::Table2*  hotPartsWavebandFactorTable;
            // plume wavebands
            // x - waveband center
            // y - waveband width
            // data - factor. We multiply the base plume signature by this
            // factor to get the plume energy in this particular waveband.
            // the different factors should all sum to 1.0  .

   LCreal* airframeSig;       // 2 dimensions i = bin, j = lower wavelength, upper wavelength, signature
   LCreal* plumeSigs;         // 2 dimensions i = bin, j = lower wavelength, upper wavelength, signature
   LCreal* hotPartsSigs;      // 2 dimensions i = bin, j = lower wavelength, upper wavelength, signature
};


} // End simulation namespace
} // End oe namespace

#endif
