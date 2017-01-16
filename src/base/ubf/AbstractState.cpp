
#include "openeaagles/base/ubf/AbstractState.hpp"

#include "openeaagles/base/Pair.hpp"
#include "openeaagles/base/PairStream.hpp"

namespace oe {
namespace base {
namespace ubf {

IMPLEMENT_ABSTRACT_SUBCLASS(AbstractState, "AbstractState")
EMPTY_SLOTTABLE(AbstractState)
EMPTY_CONSTRUCTOR(AbstractState)
EMPTY_DELETEDATA(AbstractState)
EMPTY_COPYDATA(AbstractState)
EMPTY_SERIALIZER(AbstractState)

void AbstractState::updateGlobalState()
{
   // Update all my children
   base::PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      if (isComponentSelected()) {
         // When we've selected only one
         if (getSelectedComponent() != nullptr) {
            const auto state = dynamic_cast<AbstractState*>(getSelectedComponent());
            if (state != nullptr)
               state->updateGlobalState();
         }
      }
      else {
         // When we should update them all
         base::List::Item* item = subcomponents->getFirstItem();
         while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto obj = static_cast<base::Component*>(pair->object());
            const auto state = dynamic_cast<AbstractState*>(obj);
            if (state != nullptr)
               state->updateGlobalState();
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
}

void AbstractState::updateState(const base::Component* const actor)
{
   // Update all my children
   base::PairStream* subcomponents = getComponents();
   if (subcomponents != nullptr) {
      if (isComponentSelected()) {
         // When we've selected only one
         if (getSelectedComponent() != nullptr) {
            const auto state = dynamic_cast<AbstractState*>(getSelectedComponent());
            if (state != nullptr)
               state->updateState(actor);
         }
      }
      else {
         // When we should update them all
         base::List::Item* item = subcomponents->getFirstItem();
         while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto obj = static_cast<base::Component*>(pair->object());
            const auto state = dynamic_cast<AbstractState*>(obj);
            if (state != nullptr)
               state->updateState(actor);
            item = item->getNext();
         }
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
}


const AbstractState* AbstractState::getUbfStateByType(const std::type_info& type) const
{
   const AbstractState* p = this;
   if ( !p->isClassType(type) ) {
      const base::Pair* pair = findByType(type);
      if (pair != nullptr) {
         p = dynamic_cast<const AbstractState*>( pair->object() );
      }
   }
   return p;
}

}
}
}
