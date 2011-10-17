//------------------------------------------------------------------------------
// Class: Route
//------------------------------------------------------------------------------
#ifndef __Eaagles_Simulation_Route_H__
#define __Eaagles_Simulation_Route_H__

#include "openeaagles/basic/Component.h"

namespace Eaagles {
   namespace Basic {
      class Identifier;
      class Distance;
      class NauticalMiles;
      class Number;
      class Pair;
      class PairStream;
   }
namespace Simulation {
class Navigation;
class Steerpoint;

//------------------------------------------------------------------------------
// Class: Route
//
// Description: Generic route manager -- maintains steerponts, to/from, 
//              auto sequencing, steering data, and steerpoint actions.
//
// Note:
//    1) Steerpoints are added using the 'components' slot
//
//    2) Steerpoint actions are only processed in 'auto' sequence mode.
//       Also, the current action will be cancelled if switched to 'manual'
//       sequence mode.
//
//    3) If an action is not ready (i.e.,, Action::isReadyToStart() is
//       false) then the action will be skipped.
//
//
// Form Name: Route
// Slots:
//    to                <Basic::Number>      ! Initial "TO" steerpoint by steerpoint index number (default: 0)
//                      <Basic::Identifier>  ! or by steerpoint name (default: 0)
//
//    autoSequence      <Basic::Number>      ! Auto sequence flag (default: true)
//
//    autoSeqDistance   <Basic::Distance>    ! Distance to auto sequence by distance unit
//                      <Basic::Number>      ! or by nautical miles (default: 2.0NM)
//
//    wrap              <Basic::Boolean>     ! Route wrap flag (wrap back to the beginning when past the end)
//                                           ! (default: true)
//
//------------------------------------------------------------------------------
class Route : public Basic::Component
{
   DECLARE_SUBCLASS(Route,Basic::Component)

public:
   Route();

   // Get the current 'to' steerpoint    
   virtual Steerpoint* getSteerpoint();               // The 'to' steerpoint
   virtual const Steerpoint* getSteerpoint() const;   // The 'to' steerpoint (const version)
   virtual const char* getSteerpointName() const;     // Name of the 'to' steerpoint
   virtual unsigned int getStptIndex() const;         // Index of the 'to' steerpoint

   // Auto sequence mode
   virtual bool isAutoSequence() const;
   virtual bool setAutoSequence(const bool flg);

   // Auto sequence distance (NM)
   virtual double getAutoSeqDistance() const;
   virtual bool setAutoSeqDistance(const double nm);

   // Wrap back around to first steerpoint
   virtual bool isWrapEnabled() const;
   virtual bool setWrapEnable(const bool flg);

   // Manual increment/decrement current steerpoint index
   virtual bool incStpt();
   virtual bool decStpt();

   // Change to fly direct-to this steerpoint
   virtual bool directTo(const Steerpoint* const sp);
   virtual bool directTo(const char* const name);
   virtual bool directTo(const unsigned int idx);

   // Find steerpoint (pair)
   virtual Basic::Pair* findSteerpoint(const Steerpoint* const sp);
   virtual Basic::Pair* findSteerpoint(const char* const name);
   virtual Basic::Pair* findSteerpoint(const unsigned int idx);
   virtual const Basic::Pair* findSteerpoint(const Steerpoint* const sp) const;
   virtual const Basic::Pair* findSteerpoint(const char* const name) const;
   virtual const Basic::Pair* findSteerpoint(const unsigned int idx) const;

   // Get the route -- or use Basic::Component::getComponents()
   virtual unsigned int getSteerpoints(SPtr<Steerpoint>* const stptList, const unsigned int max);    // Start from 'to'
   virtual unsigned int getAllSteerpoints(SPtr<Steerpoint>* const stptList, const unsigned int max); // All
   virtual unsigned int getNumberOfSteerpoints() const;   // return the number of steerpoint (components) in our route

   // adds a steerpoint to 'pos' position in our list;
   //    a) if 'pos' is less than 0 then its added to the start of the list
   //    b) if 'pos' is 0 (default) or equal to one plus getNumberOfSteerpoints()
   //       then it's added to the end of the list
   //    c) otherwise 'pos' must be between 1 .. getNumberOfSteerpoints()
   virtual bool insertSteerpoint(Steerpoint* const sp, const unsigned int pos = 0);

