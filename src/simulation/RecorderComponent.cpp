
#include "openeaagles/simulation/RecorderComponent.hpp"

#include "openeaagles/base/List.hpp"
#include <iostream>

namespace oe {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(RecorderComponent, "RecorderComponent")

BEGIN_SLOTTABLE(RecorderComponent)
    "enabledList",         // 1)  List of enabled data records
    "disabledList",        // 2)  List of disabled data records
END_SLOTTABLE(RecorderComponent)

BEGIN_SLOT_MAP(RecorderComponent)
    ON_SLOT( 1, setSlotEnabledList,  base::List)
    ON_SLOT( 2, setSlotDisabledList, base::List)
END_SLOT_MAP()

RecorderComponent::RecorderComponent()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void RecorderComponent::initData()
{
   enabledList = nullptr;
   numEnabled = 0;

   disabledList = nullptr;
   numDisabled = 0;
}

void RecorderComponent::copyData(const RecorderComponent& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setEnabledList(org.enabledList, org.numEnabled);
   setDisabledList(org.disabledList, org.numDisabled);
}

void RecorderComponent::deleteData()
{
   setEnabledList(nullptr, 0);
   setDisabledList(nullptr, 0);
}

//------------------------------------------------------------------------------
// Set a list of 'n' of data records enabled for processing
//------------------------------------------------------------------------------
bool RecorderComponent::setEnabledList(const unsigned int* const list, const unsigned int n)
{
   // Remove the old list
   if (enabledList != nullptr) {
      delete[] enabledList;
      enabledList = nullptr;
   }
   numEnabled = 0;

   // Create the new list
   if (list != nullptr && n > 0) {
      enabledList = new unsigned int[n];
      for (unsigned int i = 0; i < n; i++) {
         enabledList[i] = list[i];
      }
      numEnabled = n;
   }

   return true;
}


//------------------------------------------------------------------------------
// Set a list of 'n' of data records disabled from being processed
//------------------------------------------------------------------------------
bool RecorderComponent::setDisabledList(const unsigned int* const list, const unsigned int n)
{
   // Remove the old list
   if (disabledList != nullptr) {
      delete[] disabledList;
      disabledList = nullptr;
   }
   numDisabled = 0;

   // Create the new list
   if (list != nullptr && n > 0) {
      disabledList = new unsigned int[n];
      for (unsigned int i = 0; i < n; i++) {
         disabledList[i] = list[i];
      }
      numDisabled = n;
   }

   return true;
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool RecorderComponent::setSlotEnabledList(const base::List* const list)
{
   unsigned int n2 = 0;
   unsigned int* p2 = nullptr;

   // Items in the list ...
   unsigned int n = list->entries();
   if (n > 0) {
      // get the numbers from the list and set them as the filter list
      int* p1 = new int[n];
      unsigned int n1 = list->getNumberList(p1, n);
      if (n1 > 0) {
         // Make sure they're positive
         p2 = new unsigned int[n1];
         for (unsigned int i = 0; i < n1; i++) {
            if (p1[i] >= 0) {
               p2[n2++] = static_cast<unsigned int>(p1[i]);
            }
         }
      }
      delete[] p1;
      p1 = nullptr;
   }

   // Set the list --
   setEnabledList(p2, n2);
   if (p2 != nullptr) {
      delete[] p2;
      p2 = nullptr;
   }

   return true;
}

bool RecorderComponent::setSlotDisabledList(const base::List* const list)
{
   unsigned int n2 = 0;
   unsigned int* p2 = nullptr;

   // Items in the list ...
   unsigned int n = list->entries();
   if (n > 0) {
      // get the numbers from the list and set them as the filter list
      int* p1 = new int[n];
      unsigned int n1 = list->getNumberList(p1, n);
      if (n1 > 0) {
         // Make sure they're positive
         p2 = new unsigned int[n1];
         for (unsigned int i = 0; i < n1; i++) {
            if (p1[i] >= 0) {
               p2[n2++] = static_cast<unsigned int>(p1[i]);
            }
         }
      }
      delete[] p1;
      p1 = nullptr;
   }

   // Set the list --
   setDisabledList(p2, n2);
   if (p2 != nullptr) {
      delete[] p2;
      p2 = nullptr;
   }

   return true;
}

std::ostream& RecorderComponent::serialize(std::ostream& sout, const int i, const bool slotsOnly) const
{
   int j = 0;
   if ( !slotsOnly ) {
      //indent(sout,i);
      sout << "( " << getFactoryName() << std::endl;
      j = 4;
   }

   // Enabled list -- printing as numeric values only
   if (numEnabled > 0 && enabledList != nullptr) {
      indent(sout, i + j);
      sout << "enabledList: " << "[";
      for (unsigned int i = 0; i < numEnabled; i++) sout << " " << enabledList[i];
      sout << " ]" << std::endl;
   }

   // Disabled list -- printing as numeric values only
   if (numDisabled > 0 && disabledList != nullptr) {
      indent(sout, i + j);
      sout << "disabledList: " << "[";
      for (unsigned int i = 0; i < numDisabled; i++) sout << " " << disabledList[i];
      sout << " ]" << std::endl;
   }

   if ( !slotsOnly ) {
      indent(sout,i);
      sout << ")" << std::endl;
   }

   return sout;
}

}
}
