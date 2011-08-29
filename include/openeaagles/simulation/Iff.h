//------------------------------------------------------------------------------
// Class:	Iff
//------------------------------------------------------------------------------
#ifndef __IFF_H_5BF06AD6_B12F_44d8_BD34_26D7FCC25AA4__
#define __IFF_H_5BF06AD6_B12F_44d8_BD34_26D7FCC25AA4__

#include "openeaagles/simulation/Radio.h"

namespace Eaagles {
namespace Simulation {

//------------------------------------------------------------------------------
// Class:	Iff
// Base class:	Basic::Object -> Basic::Component -> Iff
//
// Description: Generic class for all IFF systems (a.k.a. the SQUAWK box)
//
// Form name: Iff
// Slots:
//   mode1         <Number>   ! Mode 1 Code   (range: 00 to 073 octal) (default: 0)
//                            ! -- first digit 0 to 7, second digit 0 to 3 (default: 0)
//   mode2         <Number>   ! Mode 2 Code   (range: 0000 to 07777 octal) (default: 0)
//   mode3a        <Number>   ! Mode 3a Code  (range: 0000 to 07777 octal) (default: 0)
//   mode4a        <Number>   ! Mode 4a Code (default: 0)
//   mode4b        <Number>   ! Mode 4b Code (default: 0)
//   enableMode1   <Number>   ! Mode 1 on (default: 0)
//   enableMode2   <Number>   ! Mode 2 on (default: 0)
//   enableMode3a  <Number>   ! Mode 3a on (default: 0)
//   enableMode4   <Number>   ! Mode 4a on (default: 0)
//   whichMode4    <Number>   ! Which mode 4 are we using (A == false; B == true)
//                            ! (default: false)
//   enableModeC   <Number>   ! Mode C on (default: 0)
//
//------------------------------------------------------------------------------
class Iff : public Radio  
{
   DECLARE_SUBCLASS(Iff,Radio)

public:
   enum {
      PWR_LOW = BaseClass::PWR_LAST,
      PWR_EMERGENCY,
      PWR_LAST       // Hook for subclasses to expand
   };

public:
   Iff();

   bool isEnabledMode1() const                     { return enableMode1; }
   unsigned short getMode1() const                 { return mode1; }
   virtual bool setMode1(const unsigned short m);
   virtual void setEnabledMode1(const bool flg);

   bool isEnabledMode2() const                     { return enableMode2; }
   unsigned short getMode2() const                 { return mode2; }
   virtual bool setMode2(const unsigned short m);
   virtual void setEnabledMode2(const bool flg);

   bool isEnabledMode3a() const                    { return enableMode3a; }
   unsigned short getMode3a() const                { return mode3a; }
   virtual bool setMode3a(const unsigned short m);
   virtual void setEnabledMode3a(const bool flg);

   bool isEnabledMode4() const                     { return enableMode4; }
   unsigned short getMode4A() const                { return mode4a; }
   unsigned short getMode4B() const                { return mode4b; }
   bool getWhichMode4() const                      { return whichMode4; }  // False (4A), True(4B)
   virtual bool setMode4A(const unsigned short m);
   virtual bool setMode4B(const unsigned short m);
   virtual void setEnabledMode4(const bool flg);
   virtual bool setWhichMode4(const bool newM);
   virtual bool zeroMode4Codes();

   bool isEnabledModeC() const                     { return enableModeC; }
   virtual void setEnabledModeC(const bool flg);

   // Slot functions
   virtual bool setSlotMode1(const Basic::Number* const msg);
   virtual bool setSlotMode2(const Basic::Number* const msg);
   virtual bool setSlotMode3a(const Basic::Number* const msg);
   virtual bool setSlotMode4a(const Basic::Number* const msg);
   virtual bool setSlotMode4b(const Basic::Number* const msg);
   virtual bool setSlotEnableMode1(const Basic::Number* const msg);
   virtual bool setSlotEnableMode2(const Basic::Number* const msg);
   virtual bool setSlotEnableMode3a(const Basic::Number* const msg);
   virtual bool setSlotWhichMode4(const Basic::Number* const msg);
   virtual bool setSlotEnableMode4(const Basic::Number* const msg);
   virtual bool setSlotEnableModeC(const Basic::Number* const msg);

   // Basic::Component interface
   virtual void reset();

private:

   // Codes
   unsigned short mode1;      // Mode 1 Code
   unsigned short mode2;      // Mode 2 Code
   unsigned short mode3a;     // Mode 3a Code
   unsigned short mode4a;     // Mode 4a Codes
   unsigned short mode4b;     // Mode 4b Codes

   // Enabled flags
   bool enableMode1;          // Mode 1 on
   bool enableMode2;          // Mode 2 on
   bool enableMode3a;         // Mode 3a on
   bool enableMode4;          // Mode 4 on
   bool enableModeC;          // Mode C on

   // Initial (reset) conditions (set by slot functions)
   unsigned short icMode1;    // Initial Mode 1 Code
   unsigned short icMode2;    // Initial Mode 2 Code
   unsigned short icMode3a;   // Initial Mode 3a Code
   unsigned short icMode4a;   // Initial Mode 4a Code
   unsigned short icMode4b;   // Initial Mode 4b Code
   bool icMode1Flg;           // Initial Mode 1 flag
   bool icMode2Flg;           // Initial Mode 2 flag
   bool icMode3aFlg;          // Initial Mode 3a flag
   bool icMode4Flg;           // Initial Mode 4 flag
   bool icModeCFlg;           // Initial Mode C flag
   bool whichMode4;           // Tells us which mode 4 we are using (A/B)
   bool icWhichMode4;         // Initial Mode 4 mode
};

} // End Simulation namespace
} // End Eaagles namespace

#endif // __IFF_H_5BF06AD6_B12F_44d8_BD34_26D7FCC25AA4__
