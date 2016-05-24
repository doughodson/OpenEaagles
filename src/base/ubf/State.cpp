
#include "openeaagles/base/ubf/State.h"

#include "openeaagles/base/Pair.h"
#include "openeaagles/base/PairStream.h"

namespace oe {
namespace base {
namespace ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(State, "UbfState")
EMPTY_SLOTTABLE(State)
EMPTY_CONSTRUCTOR(State)
EMPTY_DELETEDATA(State)
EMPTY_COPYDATA(State)
EMPTY_SERIALIZER(State)

void State::updateGlobalState(void)
{
   // Update all my children
   base::PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      if (isComponentSelected()) {
         // When we've selected only one
         if (getSelectedComponent() != nullptr) {
            State* state = dynamic_cast<State*>(getSelectedComponent());
            if (state != nullptr)
               state->updateGlobalState();
         }
      }
      else {
         // When we should update them all
         base::List::Item* item = subcomponents->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            base::Component* obj = static_cast<base::Component*>(pair->object());
            State* state = dynamic_cast<State*>(obj);
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
            State* state = dynamic_cast<State*>(getSelectedComponent());
            if (state != nullptr)
               state->updateState(actor);
         }
      }
      else {
         // When we should update them all
         base::List::Item* item = subcomponents->getFirstItem();
         while (item != nullptr) {
            base::Pair* pair = static_cast<base::Pair*>(item->getValue());
            base::Component* obj = static_cast<base::Component*>(pair->object());
            State* state = dynamic_cast<State*>(obj);
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

