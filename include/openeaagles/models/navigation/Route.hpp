
#ifndef __oe_models_Route_H__
#define __oe_models_Route_H__

#include "openeaagles/base/Component.hpp"

namespace oe {
namespace base { class Identifier; class Distance; class NauticalMiles; class Number; class Pair; class PairStream; }
namespace models {
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
//    to                <base::Number>      ! Initial "TO" steerpoint by steerpoint index number (default: 0)
//                      <base::Identifier>  ! or by steerpoint name (default: 0)
//
//    autoSequence      <base::Number>      ! Auto sequence flag (default: true)
//
//    autoSeqDistance   <base::Distance>    ! Distance to auto sequence by distance unit
//                      <base::Number>      ! or by nautical miles (default: 2.0NM)
//
//    wrap              <base::Boolean>     ! Route wrap flag (wrap back to the beginning when past the end)
//                                           ! (default: true)
//
//------------------------------------------------------------------------------
class Route : public base::Component
{
   DECLARE_SUBCLASS(Route, base::Component)

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
   virtual base::Pair* findSteerpoint(const Steerpoint* const sp);
   virtual base::Pair* findSteerpoint(const char* const name);
   virtual base::Pair* findSteerpoint(const unsigned int idx);
   virtual const base::Pair* findSteerpoint(const Steerpoint* const sp) const;
   virtual const base::Pair* findSteerpoint(const char* const name) const;
   virtual const base::Pair* findSteerpoint(const unsigned int idx) const;

   // Get the route -- or use base::Component::getComponents()
   virtual unsigned int getSteerpoints(base::safe_ptr<Steerpoint>* const stptList, const unsigned int max);    // Start from 'to'
   virtual unsigned int getAllSteerpoints(base::safe_ptr<Steerpoint>* const stptList, const unsigned int max); // All
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
   virtual bool replaceAllSteerpoints(base::PairStream* const newSteerpointList, unsigned int newStptIdx = 1);

   // clears out all of our steerpoints
   virtual bool deleteAllSteerpoints();

   // Trigger the 'to' steerpoint action; auto sequencing only
   virtual void triggerAction();

   virtual void updateData(const double dt = 0.0) override;
   virtual bool event(const int event, base::Object* const obj = nullptr) override;
   virtual void reset() override;

protected:
    // Compute nav steering data for each steerpoint.
    virtual void computeSteerpointData(const double dt, const Navigation* const nav);

    // Auto Sequence through Steerpoints
    virtual void autoSequencer(const double dt, const  Navigation* const nav);

    // Slot functions
    bool setSlotTo(const base::Identifier* const msg);
    bool setSlotTo(const base::Number* const msg);
    bool setSlotAutoSequence(const base::Number* const msg);
    bool setSlotAutoSeqDistance(const base::Distance* const msg);
    bool setSlotAutoSeqDistance(const base::Number* const msg);
    bool setSlotWrap(const base::Number* const msg);

    virtual void processComponents(
         base::PairStream* const list,              // Source list of components
         const std::type_info& filter,              // Type filter
         base::Pair* const add = nullptr,           // Optional pair to add
         base::Component* const remove = nullptr    // Optional subcomponent to remove
       ) override;

private:
   const base::Pair* findSteerpointImp(const Steerpoint* const sp) const;
   const base::Pair* findSteerpointImp(const char* const name) const;
   const base::Pair* findSteerpointImp(const unsigned int idx) const;
   const Steerpoint* getSteerpointImp() const;

   base::safe_ptr<base::Pair>         to;               // "To" Steerpoint pair [ name steerpoint ]
   base::safe_ptr<const base::String> initToStptName;   // Name of the initial "to" steerpoint
   unsigned int              initToStptIdx;             // Index of the initial "to" steerpoint
   unsigned int              stptIdx;                   // Steerpoint index [ 1 ... n ] in list
   double                    autoSeqDistNM;             // Distance to auto sequence (NM)
   bool                      autoSeq;                   // Auto sequence of steerpoint
   bool                      wrap;                      // Wrap around route when inc or dec 'to' steerpoint
};

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

inline base::Pair* Route::findSteerpoint(const Steerpoint* const sp)
{
   return const_cast<base::Pair*>(static_cast<const base::Pair*>(findSteerpointImp(sp)));
}

inline base::Pair* Route::findSteerpoint(const char* const name)
{
   return const_cast<base::Pair*>(static_cast<const base::Pair*>(findSteerpointImp(name)));
}

inline base::Pair* Route::findSteerpoint(const unsigned int idx)
{
   return const_cast<base::Pair*>(static_cast<const base::Pair*>(findSteerpointImp(idx)));
}

inline const base::Pair* Route::findSteerpoint(const Steerpoint* const sp) const
{
   return findSteerpointImp(sp);
}

inline const base::Pair* Route::findSteerpoint(const char* const name) const
{
   return findSteerpointImp(name);
}

inline const base::Pair* Route::findSteerpoint(const unsigned int idx) const
{
   return findSteerpointImp(idx);
}

}
}

#endif