   // delete a steerpoint
   virtual bool deleteSteerpoint(Steerpoint* const sp); 

   // Replace all of our steerpoints and sets our 'to' steerpoint to 'newStptIdx'
   virtual bool replaceAllSteerpoints(Basic::PairStream* const newSteerpointList, unsigned int newStptIdx = 1);

   // clears out all of our steerpoints
   virtual bool deleteAllSteerpoints();

   // Trigger the 'to' steerpoint action; auto sequencing only
   virtual void triggerAction();

   // Basic::Component interface
   virtual void updateData(const LCreal dt = 0.0);
   virtual bool event(const int event, Basic::Object* const obj = 0);
   virtual void reset();

protected:
    // Compute nav steering data for each steerpoint.
    virtual void computeSteerpointData(const LCreal dt, const Navigation* const nav);

    // Auto Sequence through Steerpoints
    virtual void autoSequencer(const LCreal dt, const  Navigation* const nav);

    // Slot functions
    bool setSlotTo(const Basic::Identifier* const msg);
    bool setSlotTo(const Basic::Number* const msg);
    bool setSlotAutoSequence(const Basic::Number* const msg);
    bool setSlotAutoSeqDistance(const Basic::Distance* const msg);
    bool setSlotAutoSeqDistance(const Basic::Number* const msg);
    bool setSlotWrap(const Basic::Number* const msg);

    // Basic::Component interface
    virtual void processComponents(
         Basic::PairStream* const list,        // Source list of components
         const std::type_info& filter,           // Type filter
         Basic::Pair* const add = 0,           // Optional pair to add
         Basic::Component* const remove = 0    // Optional subcomponent to remove
       );

private:
   const Basic::Pair* findSteerpointImp(const Steerpoint* const sp) const;
   const Basic::Pair* findSteerpointImp(const char* const name) const;
   const Basic::Pair* findSteerpointImp(const unsigned int idx) const;
   const Steerpoint* getSteerpointImp() const;

   SPtr<Basic::Pair>      to;                // "To" Steerpoint pair [ name steerpoint ]
   SPtr<const Basic::String> initToStptName; // Name of the inital "to" steerpoint
   unsigned int      initToStptIdx;            // Index of the initial "to" steerpoint
   unsigned int      stptIdx;                  // Steerpoint index [ 1 ... n ] in list
   LCreal            autoSeqDistNM;            // Distance to auto sequence               (NM)
   bool              autoSeq;                  // Auto sequence of steerpoint
   bool              wrap;                     // Wrap around route when inc or dec 'to' steerpoint
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

inline Steerpoint* Route::getSteerpoint()
{
   return (Steerpoint*) getSteerpointImp();
}

inline const Steerpoint* Route::getSteerpoint() const
{
   return getSteerpointImp();
}

inline unsigned int Route::getStptIndex() const
{
   return stptIdx;
}

inline bool Route::isAutoSequence() const
{
   return autoSeq;
}

inline bool Route::isWrapEnabled() const
{
   return wrap;
}

inline double Route::getAutoSeqDistance() const
{
   return autoSeqDistNM;
}

inline Basic::Pair* Route::findSteerpoint(const Steerpoint* const sp)
{
   return (Basic::Pair*) findSteerpointImp(sp);
}

inline Basic::Pair* Route::findSteerpoint(const char* const name)
{
   return (Basic::Pair*) findSteerpointImp(name);
}

inline Basic::Pair* Route::findSteerpoint(const unsigned int idx)
{
   return (Basic::Pair*) findSteerpointImp(idx);
}

inline const Basic::Pair* Route::findSteerpoint(const Steerpoint* const sp) const
{
   return findSteerpointImp(sp);
}

inline const Basic::Pair* Route::findSteerpoint(const char* const name) const
{
   return findSteerpointImp(name);
}

inline const Basic::Pair* Route::findSteerpoint(const unsigned int idx) const
{
   return findSteerpointImp(idx);
}

} // End Simulation namespace
} // End Eaagles namespace

#endif
