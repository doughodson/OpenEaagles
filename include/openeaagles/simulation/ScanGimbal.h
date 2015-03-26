//------------------------------------------------------------------------------
// Class: ScanGimbal
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_ScanGimbal_H__
#define __Eaagles_Simulation_ScanGimbal_H__

#include "openeaagles/simulation/Gimbal.h"

namespace Eaagles {

namespace Basic { class Integer; }

namespace Simulation {

//------------------------------------------------------------------------------
// Class: ScanGimbal
//
// Description: Simple Gimbal model: provides rate & position servo
//              control, as well as 1, 2 and 4 bar scans
//
// Factory name: ScanGimbal
// Slots:
//    scanMode             <String>       ! Sets the type of scan we desire ...(default: MANUAL_SCAN)
//                                        ! ... { manual, horizontal, vertical, conical, circular, pseudorandom }
//
//    leftToRightScan      <Boolean>      ! True to scan from left-to-right (else right-to-left) (Default: true)
//                                        ! ... and, true for from top-to-bottom (else from bottom-to-up)
//
//    scanWidth            <Number>       ! Width of the scan (radians) (default: 0.0)
//
//    searchVolume         <List>         ! Size of the search volume [ width  height ] (radians); 
//                                        ! (default scanHeight: 0)
//                                        ! ... and sets the 'scanWidth'
//                                        ! ... and sets the 'scanMode'  to "horizontal"
//
//    reference            <List>         ! Reference angles; i.e., center of the search volume [ az el ] (radians) (default: 0, 0)
//
//    barSpacing           <Number>       ! Scan bar width spacing (radians) (default: 0.0)
//
//    numBars              <Integer>      ! Sets the number of bars (default: 1)
//
//    revolutionsPerSec    <Number>       ! Revolutions per second for conical and circular scans (hz) (default: 5)
//
//    scanRadius           <Number>       ! Radius of the circle we are using for conical scans (radians or Basic::Angle} (spiral scan too)
//                                        ! (default: 2.0 * Basic::Angle::D2RCC, 2 degree radius)
//
//    pseudoRandomPattern  <PairStream>   ! Pseudo Random pattern vertices (2D - az and el) (default: 0, 0)
//
//    maxRevolutions       <Number>       ! Spiral Scan - Maximum number of revolutions (default: 1.0)
//
//
// Events:
//    SCAN_START        (Basic::Integer)     Start of scan event
//    SCAN_END          (Basic::Integer)     End of scan event
//
//
// Public member functions:
//
//      getRefPosition(osg::Vec2d& a)
//          Returns the current reference position vector (radians)
//          (i.e., the center of the scan volume).
//
//      double getRefAzimuth()
//      double getRefElevation()
//          Return the current reference positions (radians)
//
//      ScanMode getScanMode()
//          Returns the current scan mode
//
//       bool resetScan()
//          Resets the current scan mode.
//          Return 'true' if successful
//
//       getScanVolume(double* width, double* height)
//       double getScanWidth()
//       double getScanHeight()
//          Returns scan volume width & height in radians for
//          horizontal and vertical scans
//
//       getScanVolumeD(double* width, double* height)
//       double getScanWidthD()
//       double getScanHeightD()
//          Returns scan volume width & height in degrees for
//          horizontal and vertical scans
//
//       int getNumBars()
//          Returns the number of scan bars (horizontal, vertical)
//
//       int getBarNumber()
//          Returns the current 'bar' number (horizontal, vertical)
//
//       bool setScanWidth(double newWidth)
//          Sets the scan width (radians) (horizontal, vertical)
//
//       bool setNumBars(double newNumBars)
//          Sets the number of scan bars (horizontal, vertical)
//
//       bool setBarSpacing(double newSpacing)
//          Sets the spacing (radians) between scan bars (horizontal, vertical)
//
//       bool setLeftToRightScan(const bool newLeftToRightScan)
//          Sets the left to right scan flag (else right to left scanning)
//
//      setRefPosition(osg::Vec2d& nla)
//      bool setRefPosition(double az, double el)
//      bool setRefAzimuth(double az)
//      bool setRefElevation(double el)
//          Sets the reference position (center of scan/search volume)
//
//      bool setRevPerSec(const double newRevPerSec)
//          Sets the number of revolutions per second (conical scan)
//
//      bool setScanRadius(const double newScanRadius)
//          Sets the radius of the conical scan (radians)
//
//      bool setSearchVolume(double width, double height)
//      bool setSearchVolume(double width, double height, int reqBars)
//          Sets a search volume and starts a horizontal bar scan.
//          The scan width, height and number of bars are set by this function,
//          so don't set the scan width, height and bar functions.
//          If 'reqBars' is zero or not provided, then the number of bars is
//          computed based on default parameters.
//
// Notes:
//    1) Options for slot-initializing a scan gimbal in bar scan:
//       either set searchVolume slot and accept defaults it sets
//       or, for more control, set barSpacing, numBars, and scanWidth
//       slots - scan height will be calculated for you.  if you mix
//       the two, you're likely to get unexpected results.
//
//------------------------------------------------------------------------------
class ScanGimbal : public Gimbal  
{
    DECLARE_SUBCLASS(ScanGimbal,Gimbal)

public:
    enum { MANUAL_SCAN, HORIZONTAL_BAR_SCAN, VERTICAL_BAR_SCAN, 
           CONICAL_SCAN, CIRCULAR_SCAN, PSEUDO_RANDOM_SCAN, 
           SPIRAL_SCAN, USER_MODES };
    enum Side { BEGINNING = 0, ENDING = 1 };

public:
    ScanGimbal();

