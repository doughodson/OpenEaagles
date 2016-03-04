//------------------------------------------------------------------------------
// Class: Route
//------------------------------------------------------------------------------
#ifndef __oe_simulation_Route_H__
#define __oe_simulation_Route_H__

#include "openeaagles/base/Component.h"

namespace oe {
   namespace basic {
      class Identifier;
      class Distance;
      class NauticalMiles;
      class Number;
      class Pair;
      class PairStream;
   }
namespace simulation {
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
//    3) If an action is not ready (i.e., Action::isReadyToStart() is
//       false) then the action will be skipped.
//
//
// Factory name: Route
// Slots:
//    to                <basic::Number>      ! Initial "TO" steerpoint by steerpoint index number (default: 0)
//                      <basic::Identifier>  ! or by steerpoint name (default: 0)
//
//    autoSequence      <basic::Number>      ! Auto sequence flag (default: true)
//
//    autoSeqDistance   <basic::Distance>    ! Distance to auto sequence by distance unit
//                      <basic::Number>      ! or by nautical miles (default: 2.0NM)
//
//    wrap              <basic::Boolean>     ! Route wrap flag (wrap back to the beginning when past the end)
//                                           ! (default: true)
//
//------------------------------------------------------------------------------
class Route : public basic::Component
{
   DECLARE_SUBCLASS(Route, basic::Component)

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
   virtual basic::Pair* findSteerpoint(const Steerpoint* const sp);
   virtual basic::Pair* findSteerpoint(const char* const name);
   virtual basic::Pair* findSteerpoint(const unsigned int idx);
   virtual const basic::Pair* findSteerpoint(const Steerpoint* const sp) const;
   virtual const basic::Pair* findSteerpoint(const char* const name) const;
   virtual const basic::Pair* findSteerpoint(const unsigned int idx) const;

   // Get the route -- or use basic::Component::getComponents()
   virtual unsigned int getSteerpoints(basic::safe_ptr<Steerpoint>* const stptList, const unsigned int max);    // Start from 'to'
   virtual unsigned int getAllSteerpoints(basic::safe_ptr<Steerpoint>* const stptList, const unsigned int max); // All
   virtual unsigned int getNumberOfSteerpoints() const;   // return the number of steerpoint (components) in our route

   // Insert a steerpoint at 'pos' position in our list;
   //    a) if 'pos' is less than 0 then its added to the start of the list
   //    b) if 'pos' is 0 (default) or greater than getNumberOfSteerpoints()
   //       then it's added to the end of the list
   //    c) otherwise 'pos' must be between 1 .. getNumberOfSteerpoints()
   virtual bool insertSteerpoint(Steerpoint* const sp, const int pos = 0);

   // Delete a steerpoint
   virtual bool deleteSteerpoint(Steerpoint* const sp);

   // Replace all of our steerpoints and sets our 'to' steerpoint to 'newStptIdx'
   virtual bool replaceAllSteerpoints(basic::PairStream* const newSteerpointList, unsigned int newStptIdx = 1);

   // clears out all of our steerpoints
   virtual bool deleteAllSteerpoints();

   // Trigger the 'to' steerpoint action; auto sequencing only
   virtual void triggerAction();

   void updateData(const LCreal dt = 0.0) override;
   bool event(const int event, basic::Object* const obj = nullptr) override;
   void reset() override;

protected:
    // Compute nav steering data for each steerpoint.
    virtual void computeSteerpointData(const LCreal dt, const Navigation* const nav);

    // Auto Sequence through Steerpoints
    virtual void autoSequencer(const LCreal dt, const  Navigation* const nav);

    // Slot functions
    bool setSlotTo(const basic::Identifier* const msg);
    bool setSlotTo(const basic::Number* const msg);
    bool setSlotAutoSequence(const basic::Number* const msg);
    bool setSlotAutoSeqDistance(const basic::Distance* const msg);
    bool setSlotAutoSeqDistance(const basic::Number* const msg);
    bool setSlotWrap(const basic::Number* const msg);

    void processComponents(
         basic::PairStream* const list,        // Source list of components
         const std::type_info& filter,         // Type filter
         basic::Pair* const add = 0,           // Optional pair to add
         basic::Component* const remove = 0    // Optional subcomponent to remove
       ) override;

private:
   const basic::Pair* findSteerpointImp(const Steerpoint* const sp) const;
   const basic::Pair* findSteerpointImp(const char* const name) const;
   const basic::Pair* findSteerpointImp(const unsigned int idx) const;
   const Steerpoint* getSteerpointImp() const;

   basic::safe_ptr<basic::Pair>         to;             // "To" Steerpoint pair [ name steerpoint ]
   basic::safe_ptr<const basic::String> initToStptName; // Name of the initial "to" steerpoint
   unsigned int              initToStptIdx;             // Index of the initial "to" steerpoint
   unsigned int              stptIdx;                   // Steerpoint index [ 1 ... n ] in list
   LCreal                    autoSeqDistNM;             // Distance to auto sequence (NM)
   bool                      autoSeq;                   // Auto sequence of steerpoint
   bool                      wrap;                      // Wrap around route when inc or dec 'to' steerpoint
};

//------------------------------------------------------------------------------
// Inline functions
//------------------------------------------------------------------------------

inline Steerpoint* Route::getSteerpoint()
{
   return const_cast<Steerpoint*>(static_cast<const Steerpoint*>(getSteerpointImp()));
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

inline basic::Pair* Route::findSteerpoint(const Steerpoint* const sp)
{
   return const_cast<basic::Pair*>(static_cast<const basic::Pair*>(findSteerpointImp(sp)));
}

inline basic::Pair* Route::findSteerpoint(const char* const name)
{
   return const_cast<basic::Pair*>(static_cast<const basic::Pair*>(findSteerpointImp(name)));
}

inline basic::Pair* Route::findSteerpoint(const unsigned int idx)
{
   return const_cast<basic::Pair*>(static_cast<const basic::Pair*>(findSteerpointImp(idx)));
}

inline const basic::Pair* Route::findSteerpoint(const Steerpoint* const sp) const
{
   return findSteerpointImp(sp);
}

inline const basic::Pair* Route::findSteerpoint(const char* const name) const
{
   return findSteerpointImp(name);
}

inline const basic::Pair* Route::findSteerpoint(const unsigned int idx) const
{
   return findSteerpointImp(idx);
}

} // End simulation namespace
} // End oe namespace

#endif
