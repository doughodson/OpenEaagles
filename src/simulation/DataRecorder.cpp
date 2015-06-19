//------------------------------------------------------------------------------
// Classes:
//    DataRecorder      -- Abstract data recorder class
//    RecorderComponent -- Base class for all data recorder components
//------------------------------------------------------------------------------

#include "openeaagles/simulation/DataRecorder.h"

#include "openeaagles/simulation/Simulation.h"
#include "openeaagles/simulation/Station.h"

#include "openeaagles/basic/List.h"

namespace Eaagles {
namespace Simulation {

//==============================================================================
// Class DataRecorder
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(DataRecorder,"Abstract_DataRecorder")
EMPTY_SLOTTABLE(DataRecorder)
EMPTY_SERIALIZER(DataRecorder)

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
DataRecorder::DataRecorder()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void DataRecorder::initData()
{
   sta = nullptr;
   sim = nullptr;
}

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void DataRecorder::copyData(const DataRecorder& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   sta = nullptr;
   sim = nullptr;
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
void DataRecorder::deleteData()
{
   sta = nullptr;
   sim = nullptr;
}

//------------------------------------------------------------------------------
// Background thread processing of the data records
//------------------------------------------------------------------------------
void DataRecorder::processRecords()
{
   // nothing to do at this level
}

// Our parent Station
Station* DataRecorder::getStation()
{
   if (sta == nullptr) {
      getStationImp();
   }
   return sta;
}

// Our parent Station (const version)
const Station* DataRecorder::getStation() const
{
   if (sta == nullptr) {
      (const_cast<DataRecorder*>(this))->getStationImp();
   }
   return sta;
}

// Find our parent Station
Station* DataRecorder::getStationImp()
{
   if (sta == nullptr) {
      sta = static_cast<Station*>(findContainerByType(typeid(Station)));
      if (sta == nullptr && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "DataRecorder::getStationImp(): ERROR, unable to locate the Station class!" << std::endl;
      }
   }
   return sta;
}


// The simulation
Simulation* DataRecorder::getSimulation()
{
   if (sim == nullptr) {
      getSimulationImp();
   }
   return sim;
}

const Simulation* DataRecorder::getSimulation() const
{
   if (sim == nullptr) {
      (const_cast<DataRecorder*>(this))->getSimulationImp();
   }
   return sim;
}

// The simulation
Simulation* DataRecorder::getSimulationImp()
{
   if (sim == nullptr) {
      Station* p = getStation();
      if (p != nullptr) sim = p->getSimulation();
   }
   return sim;
}


bool DataRecorder::recordDataImp(const unsigned int id, const Basic::Object* pObjects[4], const double values[4])
{
   return true;
}


//==============================================================================
// Class RecorderComponent
//==============================================================================
IMPLEMENT_ABSTRACT_SUBCLASS(RecorderComponent,"RecorderComponent")

// Slot table for this form type
BEGIN_SLOTTABLE(RecorderComponent)
    "enabledList",         // 1)  List of enabled data records
    "disabledList",        // 2)  List of disabled data records
END_SLOTTABLE(RecorderComponent)

// Map slot table to handles
BEGIN_SLOT_MAP(RecorderComponent)
    ON_SLOT( 1, setSlotEnabledList,  Basic::List)
    ON_SLOT( 2, setSlotDisabledList, Basic::List)
END_SLOT_MAP()

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// copyData() -- copy member data
//------------------------------------------------------------------------------
void RecorderComponent::copyData(const RecorderComponent& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setEnabledList(org.enabledList, org.numEnabled);
   setDisabledList(org.disabledList, org.numDisabled);
}

//------------------------------------------------------------------------------
// deleteData() -- delete member data
//------------------------------------------------------------------------------
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
bool RecorderComponent::setSlotEnabledList(const Basic::List* const list)
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

bool RecorderComponent::setSlotDisabledList(const Basic::List* const list)
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


//------------------------------------------------------------------------------
// getSlotByIndex() for Component
//------------------------------------------------------------------------------
Basic::Object* RecorderComponent::getSlotByIndex(const int si)
{
   return BaseClass::getSlotByIndex(si);
}


//------------------------------------------------------------------------------
// serialize
//------------------------------------------------------------------------------
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

} // End Simulation namespace
} // End Eaagles namespace