    const osg::Vec2d& getRefPosition() const    { return refAngle; }             // Returns the current reference position vector (rad)
    double getRefAzimuth() const                { return refAngle[AZ_IDX]; }     // Return the current reference azimuth (rad)
    double getRefElevation() const              { return refAngle[ELEV_IDX]; }   // Return the current reference elevation (rad)

    unsigned int getScanMode() const            { return scanMode; }             // Returns the scan mode
    virtual bool setScanMode(const unsigned int m, const bool resetRequired = true); // Sets the scan mode (optional reset flag)
    virtual bool resetScan();                                                    // Reset the scan pattern

    void getScanVolume(double* width, double* height) const;                     // Returns the scan volume in radians (horizontal, vertical)
    void getScanVolumeD(double* width, double* height) const;                    // Returns the scan volume in degrees (horizontal, vertical)

    double getScanWidth() const                 { return scanWidth; }            // Returns the scan width in radians (horizontal, vertical)
    double getScanWidthD() const;                                                // Returns the scan width in degrees (horizontal, vertical)

    double getScanHeight() const                { return scanHeight; }           // Returns the scan height in radians (horizontal, vertical)
    double getScanHeightD() const;                                               // Returns the scan height in degrees (horizontal, vertical)

    unsigned int getNumBars() const             { return numBars; }              // Returns the number of scan bars (horizontal, vertical)
    unsigned int getBarNumber() const           { return barNum; }               // Returns the current 'bar' number (horizontal, vertical)

    double getBarSpacing() const                { return barSpacing; }           // Returns the spacing between bars (radians)

    double getScanRadius() const                { return scanRadius; }           // Returns the radius of the conical scan (radians)
    double getScanRadiusD() const;                                               // Returns the radius of the conical scan (degrees)

    double getRevPerSec() const                 { return revPerSec; }            // Returns revolutions per second
    double getMaxNumRevs() const                { return maxNumRevs; }           // Returns maximum number of revolutions (Spiral Scan)

    virtual bool setScanWidth(const double newWidth);                            // Sets the scan width (radians) (horizontal, vertical)
    virtual bool setNumBars(const double newNumBars);                            // Sets the number of scan bars (horizontal, vertical)
    virtual bool setBarSpacing(const double newSpacing);                         // Sets the spacing (radians) between scan bars (horizontal, vertical)
    virtual bool setLeftToRightScan(const bool newLeftToRightScan);              // Sets the left to right scan flag (else right to left scanning)

    virtual void setRefPosition(const osg::Vec2d& nla);                          // Sets the reference position vector (rad)
    virtual bool setRefPosition(const double refAz, const double refEl);         // Sets the reference position vector (rad)
    virtual bool setRefAzimuth(const double az);                                 // Sets the reference azimuth (rad)
    virtual bool setRefElevation(const double el);                               // Sets the reference elevation (rad)

    virtual bool setRevPerSec(const double newRevPerSec);                        // Sets the number of revolutions per second (conical scan)
    virtual bool setScanRadius(const double newScanRadius);                      // Sets the radius of the conical scan (radians)

    virtual bool setMaxRevs(const double newMaxRevs);                            // Sets the maximum number of revolutions (spiral scan)

