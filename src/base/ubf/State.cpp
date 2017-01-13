
#include "openeaagles/base/ubf/State.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace base {
namespace ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(State, "UbfState")
EMPTY_SLOTTABLE(State)
EMPTY_CONSTRUCTOR(State)
EMPTY_DELETEDATA(State)
EMPTY_COPYDATA(State)
EMPTY_SERIALIZER(State)

void State::updateGlobalState()
{
   // Update all my children
   base::PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      if (isComponentSelected()) {
         // When we've selected only one
         if (getSelectedComponent() != nullptr) {
            auto state = dynamic_cast<State*>(getSelectedComponent());
            if (state != nullptr)
               state->updateGlobalState();
         }
      }
      else {
         // When we should update them all
         base::List::Item* item = subcomponents->getFirstItem();
         while (item != nullptr) {
            auto pair = static_cast<base::Pair*>(item->getValue());
            auto obj = static_cast<base::Component*>(pair->object());
            auto state = dynamic_cast<State*>(obj);
            if (state != nullptr)
               state->updateGlobalState();
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
}

void State::updateState(const base::Component* const actor)
{
   // Update all my children
   base::PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      if (isComponentSelected()) {
         // When we've selected only one
         if (getSelectedComponent() != nullptr) {
            auto state = dynamic_cast<State*>(getSelectedComponent());
            if (state != nullptr)
               state->updateState(actor);
         }
      }
      else {
         // When we should update them all
         base::List::Item* item = subcomponents->getFirstItem();
         while (item != nullptr) {
            auto pair = static_cast<base::Pair*>(item->getValue());
            auto obj = static_cast<base::Component*>(pair->object());
            auto state = dynamic_cast<State*>(obj);
            if (state != nullptr)
               state->updateState(actor);
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
}


const State* State::getUbfStateByType(const std::type_info& type) const
{
   const State* p = this;
   if ( !p->isClassType(type) ) {
      const base::Pair* pair = findByType(type);
      if (pair != nullptr) {
         p = dynamic_cast<const State*>( pair->object() );
      }
   }
   return p;
}

}
}
}
