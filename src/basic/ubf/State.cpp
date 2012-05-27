
#include "openeaagles/basic/ubf/State.h"

#include "openeaagles/basic/Pair.h"
#include "openeaagles/basic/PairStream.h"

namespace Eaagles {
namespace Basic {

//IMPLEMENT_SUBCLASS(UbfState, "UbfState")
IMPLEMENT_ABSTRACT_SUBCLASS(State, "State")
EMPTY_SLOTTABLE(State)
EMPTY_CONSTRUCTOR(State)
EMPTY_DELETEDATA(State)
EMPTY_COPYDATA(State)
EMPTY_SERIALIZER(State)

void State::updateGlobalState(void)
{
   // Update all my children
   Basic::PairStream* subcomponents = getComponents();
   if (subcomponents != 0) {
      if (isComponentSelected() != 0) {
         // When we've selected only one
         if (getSelectedComponent() != 0) {
            State* state = dynamic_cast<State*>(getSelectedComponent());
            if (state != 0)
               state->updateGlobalState();
         }
      }
      else {
         // When we should update them all
         Basic::List::Item* item = subcomponents->getFirstItem();
         while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Basic::Component* obj = (Basic::Component*)( pair->object() );
            State* state = dynamic_cast<State*>(obj);
            if (state != 0)
               state->updateGlobalState();
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = 0;
   }
}

void State::updateState(const Basic::Component* const actor)
{
   // Update all my children
   Basic::PairStream* subcomponents = getComponents();
   if (subcomponents != 0) {
      if (isComponentSelected() != 0) {
         // When we've selected only one
         if (getSelectedComponent() != 0) {
            State* state = dynamic_cast<State*>(getSelectedComponent());
            if (state != 0)
               state->updateState(actor);
         }
      }
      else {
         // When we should update them all
         Basic::List::Item* item = subcomponents->getFirstItem();
         while (item != 0) {
            Basic::Pair* pair = (Basic::Pair*)(item->getValue());
            Basic::Component* obj = (Basic::Component*)( pair->object() );
            State* state = dynamic_cast<State*>(obj);
            if (state != 0)
               state->updateState(actor);
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = 0;
   }
}


const State* State::getUbfStateByType(const std::type_info& type) const
{
   const State* p = this;
   if ( !p->isClassType(type) ) {
      const Basic::Pair* pair = findByType(type);
      if (pair != 0) {
         p = dynamic_cast<const State*>( pair->object() );
      }
   }
   return p;
}


} // End Basic namespace
} // End Eaagles namespace