    // Sets a search volume for horizontal scan patterns (radians) -- 
    virtual bool setSearchVolume(const double width, const double height, const int reqBars = 0);

    // Event handler(s)
    virtual bool onStartScanEvent(Basic::Integer* const bar);
    virtual bool onEndScanEvent(Basic::Integer* const bar);

    // Slot functions
    virtual bool setSlotScanMode(Basic::String* const newMode);
    virtual bool setSlotLeftToRightScan(const Basic::Number* const newLeftToRightScan);
    virtual bool setSlotScanWidth(const Basic::Number* const newWidth);
    virtual bool setSlotSearchVolume(Basic::List* const numList);
    virtual bool setSlotRefPosition(const Basic::List* const numList);
    virtual bool setSlotBarSpacing(const Basic::Number* const newSpacing);
    virtual bool setSlotNumBars(const Basic::Number* const newNumBars);
    virtual bool setSlotRevPerSec(const Basic::Number* const newRevPerSec);
    virtual bool setSlotScanRadius(const Basic::Number* const newScanRadius);
    virtual bool setSlotPRVertices(const Basic::PairStream* const prObj);
    virtual bool setSlotMaxRevs(const Basic::Number* const newMaxRevs);
   
    // Component Interface
    bool event(const int event, Basic::Object* const obj = 0) override;
    void reset() override;

protected:
   virtual void scanController(const double dt);
   virtual void manualScanController(const double dt);
   virtual void barScanController(const double dt);
   virtual void conicalScanController(const double dt);
   virtual void circularScanController(const double dt);
   virtual void pseudoRandomScanController(const double dt);
   virtual void spiralScanController(const double dt);
   virtual void userModesScanController(const double dt);

   virtual void computeNewBarPos(const int bar, const Side side);
   virtual void nextBar();

   osg::Vec2d& getScanPos()                           { return scanPos; }
   const osg::Vec2d& getScanPos() const               { return scanPos; }
   bool setScanPos(const double x, const double y)    { scanPos.set(x,y); return true; }

   unsigned int getScanState() const                  { return scanState; }
   bool setScanState(const unsigned int s)            { scanState = s; return true; }

   double getNumRevs() const                          { return numRevs; }
   bool setNumRevs(const double r)                    { numRevs = r; return true; }

   double getConAngle() const                         { return conAngle; }
   bool setConAngle(const double a)                   { conAngle = a; return true; }

   bool isOddNumberOfBars() const                     { return oddNumberOfBars; }
   bool isLeftToRightScan() const                     { return leftToRightScan; }

   bool isReverseScan() const                         { return reverseScan; }
   bool setReverseScan(const bool f)                  { reverseScan = f; return true; }

   bool setBarNumber(const unsigned int n)            { if (n <= numBars) { barNum = n; return true; } else return false; } 

   // System Interface -- Callbacks by phase
   void dynamics(const LCreal dt) override;    // Phase 0

private:
    osg::Vec2d  scanPos;         // Position in scan pattern     (rad)
    osg::Vec2d* prScanVertices;  // Pseudo random scan pattern positions
    unsigned int nprv;           // Number of pseudo random vertices
    unsigned int cprv;           // Current pseudo random vertice number
    unsigned int scanMode;       // Gimbal scan mode
    double     scanWidth;        // Width of scan volume         (rad)
    double     scanHeight;       // Height of scan volume        (rad)
    unsigned int scanState;      // Scan state machine
    osg::Vec2d  refAngle;        // Gimbal reference angles     (rad)
    osg::Vec2d  lastRefAngle;    // Last gimbal reference angle (rad)

    unsigned int numBars;        // number of bars in our scan
    double     barSpacing;       // width between bars (if applicable) (rad)
    bool       oddNumberOfBars;  // flag used to reverse sequence of bar scans
    bool       leftToRightScan;  // flag to tell us if we scan from right to left or left to right
    bool       reverseScan;      // Scanning the reverse direction
    unsigned int barNum;         // Bar number that we are on
    double     conAngle;         // Conical scan angle (degrees)
    double     revPerSec;        // Revolutions per second (hz)
    double     scanRadius;       // Radius of the conical or circular scan (rad)
    bool       oneRev;           // Tells us we have done one circular or conical revolution
    double     myLastAngle;      // Angle (radians) of our last position in a rate servo
    double     numRevs;          // Spiral Scan - current number of revolutions
    double     maxNumRevs;       // Spiral Scan - maximum number of revolutions
};

} // End Simulation namespace
} // End Eaagles namespace

#endif
